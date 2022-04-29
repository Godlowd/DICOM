#pragma once
#include "DCTableWidget.h"
#include <QWidget>
#include <string>
#include <vector>
using namespace std;
class QPushButton;
class DCAllTagTable :
	public QWidget
{
public:
	DCAllTagTable::DCAllTagTable(QWidget *parent, vector<string> headerName, int tableIndex, DCDicomFileModel *fileModel);

	void loadAllTag(DCDicomFileModel *fileModel);

	void setTagArray(vector<DcmTagKey *> tagArray);
	void setTagNameArray(vector<string> tagNameArray);

	void setupTitleLabel();

	void setupMainTable(vector<string> headerName, int tableIndex, MainWidget * delegate);

	void setupCloseBtn();

	vector<int> filter();

public slots:
	void onCloseBtnPressed();

	void onSectionClicked(int);

private: 

	DCDicomFileModel *fileModel;
	DCTableWidget *mainTable;
	QPushButton *closeBtn;
	/**
	 * 感兴趣的tag列表.
	 */
	vector<DcmTagKey *> tagArray;

	/**
	 * DcmTag getName()返回的方法不一定有，所以统一从外部注入.
	 */
	vector<string> tagNameArray;

	/**
	 * 搜索框输入的筛选条件.
	 */
	vector<string> filterArray;
};

