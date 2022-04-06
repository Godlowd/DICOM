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
#include "DCAddNewScopeProtocol.h"

class MainWidget : public QWidget, public DCAddNewTagProtocol, public DCAddNewScopeProtocol
{
    Q_OBJECT
public:
    MainWidget();

public slots:
	void ItemClicked(QModelIndex index);
	void updateTagList(QModelIndex index);
	void selectTagAt(QModelIndex index);

	void genSeriesData(std::vector<DCScopeModel*> scopeArray);

	// @brief 打开文件
	void openFile();

	// @brief 添加新的tag
	void showAddNewTagDialog();
	
	// @brief 移除指定位置的tag
	void removeTag();

	// @brief 清空数据库
	void removeAllTables();

	// @brief 刷新seriesList和tagList
	void refresh();

	// @brief 添加新的scope
	void addNewScope();

	// @brief 移除选中的scope
	void removeScope();

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

	void onClickConfirmBtn(int group, int element) override;

	void DCAddNewScopeOnClickConfirmBtn(std::string scopeName) override;
};

#endif // MAINWIDGET_H
