

	#connector/ConnectorEventPublisher.cpp 
NAME = webserv
SRCS = \
	Harl.cpp\
	connector/Connector.cpp  \
	connector/ConnectorEvent.cpp \
	connector/ConnectorFactory.cpp \
	connector/ConnectorListener.cpp \
	connector/HttpConnector.cpp \
	connector/TcpConnector.cpp \
	request/Request.cpp \
	request/RequestHttp.cpp \
	request/RequestFactory.cpp \
	request/HttpRequest.cpp \
	processor/ProcessorFactory.cpp \
	processor/ProcessorImplDirectFs.cpp \
 	processor/Processor.cpp \
    HttpServer.cpp \
    config.h \
    config/Config.cpp \
    config/ConfigFactory.cpp \
	response/API/Response.cpp \
	response/API/ResponseHeader.cpp \
	response/ResponseHttp.cpp \
	response/ResponseHttpHeader.cpp \
 	response/ResponseTools.cpp \
 	response/factory/ResponseHeaderFactory.cpp \
    response/factory/ResponseFactory.cpp \
    util/FileUtil.cpp \
    util/FileUtilFactory.cpp \
    util/StringUtil.cpp \
	CGI/CGIHandler.cpp \
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

