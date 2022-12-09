
### "make" for websevr 
### "make test" to compile tests 
### "test <testname>" to run a test

NAME = webserv
BONUS = webserv_bonus
CC = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++11 -g
RM = rm -rf
INCLUDE = ./mandatory/includes/webserver.hpp ./mandatory/includes/common.hpp
INCLUDE_BONUS = ./bonus/includes/webserver_bonus.hpp ./bonus/includes/common_bonus.hpp
INC_BNS = -I./src/bonus/includes/
INCLUDES = -I./mandatory/includes/ -I./tests/
TEST_INCLUDES = -I./mandatory/includes/ -I./tests/
SRCS = $(wildcard ./mandatory/parsing/*.cpp ./mandatory/*.cpp ./mandatory/running/*.cpp ./mandatory/running/request/*.cpp ./mandatory/running/response/*.cpp ./mandatory/running/cgi/*.cpp)
SRCS_BONUS = $(wildcard ./bonus/parsing/*.cpp ./bonus/*.cpp ./bonus/running/*.cpp ./bonus/running/request/*.cpp ./bonus/running/response/*.cpp ./bonus/running/cgi/*.cpp ./bonus/running/sessions/*.cpp)
# test srcs include all srcs except main.cpp
TEST_SRCS = $(wildcard tests/*.cpp) $(filter-out src/main.cpp, $(SRCS))
# TEST_SRCS+=$(filter-out ./src/main.cpp, $(SRCS))
PREFIX = ./obj/

OBJS = $(addprefix $(PREFIX), $(SRCS:.cpp=.o))
OBJS_BONUS = $(addprefix $(PREFIX), $(SRCS_BONUS:.cpp=.o))

all: $(NAME)
bonus: $(BONUS)

$(PREFIX) :
	@mkdir -p $(PREFIX)
	@mkdir -p $(PREFIX)/mandatory/parsing
	@mkdir -p $(PREFIX)/mandatory/running
	@mkdir -p $(PREFIX)/mandatory/running/request
	@mkdir -p $(PREFIX)/mandatory/running/response
	@mkdir -p $(PREFIX)/mandatory/running/cgi
	@mkdir -p $(PREFIX)/bonus/parsing
	@mkdir -p $(PREFIX)/bonus/running
	@mkdir -p $(PREFIX)/bonus/running/request
	@mkdir -p $(PREFIX)/bonus/running/response
	@mkdir -p $(PREFIX)/bonus/running/cgi
	@mkdir -p $(PREFIX)/bonus/running/sessions

$(NAME): $(PREFIX) $(OBJS) $(INCLUDE)
	@$(CC) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@printf "\033[2K\r\033[0;32;1mTo Compile => ./webserv [/config/default.conf]\n\033[0m"

$(BONUS): $(PREFIX) $(OBJS_BONUS) $(INCLUDE_BONUS)
	@$(CC) $(CXXFLAGS) $(OBJS_BONUS) $(INC_BNS) -o $(BONUS)
	@printf "\033[2K\r\033[0;32;1mTo Compile => ./webserv_bonus [/config/default.conf]\n\033[0m"

$(PREFIX)%.o: %.cpp $(INCLUDE) $(INCLUDE_BONUS)
	@$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@printf "\033[2K\r\033[0;32mCompiling $<\033[0m"

test: $(TEST_SRCS) ./tests/tests.hpp
	$(CC) $(CXXFLAGS) $(TEST_INCLUDES) $(TEST_SRCS) -o test

clean:
	@$(RM) $(OBJS) $(TEST_SRCS:.cpp=.o) $(OBJS_BONUS) 
	@rm -rf $(PREFIX)
	@printf "\033[2K\r\033[0;32mCleaned!\033[0m"

fclean: clean
	@$(RM) $(NAME) test $(BONUS)
	@printf "\033[2K\r\033[0;32mFile Cleaned!\033[0m"

re: fclean all

re_bonus: fclean bonus

.PHONY: all clean fclean re

