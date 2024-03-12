#include <string>
#include "config/ConfigReader.h"
#include "HttpServer.h"
#include "util/FileUtil.h"

// void bye(){

// }

int main(int ac, char **av)
{
	Harl harl = Harl();
	FileUtil fu = FileUtil();
	HttpServer server;
	// TODO gestion des erreurs
	if (ac == 2)
	{
		std::string path = std::string(av[1]);
//				std::string path = "conf/webserv.conf";
		ConfigReader cr = ConfigReader();
		std::vector<Config*> configVector = std::vector<Config*>();
		if (!fu.fileExists(path))
		{
			harl.error("Config file missing for path : [%s]", path.c_str());
			return -1;
		}
		harl.info("Reading config file : [%s]", path.c_str());
		bool bValidated = cr.buildConfigVector(&configVector, path);
		if (bValidated)
		{
			server.init(configVector[0]);
		} else
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
