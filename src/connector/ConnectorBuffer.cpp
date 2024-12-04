#include "ConnectorBuffer.h"

std::list<ConnectorBuffer*> ConnectorBuffer::conBufList_READ = std::list<ConnectorBuffer*>();
std::list<ConnectorBuffer*> ConnectorBuffer::conBufList_WRITE = std::list<ConnectorBuffer*>();

void ConnectorBuffer::dumpConBufList(std::list<ConnectorBuffer*> &conBufList) {
	Harl harl;

	harl.trace("ConnectorBuffer::dumpConBufList() ");
	for (std::list<ConnectorBuffer*>::iterator it = conBufList.begin(); it != conBufList.end(); ++it) {
		ConnectorBuffer *connectorBuffer = *it;
		harl.trace("-->\tconnectorBuffer fd=%i len=%i", connectorBuffer->fd, connectorBuffer->len);
	}
	harl.trace("\n");
}

char* ConnectorBuffer::concatBuffer(char *buf, size_t bufLen) {
	char *bufferNew = new char[len + bufLen]();
	memset(bufferNew, 0, len + bufLen);
	memcpy(bufferNew, buffer, len);
	memcpy(bufferNew + len, buf, bufLen);
	len += bufLen;

	delete[] buffer;
	buffer = bufferNew;
	return buffer;
}

ConnectorBuffer* ConnectorBuffer::findByFd(std::list<ConnectorBuffer*> &conBufList, int fd) {
	Harl harl;

	for (std::list<ConnectorBuffer*>::iterator it = conBufList.begin(); it != conBufList.end(); ++it) {
		ConnectorBuffer *connectorBuffer = *it;
		if (connectorBuffer && (fd == connectorBuffer->fd)) {
			harl.trace("ConnectorBuffer::findByFd for fd=%i (%i) fd=%i len=%i", fd, connectorBuffer->type, connectorBuffer->fd, connectorBuffer->len);
			return connectorBuffer;
		}
	}
	harl.trace("ConnectorBuffer::findByFd for fd=%i ==> NULL", fd);

	return NULL;
}

void ConnectorBuffer::addReplaceToList(std::list<ConnectorBuffer*> &conBufList, ConnectorBuffer *connectorBufferParam) {
	Harl harl;

	harl.trace("ConnectorBuffer::addReplaceToList %i for fd=%i", connectorBufferParam->type, connectorBufferParam->getFd());

	for (std::list<ConnectorBuffer*>::iterator it = conBufList.begin(); it != conBufList.end(); ++it) {
		ConnectorBuffer *connectorBuffer = *it;
		if (connectorBuffer && (connectorBufferParam->fd == connectorBuffer->fd)) {
			removeFromList(conBufList, connectorBuffer);
			break;
		}
	}

	conBufList.push_back(connectorBufferParam);
}

void ConnectorBuffer::removeFromList(std::list<ConnectorBuffer*> &conBufList, ConnectorBuffer *connectorBufferParam) {
	Harl harl;
	if (connectorBufferParam != NULL)
		harl.trace("ConnectorBuffer::removeFromList %i for fd=%i", connectorBufferParam->type, connectorBufferParam->getFd());
	for (std::list<ConnectorBuffer*>::iterator it = conBufList.begin(); it != conBufList.end(); ++it) {
		ConnectorBuffer *connectorBuffer = *it;
		if ((NULL == connectorBufferParam) || (connectorBufferParam->fd == connectorBuffer->fd)) {
			conBufList.erase(it);
			delete connectorBuffer;
			return;
		}
	}
}

ConnectorBuffer::ConnectorBuffer() : len(0), fd(-1), type(TYPE_READ) {
	buffer = new char[len]();
}

ConnectorBuffer::~ConnectorBuffer() {
	delete[] buffer;
}

ConnectorBuffer::ConnectorBuffer(int fd, ConnectorBufferTypeEnum type) {
	Harl harl;

	harl.trace("ConnectorBuffer::ConnectorBuffer for fd=%i len=%i type=%i", fd, 0, type);
	this->len = 0;
	this->fd = fd;
	this->type = type;
	buffer = new char[0]();

	if (type == TYPE_READ) {
		addReplaceToList_READ(this);
	} else {
		addReplaceToList_WRITE(this);
	}
}

char* ConnectorBuffer::getBuffer() const {
	return buffer;
}

int ConnectorBuffer::getFd() const {
	return fd;
}

size_t ConnectorBuffer::getLen() const {
	return len;
}

ConnectorBuffer* ConnectorBuffer::findByFd_READ(int fd) {
	return findByFd(conBufList_READ, fd);
}

void ConnectorBuffer::addReplaceToList_READ(ConnectorBuffer *connectorBufferParam) {
	addReplaceToList(conBufList_READ, connectorBufferParam);
}
void ConnectorBuffer::removeFromList_READ(ConnectorBuffer *connectorBufferParam) {
	removeFromList(conBufList_READ, connectorBufferParam);
}

void ConnectorBuffer::dumpConBufList_READ() {
	dumpConBufList(conBufList_READ);
}

ConnectorBuffer* ConnectorBuffer::findByFd_WRITE(int fd) {
	return findByFd(conBufList_WRITE, fd);
}

void ConnectorBuffer::addReplaceToList_WRITE(ConnectorBuffer *connectorBufferParam) {
	addReplaceToList(conBufList_WRITE, connectorBufferParam);
}

void ConnectorBuffer::removeFromList_WRITE(ConnectorBuffer *connectorBufferParam) {
	removeFromList(conBufList_WRITE, connectorBufferParam);
}

void ConnectorBuffer::dumpConBufList_WRITE() {
	dumpConBufList(conBufList_WRITE);
}

void ConnectorBuffer::clearAll() {
	removeFromList(conBufList_WRITE, NULL);
	removeFromList(conBufList_READ, NULL);
}
