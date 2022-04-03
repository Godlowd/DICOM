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

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget();

public slots:
	void ItemClicked(QModelIndex index);
	void updateTagList(QModelIndex index);

	void genSeriesData(std::vector<DCScopeModel*> scopeArray);

	void pushBtnClicked();
private:
	std::vector<std::vector<DcmTagKey>> seriesVec;
	std::vector<DCScopeModel *> scopeVector;
	DCReader *reader;
	DCListView *tagList;
	DCListView *seriesListView;
	DCDicomFileModel *fileModel;
	DCDetailInfoItemDelegate *delegate;
};

#endif // MAINWIDGET_H
