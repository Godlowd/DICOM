#pragma once
#include <vector>
class Utils
{
public:
	template<class T>
	static std::vector<T> singleObjectVectorWrapper(T singleObj);
};

