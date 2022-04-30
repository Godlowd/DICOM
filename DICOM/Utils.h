#pragma once
#include <vector>
#include <QSqlQuery>
#include <string>
using namespace std;

class Utils
{
public:
	template<class T>
	static std::vector<T> singleObjectVectorWrapper(T singleObj);

	static int resultSize(QSqlQuery query);
	static string decToHex(int dec);
	
};

