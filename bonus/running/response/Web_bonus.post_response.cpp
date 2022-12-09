/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Web_bonus.post_response.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:04:51 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 14:16:38 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"


void    Webserver::_response_cgi(h_response *response, post_parse *p_parse)
{
	if (!p_parse->buffer.empty()){ // cgi here
		response->buffer = p_parse->buffer;
		response->content_length = "Content-Length: " + _to_string(p_parse->buffer.length()) + "\r\n";
		response->content_type = "Content-Type: " + _mime_types["html"] + "; charset=UTF-8\r\n\r\n";
		if (response->content_type.empty())
			response->content_type = "Content-Type: text/html; charset=UTF-8\r\n\r\n";
	}
	else{ // not cgi
		response->content_length = _count_content_length(p_parse->path);
		response->content_type = "Content-Type: " + _mime_types[p_parse->file_type] + "; charset=UTF-8\r\n\r\n";
		if (response->content_type.empty())
			response->content_type = "Content-Type: text/html; charset=UTF-8\r\n\r\n";
	}
}

void    Webserver::_response_redirect(h_response *response, post_parse *p_parse)
{
		if (!p_parse->location.empty()){
				response->location = "Location: " + p_parse->location + "\r\n";
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


void Webserver::_post_response(h_request *request, h_response *response)
{
	post_parse *p_parse;

	p_parse = (post_parse*)request->parsed_method;
	response->root = p_parse->path;
    _response_cgi(response, p_parse);
	p_parse->path = parse_path(p_parse->path, p_parse);
    _response_redirect(response, p_parse);

}