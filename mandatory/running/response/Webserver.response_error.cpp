/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.response_error.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 16:15:53 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/06 20:56:47 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

void Webserver::_response_error(get_parse *g_parse, string &root, v_servers::iterator &server_it, map_strings &locs)
{
	string tmp_path;
	fstream file;

	tmp_path = parse_path(root, g_parse);
	// if (g_parse->cache_type == "max-age=0")
	//     g_parse->status = 304;
	// else if (g_parse->status == 0)
	//     g_parse->status = 200;
	// check for valid path if yes go no error page
	_initialize_auto_index(g_parse, locs, root);
	if (g_parse->http_v != "1.1") {
		g_parse->status = 500;
		root = multimap_value(server_it->_server_data, "error_page_500");
	}
	if (error_page(tmp_path)) {
		g_parse->status = 404;
		root = multimap_value(server_it->_server_data, "error_page_404");
	}
	else
	{
		fstream file(root.c_str(), fstream::in);
		if (!file) {
			g_parse->status = 403;
			root = multimap_value(server_it->_server_data, "error_page_403");
		}
		file.close();
	}
}

void Webserver::_response_error(post_parse *p_parse, string &root, v_servers::iterator &server_it)
{
	string tmp_path;

	tmp_path = parse_path(root, p_parse);
	if (p_parse->http_v != "1.1") {
		p_parse->status = 500;
		root = multimap_value(server_it->_server_data, "error_page_500");
	}
	if (error_page(tmp_path)) {
		p_parse->status = 404;
		root = multimap_value(server_it->_server_data, "error_page_404");
	}
	else
    {
        fstream file(root.c_str(), fstream::in);
        if (!file) {
			p_parse->status = 403;
            root = multimap_value(server_it->_server_data, "error_page_403");
        }
        file.close();
    }
	if (p_parse->status == 201){
		root = multimap_value(server_it->_server_data, "page_201_created");
	}
}

void Webserver::_response_error(delete_parse *d_parse, string &root, v_servers::iterator &server_it)
{
	string tmp_path;

	tmp_path = parse_path(root, d_parse);
	if (d_parse->http_v != "1.1"){
		d_parse->status = 500;
		root = multimap_value(server_it->_server_data, "error_page_500");
	}
    if (error_page(tmp_path)){
        d_parse->status = 404;
    	root = multimap_value(server_it->_server_data, "error_page_404");
    }
    else
    {
        fstream file(root.c_str(), fstream::in);
        if (!file) {
			d_parse->status = 204;
        }
        file.close();
    }
}