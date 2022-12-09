/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.parse_utils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 16:03:11 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/08 16:46:08 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"

string _to_string(long long num) {
	string str;
	if (num == 0)
		return "0";
	while (num > 0) {
		str += (num % 10) + '0';
		num /= 10;
	}
	reverse(str.begin(), str.end());
	return str;
}

string Webserver::multimap_value(multimap<string, string> data, string key)
{
    for(multimap<string, string>::iterator it = data.begin(); it != data.end(); it++){
        if (it->first == key)
            return it->second;
    }
    return "";
}

void Webserver::check_query(string& str, get_parse* g_parse)
{
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == '?') {
			g_parse->query = str.substr(i + 1, str.length() - (i + 1));
			str = str.substr(0, i);
		}
	}
}

void	query_get_parsing(get_parse* g_req)
{
	vector<string> data;
	for (size_t i = 0; i < g_req->file_name.length(); i++) {
		if (g_req->file_name[i] == '?') {
			data = split(g_req->file_name, "?");
			g_req->file_name = data[0];
			g_req->query = data[1];
		}
	}
}

int find_last_point(string str, size_t i)
{
	for (size_t j = i + 1; j < str.length(); j++) {
		if (str[j] == '.')
			return 0;
	}
	return 1;
}

string Webserver::fix_path(string str)
{
	string line;
	replace_all(str, "%20", " ");
	replace_all(str, "//", "/");
	vector<string> data = split(str, "/");
	for (vector<string>::iterator it = data.begin(); it != data.end(); it++) {
		if (*it == data[0] && it + 1 != data.end() && (*it).find(".") != string::npos) {
			data.erase(it);
			it = data.begin();
		}
		line += "/" + *it + "/";
	}
	return line;
}

void Webserver::check_query(string &str, post_parse *p_parse)
{

	for (size_t i = 0; i < str.length(); i++){
		if (str[i] == '?'){
			p_parse->query = str.substr(i + 1 , str.length() - (i + 1));
		}
	}
}

void	query_post_parsing(post_parse* p_parse)
{
	vector<string> data;
	for (size_t i = 0; i < p_parse->file_name.length(); i++) {
		if (p_parse->file_name[i] == '?') {
			data = split(p_parse->file_name, "?");
			p_parse->file_name = data[0];
			p_parse->query = data[1];
		}
	}
}

void Webserver::check_query(string &str, delete_parse *d_parse)
{

	for (size_t i = 0; i < str.length(); i++){
		if (str[i] == '?'){
			d_parse->query = str.substr(i + 1 , str.length() - (i + 1));
		}
	}
}

void	Webserver::query_parsing(delete_parse *d_parse)
{
	vector<string> data;
	for (size_t i = 0; i < d_parse->file_name.length(); i++){
		if (d_parse->file_name[i] == '?'){
			data = split(d_parse->file_name, "?");
			d_parse->file_name = data[0];
			d_parse->query = data[1];
		}
	}
}
