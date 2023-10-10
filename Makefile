NAME			:= ircserv

CC				:= c++

RM				:= rm -f

INCLUDE			:= include

DEBUG_FLAG      := $(shell echo $$DEBUG_FLAG)
#-MMD
CFLAGS			:= -g -Wall -Wextra -Werror -std=c++98 -I $(INCLUDE) $(DEBUG_FLAG) 
VPATH           := src/

SRC_FILES		:= main.cpp Channel.cpp Server.cpp User.cpp UserManagement.cpp ServerResponses.cpp

ODIR			:= obj

OBJS_O			:= $(SRC_FILES:%.cpp=$(ODIR)/%.o)

OBJS_D			:= $(SRC_FILES:%.cpp=$(ODIR)/%.d)

all: $(NAME)

$(NAME): $(ODIR) $(OBJS_O) 
	$(CC) $(CFLAGS) $(OBJS_O) -o $@

$(ODIR)/%.o: %.cpp | $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR):
	mkdir $(ODIR)

clean:
	$(RM) $(OBJS_O) $(OBJS_D)

fclean: clean
	$(RM) $(NAME)
	$(RM) test

re: fclean all

.PHONY: all clean fclean re test test_build

test_build:
	$(CC) $(CFLAGS) test.cpp src/UserManagement.cpp src/Channel.cpp src/User.cpp src/Server.cpp -o test 

test: test_build
	@clear
	@./test
