/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.response.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 16:01:21 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 14:28:07 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>


void Webserver::socket_O(vector<t_client> &clients, vector<pollfd> &fds, size_t *i)
{
    if (clients[*i].request_done){
        clients[*i].send_n = send(clients[*i].fd, clients[*i].res_buffer.c_str() + clients[*i].total, clients[*i].bytesleft, 0);
         if (clients[*i].send_n == -1 || clients[*i].send_n == 0){
            close(fds[*i].fd);
            fds.erase(fds.begin() + *i);
			delete clients[*i].req;
            clients.erase(clients.begin() + *i);
            *i -= 1;
         }
         else{
            clients[*i].total += clients[*i].send_n;
            clients[*i].bytesleft -= clients[*i].send_n;
         }
         if (clients[*i].total == clients[*i].res_buffer_len){
            close(fds[*i].fd);
            fds.erase(fds.begin() + *i);
			delete clients[*i].req;
            clients.erase(clients.begin() + *i);
            *i -= 1;
		 }
    }
}

h_response* Webserver::_make_response(req_t* req, v_servers& servers)
{
	h_response* response = NULL;
	get_parse* g_parse;
	post_parse* p_parse;
	delete_parse* d_parse;

	if (_request->method == GET) { // GET METHOD
		_request->parsed_method = (void*)_get_method(req, servers); // parse get header and check for root
		g_parse = (get_parse*)_request->parsed_method;
		response = _initialize_header(_request, g_parse->status);
		response->request = _request;
		response->g_parse = g_parse;
	}
	else if (_request->method == POST) { // POST METHOD
		_request->parsed_method = (void*)_post_method(req);
		p_parse = (post_parse*)_request->parsed_method;
		_process_post_request(p_parse, servers);
		response = _initialize_header(_request, p_parse->status);
		response->request = _request;
		response->p_parse = p_parse;
	}
	else if (_request->method == DELETE) { // DELETE METHOD
		_request->parsed_method = (void*)_delete_method(req, servers);
		d_parse = (delete_parse*)_request->parsed_method;
		_proccess_delete_request(d_parse, servers);
		response = _initialize_header(_request, d_parse->status);
		response->request = _request;
		response->d_parse = d_parse;
	}
	else if (_request->method == UNDEFINED){
		response = new h_response;
		response->http_header = "HTTP/1.1 405 Method Not Allowed\r\ncontent-type: text/html\r\n\r\n";
		response->buffer = "<html><head><title>405 Method Not Allowed</title></head><body><h1>405 Method Not Allowed</h1></body></html>";
	}
	return response;

}

