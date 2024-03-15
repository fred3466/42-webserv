#include "CookieFactory.h"
#include "../../Harl.h"

CookieFactory::CookieFactory()
{
}

CookieFactory::~CookieFactory()
{
}

void CookieFactory::build(RequestHeader *header)
{
	std::string cookieHeader = header->getFieldValue("Cookie");
	if (!cookieHeader.empty())
	{
		cookieHeader.erase(0, 1);
		size_t pos = 0;
		std::string token;
		Cookie c;
		while ((pos = cookieHeader.find("; ")) < cookieHeader.size())
		{
			token = cookieHeader.substr(0, pos);
			if (pos + 2 < cookieHeader.size())
				cookieHeader.erase(0, pos + 2);
			c.setName(token.substr(0, token.find("=")));
			c.setValue(token.substr(token.find("=") + 1));
			header->addCookie(c);
		}
		pos = cookieHeader.find("\r");
		token = cookieHeader.substr(0, pos);
		c.setName(token.substr(0, token.find("=")));
		c.setValue(token.substr(token.find("=") + 1));
		header->addCookie(c);
	}
	Harl().debug(header->getCookieString());
}