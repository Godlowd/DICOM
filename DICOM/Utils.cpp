#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>

template <class T>
// 将单独的一个对象包装成对应的向量
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
