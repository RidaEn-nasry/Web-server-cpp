/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Web_bonus.parse_method.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 19:09:57 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 16:16:35 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"


h_request* Webserver::_make_request(req_t* req)
{
	h_request* header;
	v_str_t		str, tmp;

	header = new h_request;

	str = split(req->status_line, "\n");

	v_str_t::iterator str_it = str.begin();
	if ((*str_it).find("HTTP/") != string::npos) {
		tmp = split((*str_it), " ");
		if (tmp[0] == "GET")
			header->method = GET;
		else if (tmp[0] == "POST")
			header->method = POST;
		else if (tmp[0] == "DELETE")
			header->method = DELETE;
		else
			header->method = UNDEFINED;
	}
	tmp.clear();
	str.clear();
	return header;
}


void Webserver::_initialize_response(vector<t_client> &clients, vector<pollfd> &fds, size_t i, vector<Webserver> &servers)
{
    if (clients[i].request_done) { // done request reading
		// add_garbage();	
		if (valid_path(clients[i].req->status_line, fds[i])){}
		else if (!clients[i].err_type.empty()){
			_response = request_error(&clients[i]);
		}
		else {
    		_request = _make_request(clients[i].req);
    		_response = _make_response(clients[i].req, servers);
    		_send_response(fds[i]);
		}
    	clients[i].res_buffer = _response->http_header;
    	clients[i].res_buffer_len = _response->http_header.length();
    	clients[i].send_n = 0;
    	clients[i].bytesleft = _response->http_header.length();
    	 clients[i].total = 0;
    }

}
