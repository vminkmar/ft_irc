NAME            := ircserv

CC              := c++

RM              := rm -f

INCLUDE         := include

DEBUG_FLAG      := $(shell echo $$DEBUG_FLAG)
#-MMD
CFLAGS          := -Wall -Wextra -Werror -std=c++98 -I $(INCLUDE) $(DEBUG_FLAG) 

VPATH           := src/

SRC_MAIN        := main.cpp
SRC_SOURCES     := Server.cpp cmds_vale.cpp cmds_jakob.cpp \
                   Channel.cpp User.cpp UserManagement.cpp \
                   ServerResponses.cpp
SRC_FULL_SOURCES= $(foreach src,$(SRC_SOURCES),src/$(src))

SRC_FILES       := $(SRC_MAIN) $(SRC_SOURCES)

ODIR            := obj

OBJS_O          := $(SRC_FILES:%.cpp=$(ODIR)/%.o)

OBJS_D          := $(SRC_FILES:%.cpp=$(ODIR)/%.d)

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
	$(CC) $(CFLAGS) $(SRC_FULL_SOURCES) test.cpp -o test 

test: test_build
	@clear
	@./test
