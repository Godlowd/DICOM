#pragma once
#include <QTableWidget>
#include <string>
#include <vector>
#include "DCDicomFileModel.h"
#include <map>
#include <set>
#include "DCFilterWidgetProtocol.h"

using namespace std;
class DCTabelWidget : public QTableWidget, public DCFilterWidgetProtocol
{
public:
	DCTabelWidget::DCTabelWidget(QWidget *parent, vector<string> headerName, bool adjustContent = true);
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

	const vector<string> getShowItemsAtCol(const int col);
	const set<string> getShowItemsSetAtCol(const int col);

	void updateFilterCondition(int col, vector<string> filters);

	bool isFiltering;
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

	
};

