#pragma once
#include <list>
#include <cstring>
#include <cstddef>
#include <sys/types.h>

#include "../config.h"
#include "../Harl.h"

enum ConnectorBufferTypeEnum {
	TYPE_READ, TYPE_WRITE
};

class ConnectorBuffer {
private:
	ConnectorBuffer();

	char *buffer;
	size_t len;
	int fd;
	ConnectorBufferTypeEnum type;

	static std::list<ConnectorBuffer*> conBufList_READ;
	static std::list<ConnectorBuffer*> conBufList_WRITE;

	static ConnectorBuffer* findByFd(std::list<ConnectorBuffer*> &conBufList, int fd);
	static void addReplaceToList(std::list<ConnectorBuffer*> &conBufList, ConnectorBuffer *connectorBufferParam);
	static void removeFromList(std::list<ConnectorBuffer*> &conBufList, ConnectorBuffer *connectorBufferParam);
	static void dumpConBufList(std::list<ConnectorBuffer*> &conBufList);

public:

	ConnectorBuffer(int fd, ConnectorBufferTypeEnum type);
	~ConnectorBuffer();

	char* getBuffer() const;
	int getFd() const;
	size_t getLen() const;
	char* concatBuffer(char *buf, size_t bufLen);

	static ConnectorBuffer* findByFd_READ(int fd);
	static void addReplaceToList_READ(ConnectorBuffer *connectorBufferParam);
	static void removeFromList_READ(ConnectorBuffer *connectorBufferParam);
	static void dumpConBufList_READ();

	static ConnectorBuffer* findByFd_WRITE(int fd);
	static void addReplaceToList_WRITE(ConnectorBuffer *connectorBufferParam);
	static void removeFromList_WRITE(ConnectorBuffer *connectorBufferParam);
	static void dumpConBufList_WRITE();
	static void clearAll();
};

