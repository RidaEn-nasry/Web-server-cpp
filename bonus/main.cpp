/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:23:40 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/09 18:05:39 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// remember error handling in inet_addr ((unsigned)-1 is 255.255.255.255 which is the broadcast address, fuck);
// #include <webserver_bonus.hpp>
// data sturctures will be : 
 // - multimap<string, string> : key = listen, server_name..., value = 8080, localhost
 // - map<string, map<string, string>> : key = /, /images/... , value = {root, /var/www/html}, {index, index.html}

#include "includes/common_bonus.hpp"
#include "includes/webserver_bonus.hpp"

// server_values* generate_dummy_server() {

//   server_values* data = new server_values;
//   data->insert(pair<string, string>("listen", "8080"));
//   data->insert(pair<string, string>("client_max_body_size", "100M"));
//   data->insert(pair<string, string>("server_name", "localhost"));
//   return data;
// }

// loc* generate_dummy_loc() {

//   loc* data = new loc;
//   map<string, string> *loc1 = new map<string, string>;
//   loc1->insert(pair<string, string>("root", "/var/www/html"));
//   loc1->insert(pair<string, string>("autoindex", "on"));
//   loc1->insert(pair<string, string>("index", "index.html"));
//   loc1->insert(pair<string, string>("allow", "[GET, POST]"));
//   data->insert(pair<string, map<string, string>>("/", loc1));
//   return data;
// }

/********** For testing !!!! ***********/


/**************************************/

int main(int argc, char** argv, char **env)
{

  // obj webservs hold vector<webserver> servers!!!! constractor
  // Webserver websrvs(Webserver::parse(argv[1]));
  
  vector<Webserver> websrvs;
  if (argc < 2)
    argv[1] = (char*)"./config/default.conf";
  fstream file(argv[1], fstream::in);
  if (!file)
    return 1;
  if (argc > 2)
    return 1;
  try {

    websrvs = Webserver::parse(argv[1]);
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  };
  vector<Webserver>::iterator it = websrvs.begin();
  for (; it != websrvs.end(); ++it) {
    it->up(env);
  }
  websrvs[0].run(websrvs);

  return 0;
  
}
