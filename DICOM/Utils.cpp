#include "Utils.h"
#include <vector>
template <class T>
// 将单独的一个对象包装成对应的向量
static std::vector<T> Utils::singleObjectVectorWrapper(T singleObj) {
	std::vector<T> wrapper = { singleObj };
	return wrapper;
}