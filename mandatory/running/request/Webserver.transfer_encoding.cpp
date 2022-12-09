/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.transfer_encoding.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 01:50:56 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/07 15:46:30 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

bool Webserver::parse_transfer_encoding(vector<t_client> &clients, vector<pollfd> &fds, size_t *i)
{
	char buff[1024];
    if (clients[*i].done_body == false && clients[*i].chunked_type && clients[*i].done_header){ // ready for extracting body with chunked
        // ready for extracting body
        bzero(buff, 1024);
        if(clients[*i].done_body == false){
            clients[*i].read_n = recv(fds[*i].fd, buff, 1024, 0);
            if (clients[*i].read_n == 0 || clients[*i].read_n == -1){
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
        if (clients[*i].req_buffer.find("0\r\n\r\n") != string::npos){
            clients[*i].done_body = true;
            clients[*i].req->status_line = clients[*i].req_buffer.substr(0, clients[*i].req_buffer.find("\r\n"));
            clients[*i].req->headers = clients[*i].req_buffer.substr(clients[*i].req_buffer.find("\r\n") + 2, clients[*i].req_buffer.find("\r\n\r\n") - clients[*i].req_buffer.find("\r\n") - 2);
            clients[*i].req->body = clients[*i].req_buffer.substr(clients[*i].req_buffer.find("\r\n\r\n") + 4, clients[*i].req_buffer.length() - clients[*i].req_buffer.find("\r\n\r\n") - 4);
            clients[*i].request_done = true;
        }
    }
    else if (clients[*i].done_length == false && clients[*i].request_done == false){ // ready for extracting header
        clients[*i].req->status_line = clients[*i].req_buffer.substr(0, clients[*i].req_buffer.find("\r\n"));
        clients[*i].req->headers = clients[*i].req_buffer.substr(clients[*i].req_buffer.find("\r\n") + 2);
        clients[*i].req->headers = clients[*i].req->headers.substr(0, clients[*i].req->headers.find("\r\n\r\n"));
        clients[*i].req->body = "";
        clients[*i].request_done = true;
        
    }
    return false;
}