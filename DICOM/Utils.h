#pragma once
#include <vector>
#include <QSqlQuery>

class Utils
{
public:
	template<class T>
	static std::vector<T> singleObjectVectorWrapper(T singleObj);

	static int resultSize(QSqlQuery query);
};

