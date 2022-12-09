/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Web_bonus.running.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 09:26:25 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/09 18:06:15 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common_bonus.hpp"
#include "../includes/webserver_bonus.hpp"

void Webserver::up(char **env)
{
    this->envp = env;
    pair<config_t::const_iterator, config_t::const_iterator> range = _server_data.equal_range("listen");
    int sock;
    // start by assuming host is localhost
    u_int32_t host = inet_addr("127.0.0.1");
    // if it's not localhost check for validity
    if (_server_data.find("host")->second != "localhost")
    {
        host = inet_addr(_server_data.find("host")->second.c_str());
        if (host == INADDR_NONE)
        {
            std::cerr << "\033[1;31mError:\033[0m Invalid host address" << std::endl;
            exit(1);
        }
    }

    // loop all listen values (ports)
    for (; range.first != range.second; ++range.first)
    {
        // create a socket for each port
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
            _error(errno);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = host;
        addr.sin_port = htons(atoi(range.first->second.c_str()));
        memset(&(addr.sin_zero), '\0', 8);
        // check if kernel is hanged in kernel if so reuse it
        int optval = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
            _error(errno);
        if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
        {
            // if bind fails because of address already in use. ignore it
            if (errno == EADDRINUSE)
                continue;
            _error(errno);
        }
        if (listen(sock, 10) < 0)
            _error(errno);
        _sockets.insert(sock);
    }
}

void Webserver::run(vector<Webserver> servers)
{
    int ret, num_servers;
    t_client client;
    vector<pollfd> fds;
    vector<t_client> clients;

    num_servers = _sockets.size();
    set<int>::iterator it = _sockets.begin();
    for (size_t i = 0; i < _sockets.size(); ++i, ++it) {
        pollfd fd;
        fd.fd = *it;
        fcntl(fd.fd, F_SETFL, O_NONBLOCK);
        fd.events = POLLIN | POLLOUT | POLLHUP;
        fds.push_back(fd);
        clients.push_back(client);
    }
    for (;;) {
        // poll the sockets
        ret = poll(fds.data(), fds.size(), -1);
        if (ret < 0) {
            if (errno == EINTR)
                continue;
            _error(errno);
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN && (int)i < num_servers) {
                _accept(clients, fds, i);
            }
            if ((fds[i].revents & POLLHUP) && (int)i >= num_servers) {
                close(fds[i].fd);
                fds.erase(fds.begin() + i);
                delete clients[i].req;
                clients.erase(clients.begin() + i);
                i--;
                continue;
            }
            if (fds[i].revents & POLLIN && (int)i >= num_servers) {
                if (socket_I(clients, fds, &i))
                    continue;
                _initialize_response(clients, fds, i, servers);
            }
            if (fds[i].revents & POLLOUT && (int)i >= num_servers) {
                string method = clients[i].req->status_line.substr(0, clients[i].req->status_line.find(" "));
                string path = clients[i].req->status_line.substr(clients[i].req->status_line.find(" ") + 1);
                path = path.substr(0, path.find(" "));
                if (path == "/game/") {
                    cout << "game" << endl;
                    if (!_valid_session(clients[i].req->headers, servers))
                        _handle_invalid_session(clients[i].fd);
                }
                else if (method == "POST") {
                    if (_cookie_path(clients[i].req->status_line))
                        _cookies(clients, i, servers);
                }
                socket_O(clients, fds, &i);

            }
        }
    }
}
