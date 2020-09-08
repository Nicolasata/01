NAME	= server

CC	= g++ -lpthread -lrt -pthread 

RM	= rm -f

SRCS	= ./main.cpp \
	  ./src/Server.cpp \
	  ./src/ServerPlayerMethods.cpp \
	  ./src/ServerPlayerMovementMethods.cpp \
	  ./src/Entity/Entity.cpp \
	  ./src/World/Chunk.cpp \
	  ./src/World/World.cpp \
	  ../../common/ThreadPool.cpp \

OBJS	= $(SRCS:.cpp=.o)

CPPFLAGS = -std=c++17 -I ./src/pp
CPPFLAGS += -Wall -Wextra

LDFLAGS = -L../../common/Lib -lraknet

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
