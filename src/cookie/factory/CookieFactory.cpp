#include "CookieFactory.h"
#include "../../Harl.h"

CookieFactory::CookieFactory() {
}

CookieFactory::~CookieFactory() {
}

void CookieFactory::build(RequestHeader *header) {
	header->clearCookies();

	std::string cookieHeader = header->getFieldValue("Cookie");
	if (!cookieHeader.empty()) {
		size_t pos = 0;
		std::string token;
		Cookie c;
		while ((pos = cookieHeader.find("; ")) < cookieHeader.size()) {
			token = cookieHeader.substr(0, pos);
			if (pos + 2 < cookieHeader.size())
				cookieHeader.erase(0, pos + 2);
			c.setName(token.substr(0, token.find("=")));
			c.setValue(token.substr(token.find("=") + 1));
			header->addCookie(c);
			Harl().debug("CookieFactory::build: Ajout cookie au header req: %s", c.getCookie().c_str());
		}
		pos = cookieHeader.find("\r");
		token = cookieHeader.substr(0, pos);
		c.setName(token.substr(0, token.find("=")));
		c.setValue(token.substr(token.find("=") + 1));
		header->addCookie(c);
		Harl().debug("CookieFactory::build: Ajout cookie au header req: %s", c.getCookie().c_str());
	}
	Harl().debug(header->getCookieString());
}
