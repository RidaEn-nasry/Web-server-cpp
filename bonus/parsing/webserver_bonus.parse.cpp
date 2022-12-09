/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver_bonus.parse.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:05:53 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 19:55:58 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common_bonus.hpp"
#include "../includes/webserver_bonus.hpp"

vector<string> Webserver::split_servers(char* path)
{
    string line, str;
    fstream file(path, fstream::in);
    vector<string> server_line;
    size_t brackets = 0, valid = 0, v_server = 0;

    while (std::getline(file, str))
        line += " " + str;
    file.close();
    // here i seprate every server and push back to a vector (vector of server)
    for (size_t i = 0; i < line.length(); i++) {
        if (line.find("server ") != string::npos || line.find("server{") != string::npos)
            v_server = 1;
        if (line[i] == '{') {
            valid = 1;
            brackets++;
        }
        if (line[i] == '}')
            brackets--;
        if (valid == 1 && brackets == 0 && v_server == 1) {
            server_line.push_back(line.substr(0, i + 1));
            line.erase(0, i + 1);
            // if (syntax_block(line))
            //     throw Webserver::parsingerror();
            valid = 0;
            i = 0;
            v_server = 0;
        }
    }
    // here i check if string if full or empty if full means error happens on server parsing else good
    line = trim(line, " \t\v\f");
    if (brackets != 0 || line != "") {
        cerr << "error : server not valid" << endl, exit(1);
    }
    return server_line;
}

vector<Webserver> Webserver::parse(char* path)
{
    locations_t locations;
    mime_t mime_types;
    config_t server_data;
    vector<Webserver> servers;
    vector<string> server_line;
    // seperated by server { and } 
    server_line = split_servers(path);
    // i iterate over the vector and and parse each server element then puch_back it to vector of Webserver LOL im Good <3
    for (vector<string>::iterator it = server_line.begin(); it != server_line.end(); it++) {
        // parse locaion and fill _location with path & data
        locations = parse_locations(*it, locations);
        // line is clean from locations {....} from previous funtion !!!!!!!!!!!!
        // now start parsing server data
        server_data = parse_server_data(*it, server_data);
        // mime Types parsing same as server data parsing !!!!!
        initialize_block(locations, server_data);
        mime_types = mime_parsing(server_data);
        // all the data is done for now and now time to check error if (all good) => continue else exit(with error type) made with love <3
        servers.push_back(Webserver(server_data, locations, mime_types));
        // clear multimap and map of maps for the next server data and location
        locations.clear();
        server_data.clear();
        mime_types.clear();
    }
    if (servers.empty()) {
        cerr << "error : empty server" << endl, throw Webserver::parsingerror();
    }
    // here where im gonna check for error if (good) return else exit(with error type) love
    for (servers_it srv_it = servers.begin(); srv_it != servers.end(); srv_it++)
    {
        error_parsing(srv_it->_server_data, srv_it->_location_data);
        //check for duplicate server names in server blocks !!!
        if (srv_it + 1 != servers.end()) {
            for (servers_it tmp = srv_it + 1; tmp != servers.end(); tmp++)
                duplicate_server_name(srv_it->_server_data, tmp->_server_data, "listen");
        }
    }
    return servers;
}