#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QFile>
#include <QDataStream>
#include <qwidget.h>
#include "DCDetailInfoStruct.h"
#include "DCDetailInfoModel.h"
#include "DCListView.h"
#include "DCDetailInfoItemDelegate.h"
#include "DCReader.h"
#include "DCDicomFileModel.h"
#include "DCScopeModel.h"
#include "DCAddNewTagProtocal.h"

class MainWidget : public QWidget, public DCAddNewTagProtocol
{
    Q_OBJECT
public:
    MainWidget();

public slots:
	void ItemClicked(QModelIndex index);
	void updateTagList(QModelIndex index);

	void genSeriesData(std::vector<DCScopeModel*> scopeArray);

	// @brief 打开文件
	void openFile();

	// @brief 添加新的tag
	void addNewTag();
	
private:
	std::vector<std::vector<DcmTagKey>> seriesVec;
	std::vector<DCScopeModel *> scopeVector;
	DCReader *reader;
	DCListView *tagList;
	DCListView *seriesListView;
	DCDicomFileModel *fileModel;
	DCDetailInfoItemDelegate *delegate;

	// 当前列表选择的scope
	DCScopeModel *currentModel;

	// @brief 刷新seriesList和tagList
	void refresh();

	void onClickConfirmBtn(int group, int element) override;
};

#endif // MAINWIDGET_H
