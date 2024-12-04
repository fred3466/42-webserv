#include "MimeTypeList.h"

int MimeTypeList::sz = 0;
MimeType **MimeTypeList::_arr = new MimeType*[0];

void MimeTypeList::cleanUp()
{
	Harl().debug("MimeTypeList::cleanUp : kill %i mimetypes", MimeTypeList::getSz());

	for (int i = 0; i < sz; i++) {
		MimeType *mt = (_arr[i]);
		delete mt;
	}

	delete[] _arr;
}

void MimeTypeList::push_back(MimeType *mt) {
	MimeType **_arrNew = new MimeType*[sz + 1];
	for (int i = 0; i < sz; i++) {
		_arrNew[i] = _arr[i];
	}
	_arrNew[sz++] = mt;

	delete[] _arr;
	MimeTypeList::_arr = _arrNew;
}

std::string MimeTypeList::findMimeType(std::string extension) {
	std::string extensionUpCase = StringUtil().strUpperCase(extension);

	for (int i = 0; i < sz; i++) {
		MimeType *mt = (_arr[i]);
		std::string extTmp = "." + mt->getExtension();
		if (extTmp == extensionUpCase) {
			return mt->getMimeType();
		}
	}
	return "application/octet-stream"; // Default MIME type if not found
}

int MimeTypeList::getSz()
{
	return sz;
}
