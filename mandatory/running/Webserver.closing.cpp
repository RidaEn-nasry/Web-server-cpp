/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.closing.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:19:52 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/05 01:00:10 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>

// void    Webserver::_close_client(int client_fd) {
//     // std::cout << "Closing client" << std::endl;
//     close(client_fd);
//     _sockets.erase(client_fd);
// }

// void Webserver::_check_closed(vector<pollfd>& fds, size_t* i, bool* closed) {
//     if (*closed) {
//         _close_client(fds[*i].fd);
//         fds.erase(fds.begin() + *i);
//         *i -= 1;
//         closed = 0;
//     }
// }