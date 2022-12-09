/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.err_parsing.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:09:25 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/08 16:49:15 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>


int Webserver::path_duplicate(string path, locations_t _locations)
{
    if (_locations.find(path) == _locations.end())
        return 0;
    return 1;
}

int Webserver::duplicate_location_data(string path, mime_t location)
{
    if (location.find(path) == location.end())
        return 0;
    return 1;
}

int key_comp(string key)
{
    // check for undifend key anything other sting names[6] is error
    string names[16] = {"client_max_body_size", "error_page_400", "error_page_403",
                       "error_page_404", "error_page_405", "error_page_413", "error_page_500",
                       "host", "listen", "server_name", "include", "root", "page_200_delete",
                        "page_201_created", "page_204_no_content", "page_200_ok"};

    if (key.empty())
        return 1;
    for (int i = 0; i < 16; i++) {
        if (names[i] == key)
            return 0;
    }
    return 1;
}

int Webserver::value_comp(string key, string &value)
{
    // here where i check values of server data exmple ==> listen > 0 || listen < 65655
    long total;
    fstream exists;
    DIR *dir;
    string names[16] = {"client_max_body_size", "error_page_403",
    "error_page_404", "host", "listen", "server_name", "include", "root", "error_page_400",
    "error_page_405", "error_page_413", "error_page_500", "page_200_delete",
        "page_201_created", "page_204_no_content", "page_200_ok"};

    if (key != names[5])
        if (value.empty())
            return 1;
    // check client_max_body_size is num and bigger than 0
    if (names[0] == key) {
        total = atol(value.c_str());
        if (total < 0 || (value[value.length() - 1] != 'm')
        || value.find(".") != string::npos)
            return 1;
    }
    // check error_page should be html and valid path
    if (names[1] == key || names[2] == key || names[8] == key
        || names[9] == key || names[10] == key || names[11] == key
        || names[12] == key || names[13] == key || names[14] == key
            || names[15] == key) {
        exists.open(value.c_str(), fstream::in);
        if (!exists || value.find(".html") == string::npos){
            exists.close();
            return 1;
        }
    }
    // check host value shouldnt be less than 0 and  class c ==> private ip
    if (names[3] == key) {
        if (value == "localhost")
            value.assign("127.0.0.1");
        value.erase(remove(value.begin(), value.end(), '.'), value.end());
        if (num_check(value))
            return 1;
        total = atol(value.c_str());
        if (num_check(value) || total < 0 || total > 192168255255)
            return 1;
    }
    // check listen port from 1024 to max port 16bits
    if (names[4] == key) {
        total = atol(value.c_str());
        if (num_check(value) || total < 0 || total > 65535)
            return 1;
    }
    // check name server shouldnt have a sapce or someting like that bc is a after all domain
    if (names[5] == key) {
        for (size_t i = 0; i < value.length(); i++) {
            if (isspace(value[i]))
                return 1;
        }
    }
    // check root on server data
    if (names[7] == key) {
        dir = opendir(value.c_str());
        if (!dir)
            return 1;
        closedir(dir);
    }
    return 0;
}

void Webserver::exit_error(config_t server_data, locations_t locations, string err_str, int exit_type)
{
    // here i free and check with error shoud i output to stderr
    locations.clear();
    server_data.clear();

    if (exit_type == KEY_ERR){
        cerr << "undefined key -> "
             << "\"" << err_str << "\"" << endl;
        throw Webserver::server_data_error();
    }
    if (exit_type == VALUE_ERR){
        cerr << "undefined value -> "
             << "\"" << err_str << "\""
             << " [ +numbers, valid value, valid IP Class A, B, C ]" << endl;
        throw Webserver::server_data_error(); //
    }
    if (exit_type == DUP_DATA_ERR){
        cerr << "duplicate server data " << "\""
             << "[listen, host, client_max_body_size, error_page, server_name]"
             << "\"" << endl;
        throw Webserver::server_data_error();
    }
    if (exit_type == NO_DATA){
        cerr << "" << "\"" << "[listen, host, root]" << "\""
             << " not found" << endl;
        throw Webserver::server_data_error();
    }
    if (exit_type == DUP_PATH){
        cerr << "duplicate path -> "
             << "\"" << err_str << "\"" << endl;
        throw Webserver::location_error();
    }
    if (exit_type == EMPTY_PATH){
        cerr << "empty path" << endl;
        throw Webserver::location_error();
    }
    if (exit_type == L_KEY_ERR){
        cerr << "undefined location key ->  "
             << "\"" << err_str << "\"" << endl;
        throw Webserver::location_error();
    }
    if (exit_type == L_VALUE_ERR){
        cerr << "undefined location value ->  "
             << "\"" << err_str << "\""
             << " [ Valid Path, .html, on, off, GET, POST, DELETE ]" << endl;
        throw Webserver::location_error(); //
    }
    if (exit_type == PATH_RCS_ERR){
        cerr << "duplicate path resources -> "
             << "\"" << err_str << "\"" << endl;
        throw Webserver::location_error();
    }
}

int location_key_comp(string key)
{
    // here i check data of location {listen....} anything else then names[6] is error
    string names[9] = {"root", "index", "allow",
        "autoindex", "cgi_bin", "return_301", "allow_upload", "type_cgi", "upload_at"};
    if (key.empty())
        return (1);
    for (int i = 0; i < 9; i++) {
        if (names[i] == key)
            return 0;
    }
    return 1;
}

int Webserver::location_value_comp_2(string key, string value, string *names)
{
    fstream exists;
    string access;
    vector<string> methods;

    // check allow methods are valid [GET, POST, DELETE] else of this three is error
    if (names[2] == key) {
        value = trim(value, "[]");
        methods = split(value, ",");
        for (vector<string>::iterator it = methods.begin(); it != methods.end(); it++)
        {
            *it = trim(*it, " \t\v\f");
            if (*it != "GET" && *it != "POST" && *it != "DELETE"){
                methods.clear();
                return 1;
            }
        }
        methods.clear();
    }
    // check autoindex only on and off else error
    if (names[3] == key || names[6] == key) {
        if (value != "on" && value != "off")
            return 1;
    }
    // check cgi valid executable and valid path
    if (names[4] == key) {
        exists.open(value.c_str(), fstream::in);
        if (!exists || ((value.find("node") == string::npos))){
            exists.close();
            return 1;
        }
    }
    // check redirect value if valid else error
    if (names[5] == key) {
        for (size_t i = 0; i < value.length(); i++)
        {
            if (isspace(value[i]))
                return 1;
        }
    }
    if (names[7] == key){
        if (value.find("js") == string::npos)
            return 1;
    }
    return 0;
}

int Webserver::location_value_comp(string key, string value, locations_it path)
{
    // here i check values of location root index.......
    DIR *dir;
    string access;
    string names[9] = {"root", "index", "allow",
        "autoindex", "cgi_bin", "return_301", "allow_upload", "type_cgi", "upload_at"};
    if (value.empty())
        return 1;
    // check root if valid
    if (names[0] == key || names[8] == key) {
        if (names[0] == key){
        access = value + path->first;
        dir = opendir(access.c_str());
        if (!dir)
            return 1;
        closedir(dir);
        }
        else{
            dir = opendir(value.c_str());
            if (!dir)
                return 1;
            closedir(dir);
        }
    }
    // check index if valid
    if (names[1] == key) {
        if (value.find(".html") == string::npos)
            return 1;
    }
    return (location_value_comp_2(key, value, names));
}

int Webserver::data_value_duplicate(config_t server_data)
{
    // check for duplicates values or keys like client_max_body_size if dup is error ==> client_max_body_size * 2 ==> error
    // loop on every server data and compare if there's a duplicate keys
    config_it tmp;
    for (config_it it = server_data.begin(); it != server_data.end(); it++) {
        if (it->first == "listen" || it->first == "client_max_body_size"
            || it->first.find("error_page_") != string::npos
                || it->first == "server_name" || it->first == "root") {
            it++;
            tmp = it;
            it--;
            if (it->first == "client_max_body_size") {
                while (tmp != server_data.end()) {
                    if (it->first == tmp->first)
                        return 1;
                    tmp++;
                }
            }
            if (it->first.find("error_page_") != string::npos) {
                while (tmp != server_data.end()) {
                    if (it->first == tmp->first)
                        return 1;
                    tmp++;
                }
            }
            else {
                while (tmp != server_data.end()) {
                    if (it->second == tmp->second)
                        return 1;
                    tmp++;
                }
            }
        }
    }
    return 0;
}

void Webserver::duplicate_server_name(config_t prev, config_t next, string type)
{
    config_t tmp;
    tmp = prev;
    for (config_t::iterator it = next.begin(); it != next.end(); it++)
        tmp.insert(make_pair(it->first, it->second));
    pair <config_t::iterator, config_t::iterator> ret;
    ret = tmp.equal_range(type);
    for (config_t::iterator it = ret.first; it != ret.second; it++){
        config_t::iterator itx = ++it; --it;
        for (; itx != ret.second; itx++){
            if(it->second == itx->second && type == "listen")
                duplicate_server_name(prev, next, "host");
            if(it->second == itx->second && type == "host")
                duplicate_server_name(prev, next, "server_name");
            if(it->second == itx->second && type == "server_name"){
                    cerr << "Duplicate server_name -> " << "\"" << prev.find("server_name")->second << "\"" << endl;
                    throw Webserver::server_data_error();
            }
        }
    }
}

void Webserver::error_parsing(config_t server_data, locations_t locations)
{
    // here where i check error for server data like listen host ..........
    // i loop on every server data and check on by one if error i call exit_error with EXIT_TYPE of the error
    for (config_it srv_data = server_data.begin(); srv_data != server_data.end(); srv_data++) {
        // check key data if valid
        if (key_comp(srv_data->first))
           exit_error(server_data, locations, srv_data->first, KEY_ERR);
        // check value data if valid else error
        if (value_comp(srv_data->first, srv_data->second))
           exit_error(server_data, locations, srv_data->first, VALUE_ERR);
        // check for duplicate values or keys
        if (data_value_duplicate(server_data))
           exit_error(server_data, locations, srv_data->first, DUP_DATA_ERR);
        if (server_data.find("listen") == server_data.end()
            || server_data.find("host") == server_data.end())
           exit_error(server_data, locations, srv_data->first, NO_DATA);
    }
    // here where i loop on location map of maps and check every key and value
    for (locations_it path = locations.begin(); path != locations.end(); path++) {
        // here i check location path and data location /image {root /var/www; .....}
        if (path->second.find("root") == path->second.end())
            exit_error(server_data, locations, "root", NO_DATA);
        for (map_str_it locs_data = path->second.begin(); locs_data != path->second.end(); locs_data++)
        {
            // check if path is empty call exit_error with type of erryr
            if (path->first.empty())
               exit_error(server_data, locations, path->first, EMPTY_PATH);
            // check location key of duplicate or something wrong.....
            if (location_key_comp(locs_data->first))
               exit_error(server_data, locations, locs_data->first, L_KEY_ERR);
            // check loction value ==> valid path and so one....
            if (location_value_comp(locs_data->first, locs_data->second, path))
               exit_error(server_data, locations, locs_data->first, L_VALUE_ERR);
        } 
    }
}