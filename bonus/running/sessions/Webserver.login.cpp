/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.login.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:15:53 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/09 13:15:10 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"


bool    Webserver::_valid_login(const user& suspect, Webserver* server) const {

    string email = suspect.email;
    string password = suspect.password;

    if (email.empty() || password.empty()) {
        return false;
    }
    // search for user in the map
    map<string, user>::iterator it = server->_users.begin();
    while (it != server->_users.end())
    {
        if (it->second.email == email && it->second.password == password)
            return (true);
        it++;
    }
    return (false);
}

bool    Webserver::_expired_id(const string& date) const {
    time_t now = time(0);
    long long expr_date = _to_num(date);
    time_t expr = time_t(expr_date);
    if (now > expr)
        return true;
    return false;
}

bool    Webserver::_is_valid_session(string session_id, user& suspect, Webserver* server) const {

    // is session_id valid?
    (void)suspect;
    if (session_id.empty())
        return false;
    // search for session_id in the map
    map<string, user>::iterator it = server->_users.begin();
    while (it != server->_users.end())
    {
        if (it->first == session_id)
        {
            if (_expired_id(it->first))
                return false;
            return true;
        }
        ++it;
    }
    return false;
};



pair<string, string> Webserver::_get_credntials(const string& body) {
    string email = body.substr(body.find("email") + 6);
    email = email.substr(0, email.find("---"));
    email = trim(email, " ");
    string password = body.substr(body.find("password") + 9);
    password = password.substr(0, password.find("---"));
    password = trim(password, " ");
    return make_pair(email, password);
}



