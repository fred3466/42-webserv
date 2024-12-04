#pragma once
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <sstream>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include "../../Harl.h"
#include "../../PollManager.h"
#include "../../API/CGIHandler.h"
#include "../../FdPollfdServerTuple.h"
#include "../../ServerRunner.h"
#include "../../PollManager.h"
#include "../../connector/ConnectorBuffer.h"

// Structure avec des tubes full-duplex
typedef struct fdpipe {
	int fd_for_REQUEST[2];
	int fd_for_RESPONSE[2];
} fdpipe;

class CGIHelper {
private:
	static Harl harl;
	static bool bChildProcessTimedOut;
	static std::string content;
	static char *pendingReqBodyContent;
	static int pendingReqBodyLen;

public:
	CGIHelper();
	~CGIHelper();
	static ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor;
	static Response *incompleteResponse;
	static Request *incompleteResponseCorrespondingRequest;

	void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response);
	void feedEnv(char **envp, std::map<std::string, std::string> envMap);
	void _childProcess(CGIHandler *cgiHandler, fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath, std::string &scriptPath, Request *request, char **&envp);
	void _parentProcess(fdpipe *pipes, Request *request, Response *response);
	void executeCGIScript(CGIHandler *cgiHandler, std::string interpreterPath, std::string &scriptPath, Request *request, Response *response);
	void timedOutChildrenRun(pid_t mainProcessPid, long timeOutUs, CGIHandler *cgiHandler, fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath,
			std::string &scriptPath,
			Request *request, char **&envp);
	static void _onTimedOutCgiRequest_SIGNAL_HANDLER(int signal);
	static bool _onDataReceiving(FdPollfdServerTuple *fdPollfdServerTuple);
	static std::string getContent();
	static bool _onWritingEnabled(FdPollfdServerTuple *fdPollfdServerTuple);
};
