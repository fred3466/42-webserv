#include <string>
#include <list>
#include <unistd.h>
#include "config.h"
#include "ServerRunner.h"
#include "config/ConfigReader.h"
#include "API/Connector.h"
#include "HttpServer.h"
#include "util/FileUtil.h"
#include "Uri/Uri.h"

std::vector<Config*> configVector;

void clean() {
	ServerRunner::cleanup();

}

void signalHandler(int signum) {
	if (signum == SIGINT) {
		clean();
	}
}

int main(int ac, char **av) {

	Harl harl;
	FileUtil fu = FileUtil();

//	system("rm -f DBG/*");

	if ((ac == 2) || (ac == 1)) {
		std::string path = "conf/webserv.conf";

		if (ac == 2)
			path.assign(av[1]);

		ConfigReader cr = ConfigReader();
		configVector = std::vector<Config*>();
		if (!fu.fileExists(path)) {
			harl.error("Config file missing for path : [%s]", path.c_str());
			return -1;
		}

		harl.info("Reading config file : [%s]", path.c_str());
		bool bValidated = cr.buildConfigVector(&configVector, path);
		if (bValidated) {
			ServerRunner::run(configVector);
		} else {
			harl.error("ERROR");
		}
	} else {
		std::cout << "Nombre d'argument incorrect, syntaxe :\n webserv <chemin vers le fichier de configuration>" << std::endl;
	}

	harl.warning("Cleaning...");
//	clean();
	ServerRunner::cleanup();

	return 0;
}
