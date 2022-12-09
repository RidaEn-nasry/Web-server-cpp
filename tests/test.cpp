# include <tests.hpp>


 // * steps to add a test: 

 // 1 .create a file with name of test in tests folder. and add your test in it (take inspiration from example.cpp)
 // 2. add the function declaration in tests/tests.hpp
 // 3. add the function call in src/tests/test.cpp as else if block
 // (make sure to run test in format of exmaple.cpp so stuff stays consistent)

int main(int argc, char** argv) {

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <test_name>" << std::endl;
    return 1;
  }
  else if (strcmp(argv[1], "--list") == 0) {
    std::cout << "List of tests:" << std::endl;
    std::cout << "1. example" << std::endl;
    std::cout << "2. example2" << std::endl;
    // ... add more tests here.
  }

  else if (strcmp(argv[1], "example") == 0){
    ft_example_tests_suite();
  }

  else if (strcmp(argv[1], "parsing") == 0){
    ft_parse_tests_suite();
  }

  else{
    std::cout << "test not found" << std::endl;
    std::cout << argv[0] << "--list for a list of tests" << std::endl;
    return 1;
  }
  return 0;
}