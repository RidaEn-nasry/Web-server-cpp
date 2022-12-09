/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Web_bonus.send.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 19:31:27 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 14:21:19 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"

string Webserver::_count_content_length(string root)
{
	fstream file(root.c_str(), fstream::in);
	string str;
	stringstream buffer, len;
	buffer << file.rdbuf();
	file.close();
	len << buffer.str().length();
	return "Content-Length: " + len.str() + "\r\n";
}

int Webserver::_send(int socket, const char *buf, size_t len)
{
    size_t total = 0;
    int bytesleft = len;
    int n;
    while(total < len) {
        n = send(socket, buf + total, bytesleft, 0);
        if (n == -1){
			cout << "-1 " << endl;
            return 0;
		}
		total += n;
        bytesleft -= n;
    }
    return n == -1 ? -1 : 0;
}


h_response    *Webserver::_status_line(int code_type)
{
	h_response *response = new h_response;
	if (code_type == 200)
		response->code_status = "HTTP/1.1 200 OK\r\n";
	if (code_type == 201)
		response->code_status = "HTTP/1.1 201 Created\r\n";
	if (code_type == 204)
		response->code_status = "HTTP/1.1 204 No Content\r\n";
	if (code_type == 301)
		response->code_status = "HTTP/1.1 301 Moved Permanently\r\n";
	if (code_type == 302)
		response->code_status = "HTTP/1.1 302 Found\r\n";
	if (code_type == 304)
		response->code_status = "HTTP/1.1 304 OK\r\n";
	if (code_type == 403)
		response->code_status = "HTTP/1.1 403 Forbidden\r\n";
	if (code_type == 404)
		response->code_status = "HTTP/1.1 404 Not Found\r\n";
	if (code_type == 405)
		response->code_status = "HTTP/1.1 405 Method Not Allowed\r\n";
	if (code_type == 413)
		response->code_status = "HTTP/1.1 413 Payload Too Large\r\n";
	if (code_type == 415)
		response->code_status = "HTTP/1.1 415 Unsupported Media Type\r\n";
	if (code_type == 500)
		response->code_status = "HTTP/1.1 500 Internal Server Error\r\n";

	response->keep_connection = "Connection: Keep-Alive\r\nKeep-Alive: timeout=15, max=100\r\n";
	response->date = "Date: Mon, 21 Nov 2022 12:36:23 GMT\r\n";
	response->server_type = "Server: boostcp/1.2.0 (YumOS)\r\n";
    return response;
}

h_response *Webserver::_initialize_header(h_request *request, int code_type)
{
    h_response *response;

    response = _status_line(code_type);
	if(request->method == GET)
        _get_response(request, response);
	if(request->method == POST)
        _post_response(request, response);
	if (request->method == DELETE)
        _delete_response(request, response);
	return (response);
		
}

int Webserver::_send_response(pollfd client_fd)
{
	stringstream buff;
    fstream file;

	
    if (!_response->buffer.empty()){
        _response->http_header += _response->buffer;
		_response->buffer.clear();
    }
    else{
        
	    file.open(_response->root.c_str(), fstream::in);
	    if (file){
	    	buff << file.rdbuf();
	    	_response->http_header += buff.str();
			file.close();
	    }
    }
	(void)client_fd; 
	return 0;
}