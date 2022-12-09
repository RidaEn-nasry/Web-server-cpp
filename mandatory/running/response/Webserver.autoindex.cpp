/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.autoindex.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 21:18:04 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/03 16:33:09 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

void Webserver::_initialize_auto_index(get_parse *g_parse, map_strings &locs, string &root)
{
	fstream file;

	if (g_parse->status == 200 && locs["autoindex"] == "off") {
		if (g_parse->file_type == "html") {
			file.open("/tmp/index.html", fstream::out);
			file << autoindex(g_parse, locs);
			file.close();
			root = "/tmp/index.html";
		}
	}
}

string Webserver::autoindex(get_parse *g_parse, map_strings locs)
{
	DIR *dr, *dr2;
   struct dirent *en;
   string location = locs["root"] + g_parse->path;
   string body, filename, tmp;
   struct stat result;
   body += "<html><head><title>Index of " + g_parse->path + "</title></head><h1>Index of " 
   	+ g_parse->path + "</h1><body><table><tr><th>Name</th><th>Last Modified</th><th>Size</th></tr>";
   dr = opendir(location.c_str());
   if (dr) {
      while ((en = readdir(dr)) != NULL) {
		if ((string)en->d_name == "." || (string)en->d_name == "..")
			continue;
		filename = locs["root"] + g_parse->path  + "/" + en->d_name;
		 if(stat(filename.c_str(), &result) == 0){
			tmp = locs["root"] + g_parse->path + "/" + en->d_name;
			dr2 = opendir(tmp.c_str());
			if (!dr2){
				body += "<tr><td><a href=\"" + g_parse->path + "/" + en->d_name + "\">" + en->d_name + "</a></td><td>"
						+ ctime(&result.st_mtime) + "</td><td>" + _to_string(result.st_size) + "</td></tr>";
			}
			else{
				body += "<tr><td><a href=\"" + g_parse->path + en->d_name + "\">" + en->d_name + "</a></td><td>"
					+ ctime(&result.st_mtime) + "</td><td>" + _to_string(result.st_size) + "</td></tr>";
				closedir(dr2);
			}
		 }		 
	  }
      closedir(dr);
   }
   body += "</html>";
   return body;
}