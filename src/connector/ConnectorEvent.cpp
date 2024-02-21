#include "ConnectorEvent.h"

ConnectorEvent::~ConnectorEvent() {
}

ConnectorEvent::ConnectorEvent(const ConnectorEvent &o) :
		req(o.req), resp(o.resp), temp(o.temp) {
	*this = o;
}

//ConnectorEvent& ConnectorEvent::operator=(const ConnectorEvent &o) {
//	if (this != &o) {
//		req = o.req;
//		resp = o.resp;
//		temp = o.temp;
//		*this = o;
//	}
//	return *this;
//}

ConnectorEvent::ConnectorEvent(Request req, Response resp) {
	this->req = req;
	this->resp = resp;
}

ConnectorEvent::ConnectorEvent(std::string temp) {
	this->temp = temp;
}

std::string ConnectorEvent::getTemp() {
	return temp;
}
