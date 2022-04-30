#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
template <class T>
// ��������һ�������װ�ɶ�Ӧ������
static std::vector<T> Utils::singleObjectVectorWrapper(T singleObj) {
	std::vector<T> wrapper = { singleObj };
	return wrapper;
}

int Utils::resultSize(QSqlQuery query)
{
	int initPos = query.at();
	if (query.last()) {
		int size = query.at() + 1;
		query.seek(initPos);
		return size;
	}

	return 0;
}

string Utils::decToHex(int dec)
{
	stringstream ss;
	ss << std::hex << dec;

	return ss.str();
}
