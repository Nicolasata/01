NAME	= client

CC	= g++ -lpthread -lrt -pthread -lsfml-graphics -lsfml-window -lsfml-system

RM	= rm -f

SRCS	= ./main.cpp \
	  ./src/Client.cpp \
	  ./src/ClientPlayerMethods.cpp \
	  ./src/ClientGraphicMethods.cpp \
	  ./src/Entity/Entity.cpp \
	  ./src/World/World.cpp \
	  ./src/World/Chunk.cpp \
	  ../../common/ThreadPool.cpp \

OBJS	= $(SRCS:.cpp=.o)

CPPFLAGS = -std=c++17 -I ./src/pp
CPPFLAGS += -Wall -Wextra

LDFLAGS = -L../../common/Lib -lraknet `sdl2-config --libs`

LDLIBS = 

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
