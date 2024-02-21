#include "HttpServer.h"

int main(int ac, char **av) {
	HttpServer server;
	server.init("127.0.0.1", 8080);

//	if(ac == 2){
//		Harl harl = Harl();
//		harl.complain(av[1]);
//	}else{
//		std::cerr << "Nombre d'arguments incorrect." << std::endl;
//	}
}
