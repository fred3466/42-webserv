#include <string>
#include "config/ConfigReader.h"
#include "HttpServer.h"
#include "util/FileUtil.h"
#include "Uri/Uri.h"

// void bye(){
HttpServer server;
// }

void signalHandler(int signum)
{
	std::cout << "GoodBye.\n";
	exit(signum);
}

int main(int ac, char **av)
{
	Harl harl = Harl();
	FileUtil fu = FileUtil();
	// TODO gestion des erreurs

	if (ac == 2)
	{
		std::string path = std::string(av[1]);
		//				std::string path = "conf/webserv.conf";
		ConfigReader cr = ConfigReader();
		signal(SIGINT, signalHandler);
		std::vector<Config *> configVector = std::vector<Config *>();
		if (!fu.fileExists(path))
		{
			harl.error("Config file missing for path : [%s]", path.c_str());
			return -1;
		}
		harl.info("Reading config file : [%s]", path.c_str());
		bool bValidated = cr.buildConfigVector(&configVector, path);
		if (bValidated)
		{
//			TOTO lancer tous les serveurs, et pas seulement le premier
			server.init(configVector[0]);
		}
		else
		{
			harl.error("ERROR");
		}
		//	TODO ne pas oublier de nettoyer le vecteur de config*
	}
	else
	{
		std::cout
		<< "Nombre d'argument incorrect, syntaxe :\n webserv <chemin vers le fichier de configuration>"
				<< std::endl;
	}
}

// int main(int argc, char **argv)
// {
// 	if (argc != 2)
// 	{
// 		std::cerr << "Usage: webserv <configuration file path>\n";
// 		return 1;
// 	}

// 	std::string path = argv[1];
// 	ConfigReader cr;
// 	std::vector<Config *> configVector;

// 	if (!cr.buildConfigVector(&configVector, path))
// 	{
// 		std::cerr << "Failed to read configuration from " << path << std::endl;
// 		return 1;
// 	}

// 	HttpServer server;
// 	server.init(configVector[0]);

// 	for (size_t i = 0; i < configVector.size(); ++i)
// 	{
// 		delete configVector[i];
// 	}

// 	return 0;
// }
