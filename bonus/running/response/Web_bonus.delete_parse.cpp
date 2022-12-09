/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.delete_parse.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 01:16:06 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/08 16:46:07 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"


string Webserver::parse_path(string path, delete_parse *d_parse)
{
    string tmp_path, type;
    vector<string> data = split(path, "/");
    for(vector<string>::iterator it = data.begin(); it != data.end(); it++)
    {
		check_query(*it, d_parse);
        for(size_t i = 0; i < (*it).length(); i++)
            if ((*it)[i] == '.' && find_last_point((*it), i) && it + 1 == data.end())
            {
				d_parse->file_type = (*it).substr(i + 1, (*it).length() - i);
                d_parse->file_name = "/" + *it;
				query_parsing(d_parse);
                return tmp_path.substr(0, tmp_path.length() - 1);
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

vector<Webserver>::iterator Webserver::target_server(delete_parse *d_parse, v_servers &servers)
{
	vector<Webserver>::iterator it;
	if (d_parse->host == "localhost")
		d_parse->host.assign("127.0.0.1");
	for (it = servers.begin(); it != servers.end(); it++){
		for(config_t::iterator ptr = it->_server_data.begin(); ptr != it->_server_data.end(); ptr++) {
			if (ptr->second == "0.0.0.0")
				d_parse->host.assign("0.0.0.0");
			if (ptr->second == d_parse->host  || (d_parse->host == "0.0.0.0"))
				for(config_t::iterator ptr_2 = it->_server_data.begin(); ptr_2 != it->_server_data.end(); ptr_2++)
					if (ptr_2->second == d_parse->port)
						return it;
		}
	}
	return it;
}

void Webserver::_parse_header(delete_parse *d_parse, req_t* req)
{
	v_str_t	str, tmp;
	string  header;

	d_parse->status = 200;
	
	header = req->status_line + "\r\n" + req->headers;
	str = split(header, "\n");
	for (v_str_t::iterator str_it = str.begin(); str_it != str.end(); str_it++) {
		if ((*str_it).find("HTTP/") != string::npos) {
			tmp = split((*str_it), " ");
			d_parse->http_v = trim(tmp[2].substr(5, tmp[2].length() - 3).c_str(), " \n\r\f\t");
			d_parse->path = fix_path(tmp[1]);
			tmp.clear();
		}
		if ((*str_it).find("Host:") != string::npos){
			tmp = split((*str_it), ":");
			d_parse->host = trim(tmp[1], " \n\r\f\t");
			d_parse->port = trim(tmp[2], " \n\r\f\t");
			tmp.clear();
		}
        if ((*str_it).find("Cache-Control:") != string::npos){
			tmp = split((*str_it), ":");
			d_parse->cache_type = trim(tmp[1], " \n\r\f\t");
			tmp.clear();
		}
		if ((*str_it).find("Content-Length:") != string::npos){
			tmp = split((*str_it), " ");
			d_parse->content_length = atol(trim(tmp[1], " \n\r\f\t").c_str());
			tmp.clear();
		}
		if ((*str_it).find("Content-Type:") != string::npos){
			d_parse->content_type = *str_it;
			break;
		}
	}
}

bool Webserver::_find_location(delete_parse *d_parse, v_servers::iterator &server_it, map_strings &locs, string &root)
{
	if(server_it->_location_data.find(d_parse->path) != server_it->_location_data.end()){
		locs = server_it->_location_data[d_parse->path];
	}
	else if (server_it->_location_data.find("/") != server_it->_location_data.end()){
		locs = server_it->_location_data["/"];
	}
	else if (server_it->_server_data.find("root") != server_it->_server_data.end()){
		locs.insert(make_pair("root", multimap_value(server_it->_server_data, "root")));
	}
	if (d_parse->file_name.empty() && locs.find("allow")->second.find("DELETE") != string::npos){
			root += locs["root"] + d_parse->path + "/" + locs["index"];
	}
	else if (locs.find("allow")->second.find("DELETE") != string::npos){
 		root += locs["root"] + d_parse->path + d_parse->file_name;
	}
	else{
		d_parse->status = 405;
		root = multimap_value(server_it->_server_data, "error_page_405");
	}
	
	if (locs.find("return_301") != locs.end()){
		d_parse->status = 301;
		d_parse->location = locs["return_301"];
		d_parse->path = multimap_value(server_it->_server_data, "page_200_ok");
		return true;
	}
	return false;
}

delete_parse *Webserver::_delete_method(req_t* req, v_servers &servers)
{
	v_str_t			str, tmp;
	delete_parse		*d_parse;
	v_servers::iterator  server_it;
	string root, header, tmp_path;
	map_strings locs;
	
	d_parse = new delete_parse;	
	_parse_header(d_parse, req);
	d_parse->path = parse_path(d_parse->path, d_parse); // path funtion
	server_it = target_server(d_parse, servers);
	if(_find_location(d_parse, server_it, locs, root))
		return d_parse;
	tmp_path = parse_path(root, d_parse);
	_response_error(d_parse, root, server_it);
	d_parse->path = root;
	return d_parse;
}


void Webserver::_proccess_delete_request(delete_parse *d_parse, v_servers &servers)
{
	v_servers::iterator server_it = target_server(d_parse, servers);
	if (d_parse->status == 200){
		remove(d_parse->path.c_str());
		d_parse->path = multimap_value(server_it->_server_data, "page_200_delete");
	}
	else if (d_parse->status == 204){
		d_parse->path = multimap_value(server_it->_server_data, "page_204_no_content");
	}
}