/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.post_request.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 01:07:38 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 19:06:48 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>

vector<Webserver>::iterator Webserver::target_server(post_parse* p_parse, v_servers& servers)
{
	vector<Webserver>::iterator it;
	if (p_parse->host == "localhost")
		p_parse->host.assign("127.0.0.1");
	for (it = servers.begin(); it != servers.end(); it++){
		for(config_t::iterator ptr = it->_server_data.begin(); ptr != it->_server_data.end(); ptr++) {
			if (ptr->second == "0.0.0.0")
				p_parse->host.assign("0.0.0.0");
			if (ptr->second == p_parse->host  || (p_parse->host == "0.0.0.0"))
				for(config_t::iterator ptr_2 = it->_server_data.begin(); ptr_2 != it->_server_data.end(); ptr_2++)
					if (ptr_2->second == p_parse->port)
						return it;
		}
	}
	return it;
}

void Webserver::_upload_data(string root, post_parse* p_parse)
{
	vector<post_body>::iterator it_body;
	fstream file;
	if (p_parse->part_type == "multipart") {
		if (p_parse->format_type == "form-data") {
			for (it_body = p_parse->body.begin(); it_body != p_parse->body.end(); it_body++) {
				if (!it_body->filename.empty()) {
					string name = root + "/" + it_body->filename;
					file.open(name.c_str(), fstream::out);
					file << it_body->value;
					file.close();
				}
			}
		}
	}
}

string Webserver::parse_path(string path, post_parse* p_parse)
{
	string tmp_path, type;
	vector<string> data = split(path, "/");
	for (vector<string>::iterator it = data.begin(); it != data.end(); it++)
	{
		if ((*it).find(":") != string::npos)
			continue;
		check_query(*it, p_parse);
		for (size_t i = 0; i < (*it).length(); i++)
			if ((*it)[i] == '.' && find_last_point((*it), i) && it + 1 == data.end())
			{
				p_parse->file_type = (*it).substr(i + 1, (*it).length() - i);
				p_parse->file_name = "/" + *it;
				// query_post_parsing(p_parse);
				return tmp_path.substr(0, tmp_path.length() - 1);
			}
		if (it == data.begin())
			tmp_path += "/" + *it + "/";
		else
			tmp_path += *it + "/";
	}
	if (tmp_path.empty())
		return tmp_path = "/";
	return "/" + tmp_path.substr(0, tmp_path.length() - 1);
}

bool Webserver::_find_location(post_parse *p_parse, v_servers::iterator &server_it, map_strings &locs, string &root)
{
	bool unsupported = false;
	p_parse->status = 200;

	if (server_it->_location_data.find(p_parse->path) != server_it->_location_data.end()) {
		locs = server_it->_location_data[p_parse->path];
	}
	else if (server_it->_location_data.find("/") != server_it->_location_data.end()) {
		locs = server_it->_location_data["/"];
	}
	else if (server_it->_server_data.find("root") != server_it->_server_data.end()) {
		locs.insert(make_pair("root", multimap_value(server_it->_server_data, "root")));
	}
	if (locs.find("allow")->second.find("POST") != string::npos) {
		if (!p_parse->file_name.empty())
			root += locs["root"] + p_parse->path + "/" + p_parse->file_name;
		else
			root += locs["root"] + p_parse->path + "/" + locs["index"];
	}
	else {
		unsupported = true;
		p_parse->status = 405;
		root = multimap_value(server_it->_server_data, "error_page_405");
	}
	if (locs.find("return_301") != locs.end()){
		unsupported = true;
		p_parse->status = 301;
		p_parse->location = locs["return_301"];
		p_parse->path = multimap_value(server_it->_server_data, "page_200_ok");
		return true;
	}

	if (_entity_too_large(server_it->_server_data, p_parse) && p_parse->status == 200) {
		unsupported = true;
		p_parse->status = 413;
		root = multimap_value(server_it->_server_data, "error_page_413");
	}
	else if (locs.find("allow_upload")->second.find("on") != string::npos 
			&& locs.find("allow")->second.find("POST") != string::npos) {
				_upload_data(locs["upload_at"], p_parse);
				unsupported = true;
				p_parse->status = 201;
			}
			
	if (_is_cgi(p_parse, locs)) {
		cout << "CGI" << endl;
		if (!_handle_cgi(root, locs, p_parse)) {
			p_parse->status = 500;
			root = multimap_value(server_it->_server_data, "error_page_500");
		}
		unsupported = true;
	}
	if (unsupported == false){
		p_parse->status = 415;
		p_parse->buffer = "<html><body><h1>415 Unsupported Media Type</h1></body></html>";
	}
	return false;
}


void Webserver::_process_post_request(post_parse* p_parse, v_servers& servers)
{
	v_servers::iterator  server_it;
	string root;
	map_strings locs;


	p_parse->path = parse_path(p_parse->path, p_parse); // path funtion
	replace_all(p_parse->path, "//", "/");
	server_it = target_server(p_parse, servers);
	if (_find_location(p_parse, server_it, locs, root))
		return ;
	_response_error(p_parse, root, server_it);
	p_parse->path = root;
}