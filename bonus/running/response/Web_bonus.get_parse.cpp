/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Web_bonus.get_parse.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 16:00:30 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 16:22:08 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"

vector<Webserver>::iterator Webserver::target_server(get_parse* g_parse, v_servers& servers)
{
	vector<Webserver>::iterator it;
	if (g_parse->host == "localhost")
		g_parse->host.assign("127.0.0.1");
	for (it = servers.begin(); it != servers.end(); it++) {
		for (config_t::iterator ptr = it->_server_data.begin(); ptr != it->_server_data.end(); ptr++) {
			if (ptr->second == "0.0.0.0")
				g_parse->host.assign("0.0.0.0");
			if (ptr->second == g_parse->host || (g_parse->host == "0.0.0.0"))
				for (config_t::iterator ptr_2 = it->_server_data.begin(); ptr_2 != it->_server_data.end(); ptr_2++)
					if (ptr_2->second == g_parse->port)
						return it;
		}
	}
	return it;
}


string Webserver::parse_path(string path, get_parse* g_parse)
{
	string tmp_path, type;
	vector<string> data = split(path, "/");
	for (vector<string>::iterator it = data.begin(); it != data.end(); it++)
	{

		check_query(*it, g_parse);
		for (size_t i = 0; i < (*it).length(); i++) {

			if ((*it)[i] == '.' && find_last_point((*it), i) && it + 1 == data.end())
			{
				g_parse->file_type = (*it).substr(i + 1, (*it).length() - i);
				g_parse->file_name = "/" + *it;
				// query_get_parsing(g_parse);
				return tmp_path.substr(0, tmp_path.length() - 1);
			}
		}
		if (it == data.begin())
			tmp_path += "/" + *it + "/";
		else
			tmp_path += *it + "/";
	}
	if (tmp_path.empty())
		return tmp_path = "/";
	return tmp_path.substr(0, tmp_path.length() - 1);
}


void Webserver::_parse_header(get_parse *g_parse, req_t* req)
{
	v_str_t	str, tmp;
	string	header;

	header = req->status_line + "\r\n" + req->headers;
	str = split(header, "\n");
	g_parse->status = 200;
	for (v_str_t::iterator str_it = str.begin(); str_it != str.end(); str_it++) {
		if ((*str_it).find("HTTP/") != string::npos) {
			tmp = split((*str_it), " ");
			g_parse->http_v = trim(tmp[2].substr(5, tmp[2].length() - 3).c_str(), " \n\r\f\t");
			g_parse->path = fix_path(tmp[1]);
			tmp.clear();
		}
		if ((*str_it).find("Host:") != string::npos) {
			tmp = split((*str_it), ":");
			g_parse->host = trim(tmp[1], " \n\r\f\t");
			g_parse->port = trim(tmp[2], " \n\r\f\t");
			tmp.clear();
		}
		if ((*str_it).find("Cache-Control:") != string::npos) {
			tmp = split((*str_it), ":");
			g_parse->cache_type = trim(tmp[1], " \n\r\f\t");
			tmp.clear();
		}
		if ((*str_it).find("Content-Length:") != string::npos) {
			tmp = split((*str_it), " ");
			g_parse->content_length = atol(trim(tmp[1], " \n\r\f\t").c_str());
			tmp.clear();
		}
		if ((*str_it).find("Content-Type:") != string::npos) {
			g_parse->content_type = *str_it;
			break;
		}
	}
	str.clear();
	g_parse->path = parse_path(g_parse->path, g_parse); // path funtion
}


bool Webserver::_find_location(get_parse *g_parse, v_servers::iterator &server_it, map_strings &locs, string &root)
{
	if (server_it->_location_data.find(g_parse->path) != server_it->_location_data.end()) {
		locs = server_it->_location_data[g_parse->path];
	}
	else if (server_it->_location_data.find("/") != server_it->_location_data.end()) {
		locs = server_it->_location_data["/"];
	}
	else if (server_it->_server_data.find("root") != server_it->_server_data.end()) {
		locs.insert(make_pair("root", multimap_value(server_it->_server_data, "root")));
	}
	if (g_parse->file_name.empty() && locs.find("allow")->second.find("GET") != string::npos) {
		root += locs["root"] + g_parse->path + "/" + locs["index"];
	}
	else if (locs.find("allow")->second.find("GET") != string::npos) {
		root += locs["root"] + g_parse->path + g_parse->file_name;
	}
	else {
		g_parse->status = 405;
		root = multimap_value(server_it->_server_data, "error_page_405");
	}
	// if (g_parse->cache_type == "no-cache"){
	// 	g_parse->status = 200;
	// }
	// else if (g_parse->cache_type.empty() && (g_parse->file_type == "html"
	// 	|| root.find(".html") != string::npos)){
	// 	g_parse->status = 304;
	// }
	if (locs.find("return_301") != locs.end()) {
		g_parse->status = 301;
		g_parse->location = locs["return_301"];
		g_parse->path = multimap_value(server_it->_server_data, "page_200_ok");
		return true;
	}
	if (_is_cgi(g_parse, locs)) {

		if (!_handle_cgi(root, locs, g_parse)) {
			g_parse->status = 500;
			root = multimap_value(server_it->_server_data, "error_page_500");
		}
	}
	return false;
}

get_parse* Webserver::_get_method(req_t* req, v_servers& servers)
{
	get_parse* g_parse;
	v_servers::iterator  server_it;
	string root, header, tmp_path;
	map_strings locs;
	g_parse = new get_parse;

	_parse_header(g_parse, req);
	server_it = target_server(g_parse, servers);
	if(_find_location(g_parse, server_it, locs, root))
		return g_parse;
	_response_error(g_parse, root, server_it, locs);
	g_parse->path = root;
	return g_parse;
}