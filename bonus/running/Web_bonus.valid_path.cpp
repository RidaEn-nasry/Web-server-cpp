/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.valid_path.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 22:43:23 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/08 16:44:12 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common_bonus.hpp"
#include "../includes/webserver_bonus.hpp"


bool Webserver::valid_path(string status_line, pollfd client_fd)
{
	(void)client_fd;
	v_str_t			str, tmp;
	v_servers::iterator  server_it;
	string v_tmp, method;
	map_strings locs;
	str = split(status_line, "\n");
	// fill get_parse struct from buffer
	for (v_str_t::iterator str_it = str.begin(); str_it != str.end(); str_it++) {
		if ((*str_it).find("HTTP/") != string::npos) {
			tmp = split((*str_it), " ");
			method = trim(tmp[0], " \r\n");
			v_tmp = trim(tmp[1], " \r\n");
			tmp.clear();
		}	
	}
	str.clear();
	for (size_t i = 0; i < v_tmp.length(); i++)
		if (v_tmp[i] == '.' || v_tmp[i] == '?')
			return false;
	if (method == "GET"){
		if (v_tmp[v_tmp.length() - 1] != '/') {
			_response = new h_response;
			_response->buffer = "Moved";
			_response->http_header = "HTTP/1.1 301 Moved Permanently\r\nCache-control: no-cache\r\nLocation: " + v_tmp + "/\r\n\r\n";
			_send_response(client_fd);
			return true;
			
		}
	}
	return false;
}