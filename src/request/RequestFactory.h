#pragma once

class RequestFactory {
private:
	RequestFactory(const RequestFactory &other);
	RequestFactory& operator=(const RequestFactory &other);

public:
	RequestFactory();
	~RequestFactory();
};

