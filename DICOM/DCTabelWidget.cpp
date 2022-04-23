#include "DCTabelWidget.h"
#include "qheaderview.h"
DCTabelWidget::DCTabelWidget(QWidget *parent, vector<string> headerName, bool adjustContent) :
	QTableWidget(parent)
{
	QStringList header;
	for each (string str in headerName)
	{
		header << str.c_str();
	}
	this->setColumnCount(header.size());
	this->setHorizontalHeaderLabels(header);
	if (adjustContent) {
		this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	}
	else {
		this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	}	
}

void DCTabelWidget::updateTable(std::vector<std::vector<std::string>> valueArray)
{
	this->setRowCount(valueArray.size());
	vector<vector<string>> tempDataMap(valueArray.size());
	dataMap = tempDataMap;
	for (int index = 0; index < valueArray.size(); index++) {
		setDataForRow(index, valueArray.at(index));
	}
	resizeRowsToContents();
}

void DCTabelWidget::setDataForRow(int row, vector<string> value)
{
	for (int index = 0; index < value.size(); index++) {
		auto str = value.at(index);
		QTableWidgetItem *item = new QTableWidgetItem(str.c_str());
		item->setTextAlignment(Qt::AlignCenter);
		this->setItem(row, index, item);
		dataMap.at(row) = value;
	}
}

vector<int> DCTabelWidget::filter()
{
	auto rowCount = this->rowCount();
	map<int, string>::iterator iter;
	vector<int> result;


	for (int row = 0; row < rowCount; row++) {
		bool flag = true;
		// according to each filter condition, filter a single row
		for (auto iter = this->filters.begin(); iter != this->filters.end(); iter++) {
			auto col = iter->first;
			if (col > this->columnCount())
				return result;

			auto filterCondition = iter->second;

			auto item = this->item(row, col);
			auto value = item->text().toStdString();

			if (!filterCondition.count(value)) {
				flag = false;
			}
		}

		if (flag)
			result.push_back(row);
	}

	return result;
}

const vector<string> DCTabelWidget::getDataMapAtCol(const int col)
{
	vector<string> result;
	auto rowCount = dataMap.size();
	for (int row = 0; row < rowCount; row++) {
		auto rowVec = dataMap.at(row);

		auto item = rowVec.at(col);
		result.push_back(item);
	}
	return result;
}

const vector<string> DCTabelWidget::getShowItemsAtCol(const int col)
{
	vector<string> result;
	auto rowCount = this->rowCount();
	for (int row = 0; row < rowCount; row++) {
		auto item = this->itemAt(col, row);
		result.push_back(item->text().toStdString());
	}
	return result;
}

const set<string> DCTabelWidget::getShowItemsSetAtCol(const int col)
{
	set<string> result;
	auto rowCount = this->rowCount();
	auto colCount = this->columnCount();
	for (int row = 0; row < rowCount; row++) {
		auto item = this->item(row, col);
		result.insert(item->text().toStdString());
	}
	return result;
}

void DCTabelWidget::updateFilterCondition(map<int, set<string>> filters)
{
	this->filters = filters;
}
