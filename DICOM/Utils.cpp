#include "Utils.h"
#include <vector>
template <class T>
// ��������һ�������װ�ɶ�Ӧ������
static std::vector<T> Utils::singleObjectVectorWrapper(T singleObj) {
	std::vector<T> wrapper = { singleObj };
	return wrapper;
}