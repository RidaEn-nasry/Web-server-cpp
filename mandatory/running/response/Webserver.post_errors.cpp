/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.post_errors.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 16:44:45 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/06 15:20:17 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

bool Webserver::_entity_too_large(config server_data, post_parse *p_parse)
{	
	size_t max_upload;
	max_upload = atol(multimap_value(server_data, "client_max_body_size").c_str()) * 1024;
	for (vector<post_body>::iterator it_body = p_parse->body.begin();  it_body != p_parse->body.end(); it_body++){
		if (it_body->value.length() / 1024 > max_upload)
			return true;
	}
	return false;
}

h_response *Webserver::request_error(t_client *client)
{
	string body, header;
	h_response *response = new h_response();
	if (client->err_type == "400"){

		header = "HTTP/1.1 400 Bad Request\r\nConnection: Keep-Alive; timeout=15, max=100\r\nContent-Type: text/html\r\nServer: boostcp/1.2.0 (YumOS)\r\nContent-Length: ";
		body = "<html><head><title>400 Bad Request</title></head><body><center><h1>400 Bad Request</h1></center><hr><center>nginx</center></body></html>";
		header += _to_string(body.length()) + "\r\n\r";
		response->http_header = header + body;
		return response;
	}
	return response;
}