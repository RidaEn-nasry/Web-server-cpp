/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.error.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:50:17 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/06 01:27:37 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>


void Webserver::_error(int errorno) {
    cout << "\033[1;31mError:\033[0m " << strerror(errorno) << endl;
    exit(1);
}


void Webserver::_set_error_code(string status_code, const string& msg, const string& path) {
    _response->code_status = status_code;
    _response->msg_status = msg;
    if (!path.empty())
        _response->err_path = path;
}

int Webserver::error_page(string path)
{
    DIR* dir;
    string error_dir;
    dir = opendir(path.c_str());
    if (!dir)
        return 1;
    closedir(dir);
    return 0;
}