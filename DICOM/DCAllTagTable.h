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
	DCAllTagTable::DCAllTagTable(QWidget *parent, vector<string> headerName, int tableIndex, MainWidget * delegate);

	void loadAllTag(DCDicomFileModel *fileModel);

	void setTagArray(vector<DcmTagKey *> tagArray);
	void setTagNameArray(vector<string> tagNameArray);

	void setupTitleLabel();

	void setupMainTable(vector<string> headerName, int tableIndex, MainWidget * delegate);

	void setupCloseBtn();

public slots:
	void onCloseBtnPressed();

private:
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
};

