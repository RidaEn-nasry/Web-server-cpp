/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cookie.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:51:04 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/09 13:27:04 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"

bool    Webserver::_valid_session(const string& headers, vector<Webserver>& servers) {

    string host = headers.substr(headers.find("Host: ") + 6);
    host = host.substr(0, host.find("\r\n"));
    Webserver* server = _get_server(host, servers);
    if (headers.find("Cookie: ") == string::npos) {
        return false;
    }
    string cookie = headers.substr(headers.find("Cookie: ") + 8);
    string exp_date = cookie.substr(cookie.find("Expires=") + 8);
    exp_date = exp_date.substr(0, exp_date.find("\r\n"));

    cookie = cookie.substr(0, cookie.find("\r\n"));
    cookie = cookie.substr(cookie.find("=") + 1);
    cookie = cookie.substr(0, cookie.find(";"));



    // if cookie doesn't belong to any user 


    if (server->_users.find(cookie) == server->_users.end()) {
        return false;
    }
    // if cookie is expired

    if (_expired_id(exp_date)) {
        return false;
    }
    return true;
}

bool    Webserver::_cookie_path(const string& str) {
    if (str.find("login") != string::npos || str.find("signup") != string::npos)
        return true;
    return false;
}


string Webserver::_get_session_id(const string& email, Webserver* server) {
    map<string, user>::iterator it = server->_users.begin();
    while (it != server->_users.end())
    {
        if (it->second.email == email)
            return it->first;
        it++;
    }
    return "";
}

// pair<string, string>    Webserver::_get_cookie(const string& headers, Webserver* server) {
//     if (headers.find("Cookie: ") == string::npos) {

//         return make_pair("", "");
//     }
//     cout << "headers: " << headers << endl;
//     string cookie = headers.substr(headers.find("Cookie: ") + 8);
//     cookie = cookie.substr(0, cookie.find("\r\n"));
//     // if cookie doesn't belong to any user 
//     if (server->_users.find(cookie) == server->_users.end())
//         return make_pair("", "");
//     // if cookie is expired
//     if (_expired_id(cookie))
//         return make_pair("", "");
//     return make_pair(cookie, server->_users[cookie].email);
// }


void Webserver::_cookies(vector<t_client>& clients, size_t i, vector<Webserver>& servers) {
    // if path is to /signup create a new user 
    string path = clients[i].req->status_line.substr(clients[i].req->status_line.find(" ") + 1);
    path = path.substr(0, path.find(" "));
    string host = clients[i].req->headers.substr(clients[i].req->headers.find("Host: ") + 6);
    host = host.substr(0, host.find("\r\n"));
    Webserver* server = _get_server(host, servers);
    if (path == "/signup")
    {
        pair<string, string> email_password = _get_credntials(clients[i].req->body);
        if (!_signup(email_password.first, email_password.second, server))
        {
            // string cooki = 
            string header = "HTTP/1.1 409 Conflict\r\n";
            string body = "Conflict";
            string resp = header + "Content-Type: text/html\r\nContent-Length: " + _to_string(body.size()) + "\r\n\r\n" + body;
            _send(clients[i].fd, resp.c_str(), resp.size());
            return;
        }
        else {
            string header = "HTTP/1.1 201 Created\r\n";
            string body = "Created";
            string resp = header + "Content-Type: text/html\r\nContent-Length: " + _to_string(body.size()) + "\r\n\r\n" + body;
            _send(clients[i].fd, resp.c_str(), resp.size());
            return;
        }
    }
    else if (path == "/login") {
        pair<string, string> email_password = _get_credntials(clients[i].req->body);
        user u = user(email_password.first, email_password.second);

        if (!_valid_login(u, server))
        {
            cout << "invalid login" << endl;
            string header = "HTTP/1.1 401 Unauthorized\r\n";
            string body = "Unauthorized";
            string resp = header + "Content-Type: text/html\r\nContent-Length: " + _to_string(body.size()) + "\r\n\r\n" + body;
            _send(clients[i].fd, resp.c_str(), resp.size());
            return;
        }
        else {
            // if authorized send a redirect to /game
            string header = "HTTP/1.1 200 OK\r\n";
            string body = "OK";
            string id = _get_session_id(u.email, server);
            user u = server->_users[id];
            if (_expired_id(server->_users[id].exp_date)) {
                u.exp_date = _get_expr_date();
                server->_users.erase(id);
                id = _generate_id();
                server->_users.insert(pair<string, user>(id, u));
            }
            string cookie = "Set-Cookie: id=" + id + ";\r\n";
            string cookie2 = "Set-Cookie: Expires=" + u.exp_date + ";\r\n";
            string resp = header + cookie + cookie2 + "Content-Type: text/html\r\nContent-Length: " + _to_string(body.size()) + "\r\n\r\n" + body;
            _send(clients[i].fd, resp.c_str(), resp.size());
            return;
        }
    }

}


void    Webserver::_handle_invalid_session(int fd) {
    string header = "HTTP/1.1 401 Unauthorized\r\n";
    string body = "<html><head><title>Unauthorized</title></head><body><h1>Where the fuck are you going? You're unauthorized, go <a href=\"/login\">login</a> first</h1></body></html>";
    string resp = header + "Content-Type: text/html\r\nContent-Length: " + _to_string(body.size()) + "\r\n\r\n" + body;
    _send(fd, resp.c_str(), resp.size());
    return;
}