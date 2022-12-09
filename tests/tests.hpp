/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 11:46:02 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/11 17:39:37 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TESTS_HPP
#define TESTS_HPP

/******* Includes **********/

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <regex.h>
#include <vector>
#include <map>
#include <utility>
#include <stdlib.h>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>

/********** Using **********/

using std::multimap;
using std::map;
using std::make_pair;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::fstream;
using std::pair;
using std::cerr;
using std::atoi;
using std::isalnum;
using std::isdigit;

// print error message and exit
#define INFO_ERR() std::cout << "\033[31m[ERROR]\033[0m " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << "(): " << std::endl;

// call macro if test pass ok.
#define INFO_OK(test_name) std::cout << "\033[1;34m" << test_name << "\033[0m" \
                                     << "\033[1;32m"                           \
                                     << "OK"                                   \
                                     << "\033[0m" << std::endl;

// call macro if test fail.
#define INFO_FAIL(test_name)                            \
  {                                                     \
    std::cout << "\033[1;34m" << test_name << "\033[0m" \
              << "\033[1;31m"                           \
              << "FAIL"                                 \
              << "\033[0m" << std::endl;                \
    INFO_ERR();                                         \
  }
  
#define SUCCESS(test_name)                              \
{                                                       \
    std::cout << "\033[1;32m" << test_name << "\033[0m" \
              << "\033[1;32m"                           \
              << " ✅ SUCCESS"                          \
              << "\033[0m" << std::endl;                \
}                                                       \
  
// macro parint reason behind test failure.
#define REASON(msg)                                             \
  {                                                             \
    std::cout << "\033[1;31m"                                   \
              << "Reason: "                                     \
              << "\033[0m"                                      \
              << "\033[1;31m" << msg << "\033[0m" << std::endl; \
    exit(EXIT_FAILURE);                                         \
  }


// require x to be true.
#define REQUIRE(x, test_name, msg) \
  if (!(x))                        \
  {                                \
    INFO_FAIL(test_name);          \
    REASON(msg);                   \
  }                                \
  else                             \
      SUCCESS(test_name);          \
// equality
#define REQUIRE_EQ(x, y, test_name, msg) \
  if ((x) != (y))                        \
  {                                      \
    INFO_FAIL(test_name);                \
    REASON(msg);                         \
  }                                      \
  else                                   \
      SUCCESS(test_name);                \
  
  // unequality
#define REQUIRE_NEQ(x, y, test_name, msg) \
  if ((x) == (y))                         \
  {                                       \
    INFO_FAIL(test_name);                 \
    REASON(msg);                          \
  }                                         \
  else                                      \
      SUCCESS(test_name)                    \
  // greater than
#define REQUIRE_GT(x, y, test_name, msg) \
  if ((x) <= (y))                        \
  {                                      \
    INFO_FAIL(test_name);                \
    REASON(msg);                         \
  }                                         \
  else                                      \
      SUCCESS(test_name)                    \
  // less than
#define REQUIRE_LT(x, y, test_name, msg) \
  if ((x) >= (y))                        \
  {                                      \
    INFO_FAIL(test_name);                \
    REASON(msg);                         \
  }                                         \
  else                                      \
      SUCCESS(test_name)                    \
// greater than or equal to
#define REQUIRE_GE(x, y, test_name, msg) \
  if ((x) < (y))                         \
  {                                      \
    INFO_FAIL(test_name);                \
    REASON(msg);                         \
  }                                         \
  else                                      \
      SUCCESS(test_name)                    \
  
  // less than or equal to
#define REQUIRE_LE(x, y, test_name, msg) \
  if ((x) > (y))                         \
  {                                      \
    INFO_FAIL(test_name);                \
    REASON(msg);                         \
  }                                         \
  else                                      \
      SUCCESS(test_name)                    \
// string equality
#define REQUIRE_STREQ(x, y, test_name, msg) \
  if (strcmp((x), (y)) != 0)                \
  {                                         \
    INFO_FAIL(test_name);                   \
    REASON(msg);                            \
  }                                         \
  else                                      \
      SUCCESS(test_name)                    \

// null equality
#define REQUIRE_NULL(x, test_name, msg) \
  if ((x) != NULL)                      \
  {                                     \
    INFO_FAIL(test_name);               \
    REASON(msg);                        \
  }                                         \
  else                                      \
      SUCCESS(test_name)                    \
  // not null equality
#define REQUIRE_NOT_NULL(x, test_name, msg) \
  if ((x) == NULL)                          \
  {                                         \
    INFO_FAIL(test_name);                   \
    REASON(msg);                            \
  }                                         \
  else                                      \
      SUCCESS(test_name)                    \
  
  

// add function declaration here
void ft_example_tests_suite();
void ft_parse_tests_suite();


#endif
