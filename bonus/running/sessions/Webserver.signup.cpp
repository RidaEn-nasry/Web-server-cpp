/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.signup.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:48:00 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/09 13:15:23 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/common_bonus.hpp"
#include "../../includes/webserver_bonus.hpp"


const string Webserver::_generate_id() const {
    string id;
    srand(time(0));
    for (int i = 0; i < 10; i++)
        id += (char)(rand() % 26 + 97);
    return id;
}

string  Webserver::_get_expr_date() {

    // create a an expare time of 10 seconds
    time_t now = time(0);
    now += 10;
    string expr_date = _to_string(now);
    return expr_date;


}
bool Webserver::_signup(const string& email, const string& password, Webserver* server) {
    // if user already exist ret
    if (server->_users.size() > 0) {
        map<string, user>::iterator it = server->_users.begin();

        while (it != server->_users.end())
        {
            if (it->second.email == email)
                return false;
            it++;
        }
    }
    // create new user
    string* id = new string(_generate_id());

    user* new_user = new user();
    new_user->email = email;
    new_user->password = password;;
    new_user->exp_date = _get_expr_date();
    server->_users.insert(pair<string, user>(*id, *new_user));
    return true;
}
