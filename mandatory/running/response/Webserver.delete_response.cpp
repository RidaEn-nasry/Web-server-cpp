/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.delete_response.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:16:44 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 15:12:36 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>


void    Webserver::_response_cgi(h_response *response, delete_parse *d_parse)
{
	if (!d_parse->buffer.empty()){ // cgi here
		response->buffer = d_parse->buffer;
		response->content_length = "Content-Length: " + _to_string(d_parse->buffer.length()) + "\r\n";
		response->content_type = "Content-Type: " + _mime_types["html"] + "; charset=UTF-8\r\n\r\n";
		if (response->content_type.empty())
			response->content_type = "Content-Type: text/html; charset=UTF-8\r\n\r\n";	
	}
	else{ // not cgi
		response->content_length = _count_content_length(d_parse->path);
		response->content_type = "Content-Type: " + _mime_types[d_parse->file_type] + "; charset=UTF-8\r\n\r\n";
		if (response->content_type.empty())
			response->content_type = "Content-Type: text/html; charset=UTF-8\r\n\r\n";
	}
}

void    Webserver::_response_redirect(h_response *response, delete_parse *d_parse)
{
		if (!d_parse->location.empty()){
				response->location = "Location: " + d_parse->location + "\r\n";
				response->cache = "Cache-control: no-cache\r\n";
				response->http_header = response->code_status + response->keep_connection
				+ response->date + response->cache + response->location + response->server_type
					+ response->content_length + response->content_type;
		}
		else{
			response->http_header = response->code_status + response->keep_connection
				+ response->date + response->server_type + response->content_length + response->content_type;
		}
}

void Webserver::_delete_response(h_request *request, h_response *response)
{
	delete_parse *d_parse;

	d_parse = (delete_parse*)request->parsed_method;
	response->root = d_parse->path;
    _response_cgi(response, d_parse);
	d_parse->path = parse_path(d_parse->path, d_parse);
    _response_redirect(response, d_parse);
}