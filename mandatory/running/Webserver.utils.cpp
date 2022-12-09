/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.utils.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:26:51 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/06 15:31:50 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>

string Webserver::_to_string(long long num) {
    string str;
    if (num == 0)
        return "0";
    while (num > 0) {
        str += (num % 10) + '0';
        num /= 10;
    }
    reverse(str.begin(), str.end());
    return str;
}


bool   Webserver::_is_err_code(const string& err_code) {
    if (err_code == "400" || err_code == "500" || err_code == "505" || err_code == "413") {
        return true;
    }
    return false;
}

void Webserver::replace_all(string& s, string const& toReplace, string const& replaceWith)
{
    std::string buf;
    std::size_t pos = 0;
    std::size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}