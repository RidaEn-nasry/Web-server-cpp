/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.content_length.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 01:44:23 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/07 15:39:58 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>

bool Webserver::parse_content_length(vector<t_client> &clients, vector<pollfd> &fds, size_t *i)
{
	char buff[1024];
	v_str_t lines;
    if (clients[*i].done_body == false && clients[*i].length_type && clients[*i].done_header){ // ready for extracting body with content length
        // ready for extracting body
        if(clients[*i].done_body == false){
            bzero(buff, 1024);
            clients[*i].read_n = recv(clients[*i].fd, buff, 1024, 0);
            if ((clients[*i].read_n == 0 || clients[*i].read_n == -1) && clients[*i].req_buffer.length() != clients[*i].total){
                close(fds[*i].fd);
                fds.erase(fds.begin() + *i);
                clients.erase(clients.begin() + *i);
                *i -= 1;
                return true;
            }
            if(clients[*i].read_n != 0 && clients[*i].read_n != -1){
                clients[*i].req_buffer += string(buff, clients[*i].read_n);
                clients[*i].total += clients[*i].read_n;
            }
        }
        if (clients[*i].total == (clients[*i].content_length + clients[*i].header_length)){ // split the header and body
            clients[*i].done_body = true;
            clients[*i].req->status_line = clients[*i].req_buffer.substr(0, clients[*i].req_buffer.find("\r\n"));
            clients[*i].req->headers = clients[*i].req_buffer.substr(clients[*i].req_buffer.find("\r\n") + 2, clients[*i].req_buffer.find("\r\n\r\n") - clients[*i].req_buffer.find("\r\n") - 2);
            clients[*i].req->body = clients[*i].req_buffer.substr(clients[*i].req_buffer.find("\r\n\r\n") + 4, clients[*i].req_buffer.length() - clients[*i].req_buffer.find("\r\n\r\n") - 4);
            clients[*i].request_done = true;
        }
    }
    return false;
}