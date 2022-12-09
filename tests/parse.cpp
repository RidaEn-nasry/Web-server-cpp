#include "tests.hpp"
#include "../includes/webserver.hpp"

typedef multimap<string, string> l_multimap;
typedef map<string, map<string, string> > l_map;
typedef map<string, string> s_map;
typedef vector<string>::iterator v_it;
typedef map<string, map<string, string> >::iterator maps_it;
typedef multimap<string, string>::iterator multimap_it;
typedef vector<Webserver>::iterator serv_it;
typedef map<string, string>::iterator map_it;



void ft_parse_tests_suite() {

	// basic tests
	{
    	l_map main_l, test_l;
		s_map fill;
    	l_multimap main_d, test_d;
    	string line, str;
    	vector<Webserver> servers;
    	fstream file("./config/default.conf", fstream::in);
    	while (std::getline(file, str))
    	    line += " " + str;

		/************ Fill test location map of maps **************/

		fill.insert(make_pair("root", "/home/tankb0y/Desktop/public_html"));
		fill.insert(make_pair("index", "index.html"));
		fill.insert(make_pair("autoindex", "off"));
		test_l.insert(make_pair("/", fill));
		fill.clear();
		
		/************ Fill test location map of maps **************/
	
		fill.insert(make_pair(, "/home/tankb0y/Desktop/public_html"));
		fill.insert(make_pair("allow", "[GET,POST, GET]"));
		test_l.insert(make_pair("/images", fill));
		fill.clear();
		
		/************ Fill test location map of maps **************/

		fill.insert(make_pair("root", "/home/tankb0y/Desktop/public_html"));
		fill.insert(make_pair("allow", "[GET,]"));
		test_l.insert(make_pair("/upload", fill));
		fill.clear();
		
		/************ Fill test location map of maps **************/

		fill.insert(make_pair("root", "/home/tankb0y/Desktop/public_html"));
		fill.insert(make_pair("cgi_bin", "/bin/python2"));
		test_l.insert(make_pair("/cgi", fill));
		fill.clear();

		/************ Fill test location map of maps**************/

		fill.insert(make_pair("return_301", "wwww.disposkill.com"));
		test_l.insert(make_pair("/disposkill", fill));
		fill.clear();
		
		/***************** Fill test server data *******************/

		test_d.insert(make_pair("listen", "8081"));
		// test_d.insert(make_pair("listen", "8081"));
		// test_d.insert(make_pair("listen", "8044"));
		test_d.insert(make_pair("host", "0.0.0.0"));
		test_d.insert(make_pair("include", "config/mime.types"));
		test_d.insert(make_pair("server_name", "weberv.com"));
		test_d.insert(make_pair("client_max_body_size", "1024"));
		test_d.insert(make_pair("error_page_403", "/home/tankb0y/Desktop/public_html/err_test.html"));
		test_d.insert(make_pair("error_page_404", "/home/tankb0y/Desktop/public_html/err_test.html"));

		/************ test server data/ location parsing *************/

		main_l = Webserver::parse_locations(line, main_l);
		main_d = Webserver::parse_server_data(line, main_d);
		REQUIRE_EQ(main_l, test_l, "location parsing", "location parsing error");
		REQUIRE_EQ(main_d, test_d, "server data parsing", "server data error");


		/************** test server data duplicate values *************/

		REQUIRE(!Webserver::data_value_duplicate(main_d), "Duplicate server data", "error duplicate server data");

		/************** test for valid server data **********************/

    for (multimap_it srv_data = main_d.begin(); srv_data != main_d.end(); srv_data++) {

		REQUIRE(!key_comp(srv_data->first), "value comp server data", "error value comp server data");
		REQUIRE(!Webserver::value_comp(srv_data->first, srv_data->second), "value comp server data", "error value comp server data");
		REQUIRE(!Webserver::data_value_duplicate(main_d), "Duplicate server data", "error duplicate server data");
    }

		/************** test for valid server location data ***************/

    for (maps_it path = main_l.begin(); path != main_l.end(); path++) {
        for (map_it locs_data = path->second.begin(); locs_data != path->second.end(); locs_data++)
        {
			REQUIRE(!location_key_comp(locs_data->first), "location key checker", "error location key");
			REQUIRE(!Webserver::location_value_comp(locs_data->first, locs_data->second, path), "location value checker", "error location value");
        }
    }
	
	}
}