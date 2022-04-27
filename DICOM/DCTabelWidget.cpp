#include "DCTabelWidget.h"
#include "qheaderview.h"
#include "MainWidget.h"
#include <algorithm>

#define LIST_WIDTH 1500
#define LIST_HEIGHT 280
DCTabelWidget::DCTabelWidget(QWidget *parent, vector<string> headerName, int tableIndex, MainWidget * delegate, bool adjustContent) :
	QTableWidget(parent),
	tableIndex(tableIndex),
	delegate(delegate),
	editRow(-1),
	editCol(-1)
{
	resize(LIST_WIDTH, LIST_HEIGHT);
	
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
	
	QObject::connect(this->verticalHeader(), SIGNAL(sectionClicked(int)), delegate, SLOT(sectionChoose(int)));
	QObject::connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onCellClicked(int, int)));
	QObject::connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(onItemChanged(QTableWidgetItem *)));
}

void DCTabelWidget::updateTable(std::vector<std::vector<std::string>> valueArray)
{
	this->setRowCount(valueArray.size());
	if (!isFiltering) {
		vector<vector<string>> tempDataMap(valueArray.size());
		dataMap = tempDataMap;
	}

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
		if (!isFiltering)
			dataMap.at(row) = value;
	}
}

vector<int> DCTabelWidget::filter()
{
	auto rowCount = dataMap.size();
	map<int, string>::iterator iter;
	vector<int> result;

	// 最外层按样本遍历
	for (int row = 0; row < rowCount; row++) {
		bool flag = true;
		// 内层判断样本的各个属性条件是否满足要求
		for (auto iter = this->filters.begin(); iter != this->filters.end(); iter++) {
			auto col = iter->first;
			if (col > this->columnCount())
				return result;

			auto filterCondition = iter->second;

			auto value = dataMap.at(row).at(col);

			bool currentRowFlag = false;
			for each (string str in filterCondition)
			{
				if (str.size() == value.size() && str == value) {
					currentRowFlag = currentRowFlag | true;
					break;
				}
				else
					currentRowFlag = currentRowFlag | false;
			}
			flag = flag & currentRowFlag;
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

void DCTabelWidget::updateFilterCondition(int col, vector<string> filters)
{
	if (this->filters.find(col) == this->filters.end())
		this->filters.insert(make_pair(col, filters));
	else
		this->filters.at(col) = filters;
}

void DCTabelWidget::onCellClicked(int row, int col)
{
	editRow = row;
	editCol = col;
}

void DCTabelWidget::onItemChanged(QTableWidgetItem * item)
{
	string newValue = item->text().toStdString();
	if (delegate != nullptr && editRow != -1 && editCol != -1)
		delegate->updateTempChanges(tableIndex, editRow, editCol, newValue);
}
