#pragma once
#include <set>
#include <string>
using namespace std;

class DCFilterWidgetProtocol {
public:
	/**
	 * @brief 当checkbox选择的选项更新时，通知table更新筛选条件.
	 * 
	 * @param col 筛选条件更新的列
	 * @param filters 更新后的筛选条件
	 */
	 virtual void updateFilterCondition(int col, vector<string> filters) = 0;
};
