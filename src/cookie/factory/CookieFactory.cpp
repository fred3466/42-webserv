#include "CookieFactory.h"

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
		cookieHeader.erase(0, 7);
		size_t pos = 0;
		std::string token;
		Cookie c;
		while ((pos = cookieHeader.find("; ")) != std::string::npos)
		{
    		token = cookieHeader.substr(0, pos);
    		cookieHeader.erase(0, pos + 2);
			c.setName(token.substr(0, token.find("=")));
			c.setValue(token.substr(token.find("=") + 1));
			header->addCookie(c);
		}
	}
}