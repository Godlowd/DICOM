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

	// @brief ���ļ�
	void openFile();

	// @brief ����µ�tag
	void showAddNewTagDialog();
	
	// @brief �Ƴ�ָ��λ�õ�tag
	void removeTag();

	// @brief ������ݿ�
	void removeAllTables();

	// @brief ˢ��seriesList��tagList
	void refresh();

	// @brief ����µ�scope
	void addNewScope();

	// @brief �Ƴ�ѡ�е�scope
	void removeScope();

private:
	std::vector<std::vector<DcmTagKey>> seriesVec;
	std::vector<DCScopeModel *> scopeVector;
	DCReader *reader;
	DCListView *tagList;
	DCListView *seriesListView;
	DCDicomFileModel *fileModel;
	DCDetailInfoItemDelegate *delegate;

	// ��ǰ�б�ѡ���scope
	DCScopeModel *currentModel;

	void onClickConfirmBtn(int group, int element) override;

	void DCAddNewScopeOnClickConfirmBtn(std::string scopeName) override;
};

#endif // MAINWIDGET_H
