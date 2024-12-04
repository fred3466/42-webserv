#include "RequestResponseParam.h"

RequestResponseParam::~RequestResponseParam() {
}

RequestResponseParam::RequestResponseParam(const RequestResponseParam &o) : eventName(o.eventName), byteBuffer(o.byteBuffer), len(o.len), fdPollfdServerTuple(o.fdPollfdServerTuple), request(o.request) {
	*this = o;
}

RequestResponseParam::RequestResponseParam() : eventName("???"), byteBuffer(NULL), len(0), fdPollfdServerTuple(), request(NULL) {
	this->eventName = eventName;
}
RequestResponseParam::RequestResponseParam(std::string eventName) : byteBuffer(NULL), len(0), fdPollfdServerTuple(), request(NULL) {
	this->eventName = eventName;
}

char* RequestResponseParam::getByteBuffer() {
	return byteBuffer;
}

void RequestResponseParam::setByteBuffer(char *bytess) {
	byteBuffer = bytess;
}

int RequestResponseParam::getLen() {
	return len;
}

std::string const& RequestResponseParam::getEventName() const {
	return eventName;
}

FdPollfdServerTuple* RequestResponseParam::getFdPollfdServerTuple() {
	return fdPollfdServerTuple;
}

void RequestResponseParam::setFdPollfdServerTuple(FdPollfdServerTuple *fdPollfdServerTuple) {
	this->fdPollfdServerTuple = fdPollfdServerTuple;
}

void RequestResponseParam::setLen(int len) {
	this->len = len;
}

int RequestResponseParam::getFdClient() {
	return fdPollfdServerTuple->getFd();
}

Request* RequestResponseParam::getRequest() {
	return request;
}

void RequestResponseParam::setRequest(Request *request) {
	this->request = request;
}
