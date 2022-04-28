﻿#pragma once
#include <QTableWidget>
#include <string>
#include <vector>
#include "DCDicomFileModel.h"
#include <map>
#include <set>
#include "DCFilterWidgetProtocol.h"
#include "DCTableWidgetProtocol.h"

using namespace std;
class MainWidget;
class DCTableWidget: public QTableWidget, public DCFilterWidgetProtocol
{
	Q_OBJECT
public:
	DCTableWidget::DCTableWidget(QWidget *parent, vector<string> headerName, int tableIndex, MainWidget * delegate, bool adjustContent = true);
	void updateTable(std::vector<std::vector<std::string>> valueArray);
	void setDataForRow(int row, vector<string> value);
	
	/**
	 * @brief filter the table according to the filterMap.
	 * 
	 * @param filterMap key represents the index of the column which has a filter
	 *					value represents the condition 
	 * @return rows which values consists with filterMap
	 */
	vector<int> filter();

	/**
	 * @brief get data for a single col.
	 * 
	 * @param col
	 * @return 
	 */
	const vector<string> getDataMapAtCol(const int col);

	/**
	 * @brief 获取当前展示的某一列的数据，如果table正在筛选，则返回筛选后的数据。
	 *		  若想获得无筛选条件的当前列的数据，则应该使用getDataMapAtCol.
	 * 
	 * @param col
	 * @return 
	 */
	const vector<string> getShowItemsAtCol(const int col);
	const set<string> getShowItemsSetAtCol(const int col);

	void updateFilterCondition(int col, vector<string> filters);

	/**
	 * @brief 禁用某一列的cell，使其不可点击.
	 * 
	 * @param col 要禁用的列索引
	 */
	void disableCellForCol(int col);

	bool isFiltering;

public slots:
	void onCellClicked(int row, int col);
	void onCellChanged(int row, int col);

private:

	/**
	 * used to store string value for each item.
	 */
	vector<vector<string>> dataMap;

	/**
	 * store the filter condition for current table
	 * @key the col which has a set of filters.
	 * @value	filter
	 */
	map<int, vector<string>> filters;

	MainWidget *delegate;

	int tableIndex;

	/**
	 * 当前正在编辑的cell的行号和列号.
	 */
	int editRow;
	int editCol;
};
