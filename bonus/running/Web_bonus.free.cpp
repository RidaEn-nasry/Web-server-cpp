/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Web_bonus.free.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:16:56 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 16:17:29 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common_bonus.hpp"
#include "../includes/webserver_bonus.hpp"

void Webserver::add_garbage()
{
	if (_request){
		if (_request->method == GET){
			get_parse *g_parse = (get_parse*)_request->parsed_method;
			_free.push_back(g_parse);
		}
		else if (_request->method == POST){
			post_parse *p_parse = (post_parse*)_request->parsed_method;
			p_parse->body.clear();
			_free.push_back(p_parse);
		}
		else if (_request->method == DELETE){
			delete_parse *d_parse = (delete_parse*)_request->parsed_method;
			_free.push_back(d_parse);
		}
	}
	_free.push_back(_request);
	_free.push_back(_response);
}
