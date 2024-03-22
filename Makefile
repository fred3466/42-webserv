NAME		= webserv
PROJECT		= webserver
INCLUDE		= 
SRC_DIR		= src/
OBJ_DIR		= obj/
CC			= g++
CFLAGS		= -g -std=c++98 -pedantic-errors #-Wall -Wextra -Werror
LFLAGS		= #-lreadline 
RM			= rm -f
AR			= ar rcs

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

SRC_FILES	= \
	Harl\
	connector/Connector  \
	connector/ConnectorEvent \
	connector/ConnectorFactory \
	connector/ConnectorListener \
	connector/HttpConnector \
	connector/TcpConnector \
	request/API/Request \
	request/API/RequestHeader \
	request/RequestHttpHeader \
	request/RequestHttp \
	request/factory/RequestFactory \
	request/factory/RequestHeaderFactory \
 	processor/Processor \
	processor/ProcessorFactory \
	processor/ProcessorImplDirectFs \
	processor/ProcessorImplCgiBinPhp \
	processor/FiltreResponseMimeType \
	processor/FiltreResponseCommon \
	processor/CGI/CGIHandler \
 	location/LocationToProcessor \
 	location/ProcessorAndLocationToProcessor \
 	location/ProcessorLocator \
	mimeType/MimeType \
	mimeType/MimeTypeHelper \
    HttpServer \
    config/Config \
    config/ConfigFactory \
   	config/ConfigReader \
   	config/ConfigValidator \
	response/API/Response \
	response/API/ResponseHeader \
	response/ResponseHttp \
	response/ResponseHttpHeader \
 	response/ResponseTools \
 	response/factory/ResponseHeaderFactory \
    response/factory/ResponseFactory \
    util/FileUtil \
    util/FileUtilFactory \
    util/StringUtil \
   	cookie/Cookie \
	cookie/CookieHelper \
	cookie/factory/CookieFactory \
	error/HttpError \
	error/HttpReturnCodeHelper \
    main 
#    parser/MultipartParser \

SRC 		= 	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILES)))
OBJ 		= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))


OBJF		=	.cache_exists

all:		$(NAME)

$(NAME):	$(OBJ)
			@$(CC) $(OBJ) -o $(NAME)
			@echo "\033[K$(GREEN)$(PROJECT) compiled!$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | $(OBJF)		
			@printf "\033[K$(YELLOW)Compiling: $< $(DEF_COLOR)\r"
			@mkdir -p $(OBJ_DIR)
			@mkdir -p $(dir $(OBJ))
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LFLAGS)

$(OBJF):
			@mkdir -p $(OBJ_DIR)
			@mkdir -p $(dir $(OBJ))

clean:
			@$(RM) -rf $(OBJ_DIR)
			@echo "$(BLUE)$(PROJECT) object files cleaned!$(DEF_COLOR)"

fclean:		clean
			@$(RM) -f $(NAME)
			@echo "$(CYAN)$(PROJECT) executable files cleaned!$(DEF_COLOR)"

re:			fclean all
			@echo "$(GREEN)Cleaned and rebuilt everything for $(PROJECT)!$(DEF_COLOR)"

norm:
			@norminette $(SRC) $(INCLUDE) $(LIBFT) | grep -v Norme -B1 || true

.PHONY:		all clean fclean re norm