#include "HttpErrorFactory.h"

std::vector<HttpError> HttpErrorFactory::errorList;

void HttpErrorFactory::initialize()
{
	loadErrorList();
}

void HttpErrorFactory::loadErrorList()
{
	errorList.clear();

	std::ifstream file("conf/errors.txt");
	std::string line;

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			int code;
			std::string message;
			if (iss >> code && std::getline(iss >> std::ws, message))
			{
				HttpError error(code, message);
				errorList.push_back(error);
			}
		}
		file.close();
	}
}

HttpError* HttpErrorFactory::build(int errorCode)
{
	for (std::vector<HttpError>::const_iterator it = errorList.begin(); it != errorList.end(); ++it)
	{
		if (it->getCode() == errorCode)
		{
			return new HttpError(it->getCode(), it->getDescription());
		}
	}
	return new HttpError(errorCode, "Unknown Error");
}

// Response *HttpErrorFactory::generateErrorResponse(int errorCode, const std::string &errorMessage)
// {
//     // Placeholder for the method to load and format the error page template
//     std::string errorPageContent = "Error Page ........"; // Should load from a template file

//     // Placeholder for replacing placeholders in the errorPageContent
//     // replacePlaceholders(errorPageContent, errorCode, errorMessage);
//     ResponseHeaderFactory factory = ResponseHeaderFactory();

//     ResponseHeader *header = factory.build();
//     std::ostringstream statusLine;
//     statusLine << "HTTP/1.1 " << errorCode << " " << errorMessage << "\r\n";
//     header->setStatusLine(statusLine.str());
//     header->addField("Content-Type", "text/html");

//     ResponseHttp *response = new ResponseHttp(header);
//     char *bodyBin = new char[errorPageContent.length() + 1];
//     std::copy(errorPageContent.begin(), errorPageContent.end(), bodyBin);
//     bodyBin[errorPageContent.length()] = '\0';
//     response->setBodyBin(bodyBin);
//     response->setBodyLength(errorPageContent.length());

//     return response;
// }

// HttpError *HttpErrorFactory::build(int errorCode)
// {
//     std::ifstream file("../../conf/errors.txt");
//     std::string line;
//     std::string errorMessage = "Unknown Error"; // Default error message

//     if (!file.is_open())
//     {
//         return new HttpError(errorCode, errorMessage);
//     }

//     while (std::getline(file, line))
//     {
//         std::istringstream iss(line);
//         int code;
//         std::string message;
//         if (!(iss >> code) || code != errorCode)
//             continue; // Skip lines that don't match the errorCode

//         // Extract the rest of the line as the error message
//         std::getline(iss >> std::ws, message); // std::ws is a manipulator that eats up leading whitespaces
//         errorMessage = message;
//         break; // Found the matching error code, no need to continue
//     }

//     file.close(); // Don't forget to close the file
//     return new HttpError(errorCode, errorMessage);
// }

// HttpError *HttpErrorFactory::build(int errorCode)
// {
//     std::string errorMessage;

//     switch (errorCode)
//     {
//     case 400:
//         errorMessage = "Bad Request";
//         break;
//     case 401:
//         errorMessage = "Unauthorized";
//         break;
//     case 403:
//         errorMessage = "Forbidden";
//         break;
//     case 404:
//         errorMessage = "Not Found";
//         break;
//     case 500:
//         errorMessage = "Internal Server Error";
//         break;
//     case 501:
//         errorMessage = "Not Implemented";
//         break;
//     case 503:
//         errorMessage = "Service Unavailable";
//         break;
//     default:
//         errorMessage = "Unknown Error";
//         break;
//     }

//     return new HttpError(errorCode, errorMessage);
// }