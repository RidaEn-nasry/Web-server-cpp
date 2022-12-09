/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:23:06 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/09 16:31:57 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
#include <common.hpp>
// change to response_h struct for checking status code

// don't forget to uncomment syntax_block condition, and error_pages
// handling error in is_cgi is fucked
// handle error when there isn't an allowed method in the method and the _handle_cgi get an error_page

// don't forget to uncomment client_max_body_size
typedef struct req_s {
	string status_line;
	string headers;
	string body;
} req_t;


// typedef struct res_status_line {
// 	string version;
// 	string status_code;
// 	string status_message;
// };

// typedef struct res_s {
// 	string status_code;
// 	string status_message;
// } res_t;


// typedef struct res_s {

// 	// res_status_line status_line;
// 	string headers;
// 	string body;
// } res_t;



/***************** request data *******************/


typedef struct {

	int status;
	string http_v;
	string path;
	string host;
	string port;
	string content_type;
	string file_name;
	string file_type;
	string cache_type;
	string content_length;
	string part_type; //multipart .. / application
	string format_type; // form-data / raw
	string boundary;
	string query;
	string buffer;
	string location;

} delete_parse;

typedef struct {

	string format_type; // form-data
	string filename; // file_name of upload file
	string content_type; //text/plain / jpg....
	string key; // key
	string value; //value

}	post_body;


typedef struct {

	int status;
	string http_v;
	string path;
	string host;
	string port;
	string content_type;
	string file_name;
	string file_type;
	string cache_type;
	string content_length;
	string part_type; //multipart .. / application
	string format_type; // form-data / raw
	string boundary;
	string query;
	string buffer;
	string location;
	vector<post_body> body;

} post_parse;

typedef struct {

	int	status;
	string http_v;
	string path;
	string host;
	string port;
	string file_name;
	string file_type;
	string cache_type;
	string content_type;
	string content_length;
	string query;
	string buffer;
	string location;
}	get_parse;



typedef struct {

	int method;
	void* parsed_method;

} h_request;

/***************** response data *******************/

typedef struct {

	string date;
	string http_header;
	string code_status;
	string msg_status;
	string err_path;
	string keep_connection;
	string content_length;
	string content_type;
	string server_type;
	string root;
	string buffer;
	string location;
	string cache;
	get_parse* g_parse;
	post_parse* p_parse;
	delete_parse* d_parse;
	h_request* request;

} h_response;

/**************************************************/

typedef struct {

	//////////// response data /////////////
	int fd;
	string 	res_buffer;
	size_t res_buffer_len;
	size_t bytesleft;
	int send_n;
	size_t total;
	
	//////////// request data /////////////
	
	string	req_buffer;
	ssize_t 	content_length;
	size_t  header_length;
	size_t 	total_read;
	int		read_n;
	bool	length_type;
	bool 	chunked_type;
	bool 	done_header;
	bool 	done_body;
	bool 	done_length;
	bool 	request_done;
	req_t* req;
	
	//////////// error data /////////////
	
	bool	error;
	string err_type;

	

} t_client;

class Webserver {

public:

	/*********** Parsing exception ***************/

	class parsingerror : public std::exception {
		virtual const char* what() const throw() {
			return "\033[1;31mConfig error : server syntax error\033[0m";
		}
	};

	class location_error : public std::exception {
		virtual const char* what() const throw() {
			return "\033[1;31mConfig error : location syntax error\033[0m";
		}
	};

	class server_data_error : public std::exception {
		virtual const char* what() const throw() {
			return "\033[1;31mConfig error : server data syntax error\033[0m";
		}
	};


	/*********** server exception ***************/

	class PollException : public std::exception {
		virtual const char* what() const throw() {
			return "poll() failed";
		}
	};

	class AcceptException : public std::exception {
		virtual const char* what() const throw() {
			return "accept() failed";
		}
	};

	class ListenException : public std::exception {
		virtual const char* what() const throw() {
			return "listen() failed";
		}
	};

	class BindException : public std::exception {
		virtual const char* what() const throw() {
			return "bind() failed";
		}
	};

	class SocketException : public std::exception {
		virtual const char* what() const throw() {
			return "socket() failed";
		}
	};

	class InvalidHostException : public std::exception {
		virtual const char* what() const throw() {
			return "Invalid host";
		}
	};


	class RecvException : public std::exception {
		virtual const char* what() const throw() {
			return "recv() failed";
		}
	};
	/*************** member types ***************/

	typedef multimap<string, string> config_t;
	typedef map<string, map<string, string> > locations_t;
	typedef map<string, string> mime_t;


	/************** Webserver member functions **************/

	Webserver(config_t server_vals, locations_t location_vals, mime_t mime_types);
	Webserver(vector<Webserver> servers);

	/************* Parsing public member functions ***************/

	static vector<Webserver> parse(char* path);

	/********************************************************/

	// up create socket, bind it and start lisiting on the port
	void up();
	// actually launch the infinite loop to repsonde to clients requests

	void run(vector<Webserver> servers);
	void shutdown();
	void restart();


	/// dump functions
	void dump_server_data() {
		for (config_t::iterator it = _server_data.begin(); it != _server_data.end(); it++) {
			cout << it->first << " : " << it->second << endl;
		}
	}

	void dump_location_data() {
		for (locations_t::iterator it = _location_data.begin(); it != _location_data.end(); it++) {
			cout << it->first << endl;
			for (map<string, string>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				cout << it2->first << " : " << it2->second << endl;
			}
		}
	};

private:
	// vector<Webserver> _servers;
	config_t _server_data;
	locations_t _location_data;
	// cgi_
	mime_t _mime_types;
	static set<int> _sockets;
	// res_t* _res;
	h_response* _response;
	h_request* _request;
	// res_t* _response;
	/**************** private member types *****************/
	typedef vector<string> v_str_t;
	typedef vector<string>::iterator v_str_it;
	typedef map<string, map<string, string> >::iterator locations_it;
	typedef multimap<string, string> config;
	typedef multimap<string, string>::iterator config_it;
	typedef vector<Webserver>::iterator servers_it;
	typedef map<string, string>::iterator map_str_it;
	typedef vector<int> v_int_t;
	typedef vector<int>::iterator v_int_it;
	typedef map<string, string> map_strings;
	typedef vector<Webserver> v_servers;


	/**************** Parsing private member functions ******************/

	static multimap<string, string> parse_server_data(string& line, config_t _server_data);
	static map<string, map<string, string> > parse_locations(string& line, locations_t _locations);
	static map<string, string> mime_parsing(config_t _server_data);
	static vector<string> split_servers(char* path);
	static void error_parsing(config_t server_data, locations_t locations);
	static void    duplicate_server_name(config_t next, config_t prev, string type);
	static void exit_error(config_t server_data, locations_t locations, string err_str, int exit_type);
	static int path_duplicate(string path, locations_t _locations);
	static int location_value_comp(string key, string value, locations_it path);
	static int location_value_comp_2(string key, string value, string* names);
	static int duplicate_location_data(string path, mime_t location);
	static int data_value_duplicate(config_t server_data);
	static int value_comp(string key, string &value);
	static void    initialize_block(locations_t& _locations, config_t& _server_data);

	/*******************************************************************/

	// Request parsing functions
	bool socket_I(vector<t_client> &clients, vector<pollfd> &fds, size_t *i);
	bool parse_header(vector<t_client> &clients, vector<pollfd> &fds, size_t *i);
	bool parse_content_length(vector<t_client> &clients, vector<pollfd> &fds, size_t *i);
	bool parse_transfer_encoding(vector<t_client> &clients, vector<pollfd> &fds, size_t *i);
	
	// Response parsing functions
	void _initialize_response(vector<t_client> &clients, vector<pollfd> &fds, size_t i, vector<Webserver> &servers);
	void socket_O(vector<t_client> &clients, vector<pollfd> &fds, size_t *i);
	
	// accept
	void _accept(vector<t_client> &clients, vector<pollfd> &fds, size_t i);

	// free
	void add_garbage();
	
	// errors
	bool   _is_err_code(const string& err_code);
	void _error(int errono);
	void _set_error_code(string status_code, const string& status_msg, const string& err_path);

	// GET // method parsing
	h_request* _make_request(req_t* req);
	h_response* _make_response(req_t* req, v_servers& servers);
	get_parse* _get_method(req_t* req, v_servers& servers);
	string parse_path(string path, get_parse* g_parse);
	vector<Webserver>::iterator target_server(get_parse* g_parse, v_servers& servers);
	string multimap_value(multimap<string, string> data, string key);
	int error_page(string path);
	void check_query(string& str, get_parse* g_parse);
	void _parse_header(get_parse *g_parse, req_t* req);
	bool _find_location(get_parse *g_parse, v_servers::iterator &server_it, map_strings &locs, string &root);
	void _initialize_auto_index(get_parse *g_parse, map_strings &locs, string &root);

	
	// get response Error method
	void _response_error(get_parse *g_parse, string &root, v_servers::iterator &server_it, map_strings &locs);


	// POST // method parsing
	post_parse* _post_method(req_t* req);
	post_body _parse_body(string line);
	vector<post_body> _parse_urlencoded(string line);
	vector<post_body> _parse_raw(string line, string type);
	void _process_post_request(post_parse* p_parse, v_servers& servers);
	string parse_path(string path, post_parse* p_parse);
	vector<Webserver>::iterator target_server(post_parse* p_parse, v_servers& servers);
	void _upload_data(string root, post_parse* p_parse);
	bool _entity_too_large(config server_data, post_parse* p_parse);
	void check_query(string& str, post_parse* p_parse);
	void _parse_header(post_parse *p_parse, req_t* req);
	bool _find_location(post_parse *p_parse, v_servers::iterator &server_it, map_strings &locs, string &root);
	
	// post response Error method
	
	void _response_error(post_parse *p_parse, string &root, v_servers::iterator &server_it);
	
	


	// DELETE // method parsing
	delete_parse* _delete_method(req_t* req, v_servers& servers);
	string parse_path(string path, delete_parse* d_parse);
	vector<Webserver>::iterator target_server(delete_parse* d_parse, v_servers& servers);
	void _proccess_delete_request(delete_parse* d_parse, v_servers& servers);
	void check_query(string& str, delete_parse* d_parse);
	void query_parsing(delete_parse *d_parse);
	void _parse_header(delete_parse *d_parse, req_t* req);
	bool _find_location(delete_parse *d_parse, v_servers::iterator &server_it, map_strings &locs, string &root);
	
	// delete response Error method
	
	void _response_error(delete_parse *d_parse, string &root, v_servers::iterator &server_it);

	
	// server error response
	h_response *request_error(t_client *client);
	

	// GET CGI // method
	bool _is_cgi(get_parse* g_parse, map_strings locs);
	bool  _is_cgi(post_parse* p_parse, map_strings locs);
	bool _handle_cgi(const string& path, const map<string, string>& loc, get_parse* g_parse);
	// POST CGI // method
	bool _handle_cgi(const string& path, const map<string, string>& loc, post_parse* p_parse);

	// SEND private function
	h_response* _status_line(int code_type);
	h_response* _initialize_header(h_request* request, int code_type);
	int		_send_response(pollfd client_fd);
	string		_count_content_length(string root);
	int			_send(int socket, const char* buf, size_t len);

	//GET response
	void	_get_response(h_request* request, h_response* response);
	void    _response_cgi(h_response* response, get_parse* g_parse);
	void    _response_redirect(h_response* response, get_parse* g_parse);

	//Post response
	void 	_post_response(h_request* request, h_response* response);
	void    _response_redirect(h_response* response, post_parse* p_parse);
	void    _response_cgi(h_response* response, post_parse* p_parse);

	//delete response
	void 	_delete_response(h_request* request, h_response* response);
	void	_response_redirect(h_response* response, delete_parse* d_parse);
	void    _response_cgi(h_response* response, delete_parse* d_parse);

	// valid_path
	bool valid_path(string, pollfd client_fd);


	// autoindex
	string autoindex(get_parse* g_parse, map_strings locs);


	// utils 
	string _to_string(long long num);
	void replace_all(string& s, string const& toReplace, string const& replaceWith);
	string fix_path(string str);
	
	// getters
	string _get_host() const;
	Webserver* _get_server(const string& host, vector<Webserver>& servers);
	bool _get_port(const string& port) const;
	string _get_err_page(const string& path);


};


/*************** Parsing function  ****************/

vector<string>	split(string s, const string set);
size_t	value_len(string str, size_t i);
size_t	key_len(string str);
string	trim(string s, const string set);
int	empty_brackets(string line);
int	num_check(string value);
int	location_key_comp(string key);
int	key_comp(string key);
int syntax_block(string line);
int find_last_point(string str, size_t i);



/**************************************************/

#endif
