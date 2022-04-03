#include "MainWidget.h"
#include <QDebug>
#include "DCDetailInfoModel.h"
#include "DCListView.h"
#include "DCDetailInfoItemDelegate.h"
#include "DCReader.h"
#include "DCDicomFileModel.h"
#include "DCScopeModel.h"
#include "CommonDefine.h"
#include <QStandardItemModel>

#define SERIES_LIST_WIDTH 150
#define SERIES_LIST_HEIGHT 500
#define SERIES_ORIGIN_X 16
#define SERIES_ORIGIN_Y 0
MainWidget::MainWidget():seriesVec()
{
    setWindowTitle("DICOM");
    this->resize(1920,1080);
	//// filemodel
	//fileModel = new DCDicomFileModel();

	//// reader
	//reader = new DCReader();
	//reader->readFromFile("image-00000.dcm", fileModel);

	//seriesListView = new DCListView(this);
	//seriesListView->move(SERIES_ORIGIN_X, SERIES_ORIGIN_Y);
	//seriesListView->resize(SERIES_LIST_WIDTH, SERIES_LIST_HEIGHT);
	//connect(seriesListView, SIGNAL(clicked(QModelIndex)), this, SLOT(updateTagList(QModelIndex)));

	//// test data generation
	//

	//DCScopeModel *scope = new DCScopeModel(CommonTag::STUDY_TAGS, "STUDY_TAGS");
	//scope->loadDetailInfo(fileModel);
	//scopeVector.push_back(scope);

	//scope = new DCScopeModel(CommonTag::SERIES_TAGS, "SERIES_TAGS");
	//scope->loadDetailInfo(fileModel);
	//scopeVector.push_back(scope);

	//scope = new DCScopeModel(CommonTag::PATIENT_TAGS, "PATIENT_TAGS");
	//scope->loadDetailInfo(fileModel);
	//scopeVector.push_back(scope);

	//genSeriesData(scopeVector);

	//tagList = new DCListView(this);
	//tagList->move(SERIES_ORIGIN_X + SERIES_LIST_WIDTH + 30, SERIES_ORIGIN_Y);
	//tagList->resize(700, 500);
	//
	//DCDetailInfoModel *model = new DCDetailInfoModel(scopeVector.front()->getDetailInfoArray());
	//tagList->setModel(model);

	//delegate = new DCDetailInfoItemDelegate(model);
	//tagList->setItemDelegate(delegate);

	//connect(tagList, SIGNAL(clicked(QModelIndex)), this, SLOT(ItemClicked(QModelIndex)));

	// add new series button
	QPushButton *addNewSeriesBtn = new QPushButton("tet", this);
	addNewSeriesBtn->move(SERIES_ORIGIN_X, SERIES_ORIGIN_Y + SERIES_LIST_HEIGHT + 30);
	addNewSeriesBtn->resize(100, 100);
	addNewSeriesBtn->show();
	QObject::connect(addNewSeriesBtn, SIGNAL(clicked()), this, SLOT(pushBtnClicked()));
}

void MainWidget::ItemClicked(QModelIndex index)
{
	int a;
}

void MainWidget::updateTagList(QModelIndex index) {
	int row = index.row();
	DCScopeModel *scope = scopeVector.at(index.row());
	scope->loadDetailInfo(fileModel);
	DCDetailInfoModel *model = new DCDetailInfoModel(scope->getDetailInfoArray());
	delegate->model = model;
	tagList->setModel(model);  
	 
}

void MainWidget::genSeriesData(std::vector<DCScopeModel *> scopeArray) {
	QStandardItemModel *model = new QStandardItemModel();
	for (auto scope : scopeArray) {
		QStandardItem *item = new QStandardItem(QString::fromStdString(scope->getName()));
		model->appendRow(item);
	}
	seriesListView->setModel(model);
}

void MainWidget::pushBtnClicked()
{
	int a = 0;
}
