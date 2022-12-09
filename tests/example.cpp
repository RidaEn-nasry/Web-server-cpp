/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:02:37 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/11 15:40:45 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <tests.hpp>

// use macros that suites your tests
// avaliable macros:
// REQUIRE(expr, tets_name, failire_msg); // require expr to be true
// REQUIRE_STREQ(x, y, test_name, failure_msg); // require x == y where x and y are strings
// REQUIRE_EQ(x, y, test_name, failure_msg); // require x == y
// REQUIRE_NEQ(x, y, test_name, failure_msg); // require x != y
// REQUIRE_GT(x, y, test_name, failure_msg); // require x > y
// REQUIRE_LT(x, y, test_name, failure_msg); // require x < y
// REQUIRE_GE(x, y, test_name, failure_msg); // require x >= y
// REQUIRE_LE(x, y, test_name, failure_msg); // require x <= y
// REQUIRE_NULL(x, test_name, failure_msg); // require x == NULL
// REQUIRE_NOT_NULL(x, test_name, failure_msg); // require x != NULL


void ft_example_tests_suite() {
    
    {
       // give test_name here
        std::string test_name("example tests");

        int result = 1;
        // call the function to test
        // int result = function_to_test(i);
        REQUIRE(result == 0, test_name, "function babla shoudl return 1");
        // at the end call INFO_OK(test_name) to declare the test as passed
        INFO_OK(test_name); 
    }

    
}