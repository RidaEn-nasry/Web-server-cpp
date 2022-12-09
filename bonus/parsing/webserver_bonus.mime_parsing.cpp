/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver_bonus.mime_parsing.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 18:54:12 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/08 16:44:47 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common_bonus.hpp"
#include "../includes/webserver_bonus.hpp"

 map<string, string> Webserver::mime_parsing(config_t _server_data)
 {
	mime_t mime_types;
	fstream file;
	config_it it;
	string line, tmp;
    regex_t regex;
    regmatch_t index;
	vector<string> data;
	size_t _key_len, _value_len;
	int ret;
	for(it = _server_data.begin(); it != _server_data.end(); it++)
		if (it->first == "include")
			break;
	file.open(it->second.c_str(), fstream::in);
	if (!file)
		return mime_types;
	while(getline(file, tmp))
		line += " " + tmp;
	file.close();
    regcomp(&regex, "\\{.*\\}", REG_EXTENDED);
    ret = regexec(&regex, line.c_str(), 1, &index, 0);
	if(ret == 0)
	{
		tmp = trim(line.substr(index.rm_so, index.rm_eo - index.rm_so), "{} \t\v");
		data = split(tmp, ";");
		for(vector<string>::iterator it = data.begin(); it != data.end(); it++)
		{
			*it = trim((*it), " \t\v\f");
            _key_len = key_len(*it);
            _value_len = value_len(*it, _key_len);
			mime_types.insert(make_pair((*it).substr(0, _key_len), (*it).substr(_value_len, (*it).length())));
		}
		data.clear();
		regfree(&regex);
	}
	return mime_types;
 }