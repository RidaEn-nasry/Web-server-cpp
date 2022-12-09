/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.request.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 01:28:16 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/08 16:46:29 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"

bool Webserver::parse_header(vector<t_client> &clients, vector<pollfd> &fds, size_t *i)
{
	char buff[1024];
	v_str_t lines, tmp;
	if(clients[*i].done_header == false){
        bzero(buff, 1024);
        clients[*i].read_n = recv(clients[*i].fd, buff, 1024, 0);
        if (clients[*i].read_n == 0 || clients[*i].read_n == -1){
            close(fds[*i].fd);
            fds.erase(fds.begin() + *i);
            delete clients[*i].req;
            clients.erase(clients.begin() + *i);
            *i -= 1;
            return true;
        }
        clients[*i].req_buffer += string(buff, clients[*i].read_n);
    }
    if (clients[*i].req_buffer.find("\r\n\r\n") != string::npos && clients[*i].done_length == false){
        clients[*i].done_header = true;
        if (clients[*i].req_buffer.find("Host:") == string::npos){
            clients[*i].error = true;
        }
        string status_line = clients[*i].req_buffer.substr(0, clients[*i].req_buffer.find("\r\n"));
        string method = status_line.substr(0, status_line.find(" "));
        clients[*i].total = clients[*i].req_buffer.length();
        
        if (clients[*i].done_length == false && clients[*i].done_header){ // looking for content length or transfer encoding
            
            lines = split(clients[*i].req_buffer, "\n");
            for (v_str_t::iterator it = lines.begin(); it != lines.end(); ++it) {
                if ((*it).find("Content-Length:") != string::npos && (method != "GET" && method != "DELETE")){
                    tmp = split((*it), " ");
                    clients[*i].content_length = atol(trim(tmp[1], " \r\n").c_str());
                    if (clients[*i].content_length < 0){   
                        clients[*i].error = true;
                    }
                    tmp.clear();
                    lines.clear();
                    clients[*i].length_type = true;
                    clients[*i].done_length = true;
                    string tmp_header = clients[*i].req_buffer.substr(0, clients[*i].req_buffer.find("\r\n\r\n") + 4);
                    clients[*i].header_length = tmp_header.length();
                    break;
                }
                else if ((*it).find("Transfer-Encoding:") != string::npos && (method != "GET" && method != "DELETE")){
                    tmp = split((*it), " ");
                    if (trim(tmp[1], " \r\n") == "chunked"){
                        clients[*i].done_length = true;
                        clients[*i].chunked_type = true;
                        string tmp_header = clients[*i].req_buffer.substr(0, clients[*i].req_buffer.find("\r\n\r\n") + 4);
                        clients[*i].header_length = tmp_header.length();
                        break;
                    }
                    else{
                        clients[*i].error = true;
                        break;
                    }
                }
            }
        }
    }
    return false;
}

bool Webserver::socket_I(vector<t_client> &clients, vector<pollfd> &fds, size_t *i)
{
	v_str_t lines, tmp;

	if(parse_header(clients, fds, i))
        return true;
	if (parse_content_length(clients, fds, i))
		return true;
	if (parse_transfer_encoding(clients, fds, i))
		return true;
    if (clients[*i].error){
        
        clients[*i].err_type = "400"; 
    }
	return false;
}