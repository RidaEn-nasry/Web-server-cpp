/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cgi.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 13:12:48 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/09 18:46:50 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

// checking if path maps to a cgi directory
bool    Webserver::_is_cgi(get_parse* g_parse, map_strings locs) {
    // getting the path of the request
    // if ther's no '.' in the path, it's not a cgi script
    if (locs.find("type_cgi") != locs.end() && locs.find("cgi_bin") != locs.end()) {
        if (locs["type_cgi"] == g_parse->file_type)
            return true;
    }
    return false;
}

bool    Webserver::_is_cgi(post_parse* p_parse, map_strings locs) {
    // getting the path of the request
    // if ther's no '.' in the path, it's not a cgi script
    if (locs.find("type_cgi") != locs.end() && locs.find("cgi_bin") != locs.end()) {
        if (locs["type_cgi"] == p_parse->file_type)
            return true;
    }
    return false;
}

bool Webserver::_handle_cgi(const string& path, const map<string, string>& loc, get_parse* g_parse) {

    string command = loc.find("cgi_bin")->second;
    char* argv[3];
    argv[0] = (char*)command.c_str();
    argv[1] = (char*)path.c_str();
    argv[2] = NULL;
    int fd = 0;
    pid_t pid = fork();
    if (pid == 0) {
        // child process 
        // write body to stdin
        // dup2(STDOUT_FILENO, STDIN_FILENO);
        fd = open("/tmp/output.txt", O_RDONLY | O_WRONLY | O_CREAT, 0666);
        dup2(fd, 1);
        // fclose(stdout);
        if (execve(command.c_str(), argv, envp) == -1) {
            _set_error_code("500", "Internal Server Error", "");
            return false;
        }
    }
    else if (pid > 0) {
        waitpid(-1, NULL, 0);
        // wait(NULL);
        // parent process
        // wait for child to finish
        // keep reading from stdout until EOF
        int fd_2 = open("/tmp/output.txt", O_RDONLY, 0666);
        char buffer[1];
        string output;
        int ret;
        while ((ret = read(fd_2, buffer, 1)) > 0) {
            output += string(buffer, ret);
        }
        g_parse->buffer = output;
        if (ret == -1) {
            _set_error_code("500", "Internal Server Error", "");
            return false;
        }
        remove("/tmp/output.txt");
    }
    else {
        _set_error_code("500", "Internal Server Error", "");
        return false;
    }
    return true;

}

bool    Webserver::_handle_cgi(const string& path, const map<string, string>& loc, post_parse* p_parse) {
    string command = loc.find("cgi_bin")->second;
    char* argv[4];
    argv[0] = (char*)command.c_str();
    argv[1] = (char*)path.c_str();
    argv[2] = (char*)p_parse->body[0].value.c_str();
    argv[3] = NULL;
    int fd_out = 0;
    string body = p_parse->body[0].value;
    pid_t pid = fork();
    if (pid == 0) {
        // child process 
        // write body to stdin
        fd_out = open("/tmp/output.txt", O_RDONLY | O_WRONLY | O_CREAT, 0666);
        if (fd_out == -1) {
            _set_error_code("500", "Internal Server Error", "");
            return false;
        }
        dup2(fd_out, 1);
        if (execve(command.c_str(), argv, envp) == -1) {
            _set_error_code("500", "Internal Server Error", "");
            return false;
        }
    }
    else if (pid > 0) {
        // closing both ends of the pipe
        wait(NULL);
        // parent process
        // wait for child to finish
        // keep reading from stdout until EOF
        int fd_2 = open("/tmp/output.txt", O_RDONLY, 0666);
        if (fd_2 == -1) {
            _set_error_code("500", "Internal Server Error", "");
            return false;
        }
        char buffer[1];
        string output;
        int ret;
        while ((ret = read(fd_2, buffer, 1)) > 0) {
            output += string(buffer, ret);
        }
        p_parse->buffer = output;

        if (ret == -1) {
            _set_error_code("500", "Internal Server Error", "");
            return false;
        }
        remove("/tmp/output.txt");
    }
    else {
        _set_error_code("500", "Internal Server Error", "");
        return false;
    }
    return true;
}