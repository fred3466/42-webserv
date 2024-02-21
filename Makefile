

	#connector/ConnectorEventPublisher.cpp 
NAME = webserv
SRCS = \
	Harl.cpp\
	connector/Connector.cpp  \
	connector/ConnectorEvent.cpp \
	connector/ConnectorFactory.cpp \
	connector/ConnectorListener.cpp \
	connector/HttpConnector.cpp \
	request/Request.cpp \
	request/RequestFactory.cpp \
	response/Response.cpp \
    HttpServer.cpp \
	main.cpp 
	
OBJS = $($(addprefix src/,${SRCS}):.cpp=.o)
INC  = 

CC = g++
CFLAGS =  -g -std=c++98  -pedantic-errors #-Wall -Wextra -Werror
LFLAGS = #-lreadline 

RM = rm -rf

all: $(NAME)

$(NAME):  

	$(CC) ${INC} $(CFLAGS) $(addprefix src/,${SRCS}) $(LFLAGS) -o $(NAME) -g

prof:
	$(CC) ${INC} $(CFLAGS) $(addprefix src/,${SRCS}) $(LFLAGS) -o $(NAME)_prof -g -pg
	
clean:
	rm -f $(OBJS)
	

fclean: clean
	rm -f $(NAME)
	rm -f debug

re: fclean all

