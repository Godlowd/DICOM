#pragma once
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QCheckBox>
#include <QSet>
#include "DCFilterWidgetProtocol.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

enum FilterTable
{
	PatientTable,
	StudyTable,
	SeriesTable
};

class FilterWidget :
	public QWidget
{
public:
	FilterWidget(QStringList Items, QStringList showItems, int col, QWidget *parent = 0);
	~FilterWidget();


	//获取要显示的列表
	vector<string> getShowList();

	//获取列号
	int getColumn();

	//显示
	void exec(const QPoint &pos);

	DCFilterWidgetProtocol *delegate;

	FilterTable filterTable;
private slots:
	//勾选状态更改
	void slot_stateChanged();

private:

	QListWidget* m_ListWidget = nullptr;

	//要显示的项
	vector<string> m_showList;

	//列号
	int m_col;


};