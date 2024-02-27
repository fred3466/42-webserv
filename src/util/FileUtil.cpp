/*
 * FileUtil.cpp
 *
 *  Created on: 24 févr. 2024
 *      Author: fbourgue
 */

#include "FileUtil.h"

FileUtil::FileUtil()
{
	// TODO Auto-generated constructor stub

}

FileUtil::~FileUtil()
{
	// TODO Auto-generated destructor stub
}

//FileUtil::FileUtil(const FileUtil &other)
//{
//	// TODO Auto-generated constructor stub
//
//}
//
//FileUtil& FileUtil::operator=(const FileUtil &other)
//{
//	// TODO Auto-generated method stub
//
//}

int FileUtil::readFile(std::string path, std::string body)
{
	int rno, fee;
	char name[50];

	std::ifstream is(path.c_str(), std::ios::binary);

	// get length of file:
	is.seekg(0, is.end);
	int length = is.tellg();
	is.seekg(0, is.beg);

	char *buffer = new char[length];
	is.read(buffer, length);
	body += buffer;
	return length;
}

int getdir(std::string dir, std::vector<std::string> &files)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL)
	{
		std::cout << "Error(" << errno << ") opening " << dir << std::endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL)
	{
		files.push_back(std::string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}
std::vector<std::string> FileUtil::listDir(std::string path)
{
	std::vector<std::string> files = std::vector<std::string>();
	getdir(path, files);

//	for (unsigned int i = 0; i < files.size(); i++)
//	{
////				std::cout << files[i] << std::endl;
//
//		std::string sending = "<a href=\"" + files[i] + "\">" + files[i]
//				+ "</a>\n";
//
//		body += sending;
//
////				int length = sending.length() + 1;
////				char *cstr = new char[length];
////				std::strcpy(cstr, sending.c_str());
////
////				send(request->getFdClient(), cstr, length - 1, 0);
//	}
	return files;
}

