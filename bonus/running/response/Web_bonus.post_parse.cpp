/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.post_parse.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 16:12:04 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/08 16:46:07 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"

string find_body_header(string str, string &file_header)
{
	string body_header;
	size_t i;
	
	if (str.find("Content-Disposition:") != string::npos){
		if (str.find("Content-Type:") != string::npos){
			i = str.find("Content-Type:");
		for(; i < str.length(); i++){
			if (str[i] == '\n'){
				file_header = str.substr(0, i);
				return str.substr(i + 3 , str.length() - (i + 6));
			}
			}
		}
		else{
			i = str.find("Content-Disposition:");
			for(; i < str.length(); i++){
				if (str[i] == '\n'){
					file_header = str.substr(0, i);
					return str.substr(i + 3 , str.length() - (i + 5));
				}
			}
		}
	}

	return str;
}

vector<post_body> Webserver::_parse_raw(string line, string type)
{
	post_body p_body;
	vector<post_body> v_body;
	
	p_body.value = line;
	p_body.content_type = type;
	v_body.push_back(p_body);
	return v_body;
}

vector<post_body> Webserver::_parse_urlencoded(string line)
{
	post_body p_body;
	v_str_t tmp, tmp2;
	vector<post_body> v_body;
	
	p_body.format_type = "x-www-form-urlencoded";
	tmp = split(line, "&");
	for (v_str_t::iterator it_body = tmp.begin(); it_body != tmp.end() - 1; it_body++){
		tmp2 = split(*it_body, "=");
		p_body.key = trim(tmp2[0], "\r\n");
		p_body.value = trim(tmp2[1], "\r\n");
		p_body.content_type = "form-urlencoded";
		v_body.push_back(p_body);
		tmp2.clear();
	}
	tmp.clear();
	return v_body;
}

post_body Webserver::_parse_body(string line)
{
	post_body p_body;
	v_str_t tmp, tmp2;
	fstream file;
	string file_header;
	p_body.value = find_body_header(line, file_header);
	tmp = split(file_header, "\r");
	for (v_str_t::iterator it_body = tmp.begin(); it_body != tmp.end(); it_body++){
		if ((*it_body).find("Content-Disposition:") != string::npos){
			tmp2 = split(*it_body, ";");
			p_body.format_type = trim(tmp2[0].substr(20, tmp2[0].length() - 20), " ;\r\n");
			if ((*it_body).find("name=") != string::npos)
				p_body.key = trim(tmp2[1].substr(5, tmp2[1].length() - 5), "\";= \r\n");
			if ((*it_body).find("filename=") != string::npos)
				p_body.filename = trim(tmp2[2].substr(9, tmp2[2].length() - 9), "\";= \r\n");
			tmp2.clear();
		}
		else if ((*it_body).find("Content-Type:") != string::npos){
			tmp2 = split(*it_body, " ");
			p_body.content_type = trim(tmp2[1], " \r\n");
		}
	}
	return p_body;
}

void Webserver::_parse_header(post_parse *p_parse, req_t* req)

{
	v_str_t	str, tmp, tmp2;
	string	header;
	p_parse->status = 0;

	header = req->status_line + "\r\n" + req->headers;
	str = split(header, "\n");
	for (v_str_t::iterator str_it = str.begin(); str_it != str.end(); str_it++) {
		if ((*str_it).find("HTTP/") != string::npos) {
			tmp = split((*str_it), " ");
			p_parse->http_v = trim(tmp[2].substr(5, tmp[2].length() - 3).c_str(), " \n\r\f\t");
			p_parse->path = fix_path(tmp[1]);
			tmp.clear();
		}
		if ((*str_it).find("Host:") != string::npos){
			tmp = split((*str_it), ":");
			p_parse->host = trim(tmp[1], " \n\r\f\t");
			p_parse->port = trim(tmp[2], " \n\r\f\t");
			tmp.clear();
		}
        if ((*str_it).find("Cache-Control:") != string::npos){
			tmp = split((*str_it), ":");
			p_parse->cache_type = trim(tmp[1], " \n\r\f\t");
			tmp.clear();
		}
		if ((*str_it).find("Content-Length:") != string::npos){
			tmp = split((*str_it), " ");
			p_parse->content_length = trim(tmp[1], " \r\n");
			tmp.clear();
		}
		if ((*str_it).find("Content-Type:") != string::npos){
			if ((*str_it).find("multipart") != string::npos){
				tmp = split((*str_it), " ");
				tmp2 = split(tmp[1], "/");
				p_parse->part_type = tmp2[0];
				p_parse->format_type = trim(tmp2[1], ";\r\n");
				if ((*str_it).find("boundary") != string::npos)
					p_parse->boundary = "--" + tmp[2].substr(9, trim(tmp[2], "\r\n").length() - 9);
				tmp.clear(); tmp2.clear();
			}
			else if ((*str_it).find("application") != string::npos){
				tmp = split((*str_it), " ");
				tmp2 = split(tmp[1], "/");
				p_parse->part_type = tmp2[0];
				p_parse->format_type = trim(tmp2[1], ";\r\n");
			}
			else { // raw
				tmp = split((*str_it), " ");
				p_parse->part_type = trim(tmp[1], "\r\n");
				p_parse->format_type = "raw";
			}
		}
	}
}

post_parse *Webserver::_post_method(req_t* req)
{
	v_str_t	tmp;
	post_parse	*p_parse;
	v_servers::iterator  server_it;
	string root, tmp_path;
	map_strings locs;

	p_parse = new post_parse;
	_parse_header(p_parse, req);

	if (!p_parse->boundary.empty()){
	tmp = split(req->body, p_parse->boundary);
	for (v_str_t::iterator it_body = tmp.begin(); it_body != tmp.end() - 1; it_body++){
		p_parse->body.push_back(_parse_body((*it_body)));
		}
	}
	else if (p_parse->part_type == "application"){
		if (p_parse->format_type == "x-www-form-urlencoded"){
			p_parse->body = _parse_urlencoded(req->body);
		}
	}
	else if (p_parse->format_type == "binary_raw") { // raw
		p_parse->body = _parse_raw(req->body, p_parse->part_type);
	}
	return p_parse;
}