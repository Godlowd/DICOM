#pragma once
#include <set>
#include <string>
using namespace std;

class DCFilterWidgetProtocol {
public:
	/**
	 * @brief 当筛选框的某一个checkbox状态改变时，调用这个方法。传递最新的筛选条件给MainWidget.
	 * 
	 * @param filters 最新的筛选条件
	 */
	 virtual void updateFilterCondition(map<int, set<string>> filters) = 0;
};
