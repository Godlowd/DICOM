#include "DCTableWidget.h"
#include "qheaderview.h"
#include "MainWidget.h"
#include <algorithm>

#define LIST_WIDTH 1500
#define LIST_HEIGHT 280
DCTableWidget::DCTableWidget(QWidget *parent, vector<string> headerName, int tableIndex, MainWidget * delegate, bool adjustContent) :
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
	QObject::connect(this, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(onCurrentCellChanged(int, int, int, int)));
	QObject::connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(onCellChanged(int, int)));
}

void DCTableWidget::updateTable(std::vector<std::vector<std::string>> valueArray)
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

void DCTableWidget::setDataForRow(int row, vector<string> value)
{
	for (int index = 0; index < value.size(); index++) {
		if (!isFiltering)
			dataMap.at(row) = value;
		auto str = value.at(index).c_str();
		QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit(str));
		item->setTextAlignment(Qt::AlignCenter);
		this->setItem(row, index, item);
	}
}

vector<int> DCTableWidget::filter()
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

const vector<string> DCTableWidget::getDataMapAtCol(const int col)
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

const vector<string> DCTableWidget::getShowItemsAtCol(const int col)
{
	vector<string> result;
	auto rowCount = this->rowCount();
	for (int row = 0; row < rowCount; row++) {
		auto item = this->itemAt(col, row);
		result.push_back(item->text().toStdString());
	}
	return result;
}

const set<string> DCTableWidget::getShowItemsSetAtCol(const int col)
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

void DCTableWidget::updateFilterCondition(int col, vector<string> filters)
{
	if (this->filters.find(col) == this->filters.end())
		this->filters.insert(make_pair(col, filters));
	else
		this->filters.at(col) = filters;
}

void DCTableWidget::disableCellForCol(int col)
{
	auto rowCount = this->rowCount();
	for (int row = 0; row < rowCount; row++) {
		auto item = this->item(row, col);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable) & (~Qt::ItemIsSelectable));
	}
}

void DCTableWidget::onCellClicked(int row, int col)
{
	editRow = row;
	editCol = col;
}

void DCTableWidget::onCellChanged(int row, int col)
{
	if (dataMap.size() > row && dataMap.at(row).size() > col) {
		string oldValue = dataMap.at(row).at(col);
		string newValue = this->item(row, col)->text().toStdString();

		if (delegate != nullptr && editRow != -1 && editCol != -1 && oldValue != newValue) {
			dataMap.at(row).at(col) = newValue;
			delegate->updateTempChanges(tableIndex, editRow, editCol, newValue);
			this->item(row, col)->setBackground(Qt::gray);
		}
	}

}
