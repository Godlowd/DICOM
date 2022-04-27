#pragma once
#include <string>
using namespace std;
class DCTableWidgetProtocol {
	virtual void onPatientHeaderClicked(int row) = 0;
	virtual void onStudyHeaderClicked(int row) = 0;
	virtual void onSeriesHeaderClicked(int row) = 0;

	/**
	 * @brief 当选择了三张表中任意一行时，同时选中三张表的同一行
	 *
	 * @param index selected index
	 */
	virtual void sectionChoose(int index) = 0;

	/**
	 * @brief 编辑表中的值时，将编辑的新值放到临时的存储map中..
	 * 
	 * @param tableIndex 编辑的是哪一张表
	 * @param row 
	 * @param col
	 * @param newValue
	 */
	virtual void updateTempChanges(int tableIndex, int row, int col, string newValue) = 0;
};
