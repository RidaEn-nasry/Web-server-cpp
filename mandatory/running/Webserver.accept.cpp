/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.accept.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:07:36 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/07 15:23:32 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

void Webserver::_accept(vector<t_client> &clients, vector<pollfd> &fds, size_t i)
{
	t_client client;
    int client_fd = 0;
    if ((client_fd = accept(fds[i].fd, NULL, NULL)) < 0)
        throw AcceptException();
    pollfd fd;
    fd.fd = client_fd;
    fcntl(fd.fd, F_SETFL, O_NONBLOCK);
    fd.events = POLLIN | POLLOUT;
    fds.push_back(fd);
    client.fd = fd.fd;
    client.done_header = false;
    client.length_type = false;
    client.done_body = false;
    client.chunked_type = false;
    client.done_length = false;
    client.request_done = false;
    client.error = false;
    client.req = new req_t();
    clients.push_back(client);
}