#include "FdPollfdServerTuple.h"

FdPollfdServerTuple::FdPollfdServerTuple() :
		_pollfd(), httpServer(), typeEnum(NOT_SET_TYPE), bSocketConnectionCloseRequired(true), bSocketKeepAlive(0), nbWritten(0),
		response(0), request(0), bTooBigError(false) {
}

FdPollfdServerTuple::FdPollfdServerTuple(pollfd *_pollfd, Server *httpServer, FdPollfdServerTupleTypeEnum typeEnum)
: bSocketConnectionCloseRequired(true), bSocketKeepAlive(0), nbWritten(0), response(0), request(0) {

	this->_pollfd = _pollfd;
	this->httpServer = httpServer;
	this->typeEnum = typeEnum;
	this->bSocketConnectionCloseRequired = true;
	this->bSocketKeepAlive = false;
	this->bTooBigError = false;
	nbWritten = 0;

}

FdPollfdServerTuple::FdPollfdServerTuple(FdPollfdServerTuple &bis) {
	this->_pollfd = bis._pollfd;
	this->httpServer = bis.httpServer;
	this->typeEnum = bis.typeEnum;
	this->bSocketConnectionCloseRequired = bis.bSocketConnectionCloseRequired;
	this->bSocketKeepAlive = bis.bSocketKeepAlive;
	this->nbWritten = bis.nbWritten;
	this->response = bis.response;
	this->request = bis.request;
	this->bTooBigError = bis.bTooBigError;

	if (this != &bis)
		*this = bis;
}

FdPollfdServerTuple& FdPollfdServerTuple::operator=(FdPollfdServerTuple &bis) {
	this->_pollfd = bis._pollfd;
	this->httpServer = bis.httpServer;
	this->typeEnum = bis.typeEnum;
	this->bSocketConnectionCloseRequired = bis.bSocketConnectionCloseRequired;
	this->bSocketKeepAlive = bis.bSocketKeepAlive;
	this->nbWritten = bis.nbWritten;
	this->response = bis.response;
	this->request = bis.request;
	this->bTooBigError = bis.bTooBigError;

	return *this;
}

pollfd* FdPollfdServerTuple::getPollfd() {
	return _pollfd;
}

void FdPollfdServerTuple::setPollfd(pollfd *pollfd) {
	_pollfd = pollfd;
}

int FdPollfdServerTuple::getFd() {
	if (_pollfd)
		return _pollfd->fd;
	return -1;
}

void FdPollfdServerTuple::setFd(int fd) {
	if (_pollfd)
		_pollfd->fd = fd;
}

Server* FdPollfdServerTuple::getHttpServer() {
	return httpServer;
}

bool FdPollfdServerTuple::isBConnectionCloseRequired() {
	return bSocketConnectionCloseRequired;
}

bool FdPollfdServerTuple::isBKeepAlive() const {
	return bSocketKeepAlive;
}

int FdPollfdServerTuple::getNbWritten() const {
	return nbWritten;
}

void FdPollfdServerTuple::setNbWritten(int nbWritten) {
	this->nbWritten = nbWritten;
}

void FdPollfdServerTuple::setBKeepAlive(bool bKeepAlive) {
	this->bSocketKeepAlive = bKeepAlive;
}

void FdPollfdServerTuple::setBConnectionCloseRequired(bool bConnectionCloseRequired) {
	this->bSocketConnectionCloseRequired = bConnectionCloseRequired;
}

void FdPollfdServerTuple::setHttpServer(Server *httpServer) {
	this->httpServer = httpServer;
}

FdPollfdServerTuple::~FdPollfdServerTuple() {
}

FdPollfdServerTupleTypeEnum FdPollfdServerTuple::getTypeEnum() const
{
	return typeEnum;
}

Response* FdPollfdServerTuple::getResponse()
{
	return response;
}

Request* FdPollfdServerTuple::getRequest()
{
	return request;
}

bool FdPollfdServerTuple::isBTooBigError() const
{
	return bTooBigError;
}

void FdPollfdServerTuple::setBTooBigError(bool bTooBigError)
		{
	this->bTooBigError = bTooBigError;
}

void FdPollfdServerTuple::setRequest(Request *request)
		{
	this->request = request;
}

void FdPollfdServerTuple::setResponse(Response *response)
		{
	this->response = response;
}

void FdPollfdServerTuple::setTypeEnum(FdPollfdServerTupleTypeEnum typeEnum)
		{
	this->typeEnum = typeEnum;
}
