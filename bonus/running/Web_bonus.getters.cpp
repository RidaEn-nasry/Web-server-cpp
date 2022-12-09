/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.getters.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:19:10 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/08 16:44:12 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common_bonus.hpp"
#include "../includes/webserver_bonus.hpp"

bool Webserver::_get_port(const string& port) const {
    // remmember loop all over ports of the server
    // find all listen values and check if one of them is the port eqaul_range
    pair<config_t::const_iterator, config_t::const_iterator> ret;

    ret = _server_data.equal_range("listen");
    for (; ret.first != ret.second; ++ret.first) {
        if (ret.first->second == port) {
            return true;
        }
    }
    return false;
};

string  Webserver::_get_host() const {
    string host = _server_data.find("host")->second;
    return host;
}

Webserver* Webserver::_get_server(const string& host, vector<Webserver>& servers) {

    Webserver* server = NULL;
    string port = host.substr(host.find(":") + 1, host.length());
    string actuall_host = host.substr(0, host.find(":"));



    // validating server_name 
    // check if host matches server_name 
    // string server_name = _server_data.find("server_name")->second;
    // string server_name = "www.example.com";
    // if (server_name == actuall_host) {
    //     // check if port matches listen
    //     if (_get_port(port)) {
    //         server = this;
    //     }
    // }
    // else {

    for (vector<Webserver>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        // string server_name = multimap_value(it->_server_data, "server_name");
        string server_name = it->_server_data.find("server_name")->second;
        // cout << "server_name: " << server_name << endl;
        if (server_name == actuall_host) {
            // check if port matches listen
            if (it->_get_port(port)) {
                server = &(*it);
            }
        }
        if (it->_get_host() == "0.0.0.0" || actuall_host == "0.0.0.0") {
            if (it->_get_port(port)) {
                server = &(*it);
                break;
            }
        }
        if ((it->_get_host() == "localhost" && actuall_host == "127.0.0.1") ||
            (it->_get_host() == "127.0.0.1" && actuall_host == "localhost")) {
            if (it->_get_port(port))
            {
                server = &(*it);
                break;
            }
        }
        if (it->_get_host() == actuall_host)
        {
            if (it->_get_port(port))
            {
                server = &(*it);
                break;
            }
        }
    }
    return server;
}


string Webserver::_get_err_page(const string& path) {

    // turn content of file into a string 

    string page;
    fstream file;
    file.open(path.c_str(), std::ios::in);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
            page += line;
        file.close();
    }
    // cout << "page: " << page << endl;

    return page;
}
