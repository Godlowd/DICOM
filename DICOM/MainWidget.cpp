#include "MainWidget.h"
#include <QDebug>
#include "DCDetailInfoModel.h"
#include "DCListView.h"
#include "DCDetailInfoItemDelegate.h"
#include "DCReader.h"
#include "DCDicomFileModel.h"
#include "DCScopeModel.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include "DCAddNewTagDialog.h"
#include "DCAddNewScopeDialog.h"
#include "DCDBManager.h"

#define SERIES_LIST_WIDTH 150
#define SERIES_LIST_HEIGHT 500
#define SERIES_ORIGIN_X 16
#define SERIES_ORIGIN_Y 0
MainWidget::MainWidget():seriesVec()
{
    setWindowTitle("DICOM");
    this->resize(1500,800);
	// filemodel
	fileModel = new DCDicomFileModel();

	// reader
	reader = new DCReader();
	/*reader->readFromFile("image-00000.dcm", fileModel);*/
	reader->readFromFile("111.dcm", fileModel);

	seriesListView = new DCListView(this);
	seriesListView->move(SERIES_ORIGIN_X, SERIES_ORIGIN_Y);
	seriesListView->resize(SERIES_LIST_WIDTH, SERIES_LIST_HEIGHT);
	connect(seriesListView, SIGNAL(clicked(QModelIndex)), this, SLOT(updateTagList(QModelIndex)));

	// test data generation
	scopeVector = DCDBManager::getInstance().loadAllScope();

	currentModel = scopeVector.at(0);
	tagList = new DCListView(this);
	tagList->move(SERIES_ORIGIN_X + SERIES_LIST_WIDTH + 30, SERIES_ORIGIN_Y);
	tagList->resize(700, 500);
	connect(tagList, SIGNAL(clicked(QModelIndex)), this, SLOT(selectTagAt(QModelIndex)));

	connect(tagList, SIGNAL(clicked(QModelIndex)), this, SLOT(ItemClicked(QModelIndex)));

	// add new series button
	QPushButton *addNewSeriesBtn = new QPushButton("open file", this);
	addNewSeriesBtn->move(SERIES_ORIGIN_X, SERIES_ORIGIN_Y + SERIES_LIST_HEIGHT + 30);
	addNewSeriesBtn->resize(100, 40);
	addNewSeriesBtn->show();
	QObject::connect(addNewSeriesBtn, SIGNAL(clicked()), this, SLOT(openFile()));

	// add new tag button
	QPushButton *addNewTagBtn = new QPushButton("Add Tag", this);
	addNewTagBtn->move(SERIES_ORIGIN_X + 200, SERIES_ORIGIN_Y + SERIES_LIST_HEIGHT + 30);
	addNewTagBtn->adjustSize();
	addNewTagBtn->show();
	QObject::connect(addNewTagBtn, SIGNAL(clicked()), this, SLOT(showAddNewTagDialog()));

	// remove tag button
	QPushButton *removeTagBtn = new QPushButton("Remove Tag", this);
	removeTagBtn->move(SERIES_ORIGIN_X + 200 + 200, SERIES_ORIGIN_Y + SERIES_LIST_HEIGHT + 30);
	removeTagBtn->adjustSize();
	removeTagBtn->show();
	QObject::connect(removeTagBtn, SIGNAL(clicked()), this, SLOT(removeTag()));

	// remove all tables button
	QPushButton *removeAllTableBtn = new QPushButton("DELETE TABLE", this);
	removeAllTableBtn->move(SERIES_ORIGIN_X + 200 + 200 + 200, SERIES_ORIGIN_Y + SERIES_LIST_HEIGHT + 30);
	removeAllTableBtn->adjustSize();
	removeAllTableBtn->show();
	QObject::connect(removeAllTableBtn, SIGNAL(clicked()), this, SLOT(removeAllTables()));

	// refresh ui button
	QPushButton *refreshBtn = new QPushButton("REFRESH", this);
	refreshBtn->move(SERIES_ORIGIN_X + 200 + 200 + 200+200, SERIES_ORIGIN_Y + SERIES_LIST_HEIGHT + 30);
	refreshBtn->adjustSize();
	refreshBtn->show();
	QObject::connect(refreshBtn, SIGNAL(clicked()), this, SLOT(refresh()));

	// add new scope button
	QPushButton *newScopeBtn = new QPushButton("ADD NEW SCOPE", this);
	newScopeBtn->move(SERIES_ORIGIN_X + 200 + 200 + 200 + 200+ 200, SERIES_ORIGIN_Y + SERIES_LIST_HEIGHT + 30);
	newScopeBtn->adjustSize();
	newScopeBtn->show();
	QObject::connect(newScopeBtn, SIGNAL(clicked()), this, SLOT(addNewScope()));

	// remove scope button
	QPushButton *removeScopeBtn = new QPushButton("REMOVE SCOPE", this);
	removeScopeBtn->move(SERIES_ORIGIN_X + 200 + 200 + 200 + 200 + 200 +200, SERIES_ORIGIN_Y + SERIES_LIST_HEIGHT + 30);
	removeScopeBtn->adjustSize();
	removeScopeBtn->show();
	QObject::connect(removeScopeBtn, SIGNAL(clicked()), this, SLOT(removeScope()));
	refresh();
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
	currentModel = scope;
}

void MainWidget::selectTagAt(QModelIndex index)
{
	tagList->selectedRow = index.row();
}

void MainWidget::genSeriesData(std::vector<DCScopeModel *> scopeArray) {
	QStandardItemModel *model = new QStandardItemModel();
	for (auto scope : scopeArray) {
		QStandardItem *item = new QStandardItem(QString::fromStdString(scope->getName()));
		model->appendRow(item);
	}
	seriesListView->setModel(model);
}

void MainWidget::openFile()
{
	QString filePath = QFileDialog::getOpenFileName(
		this, 
		tr("choose a .dcm file"), 
		reader->getPath() != "" ? QString::fromStdString(reader->getPath()) : "C:/", 
		tr("DICOM(*.dcm)"));

	if (!filePath.isEmpty()) {
		reader->readFromFile(filePath.toStdString(), fileModel);
		refresh();
	}
	else {
		return;
	}
}

void MainWidget::showAddNewTagDialog() {
	DCAddNewTagDialog *dialog = new DCAddNewTagDialog(this, this);
	dialog->show();
}

void MainWidget::removeTag()
{
	currentModel->removeTag(tagList->selectedRow);
	refresh();
}

void MainWidget::removeAllTables()
{
	DCDBManager::getInstance().deleteAllTables();
}

void MainWidget::refresh() {
	if (fileModel != nullptr) {
		scopeVector = DCDBManager::getInstance().loadAllScope();

		currentModel = scopeVector.at(0);

		for (auto scope : scopeVector) {
			scope->loadDetailInfo(fileModel);
		}
		genSeriesData(scopeVector);

		DCDetailInfoModel *model = new DCDetailInfoModel(currentModel->getDetailInfoArray());
		tagList->setModel(model);
		delegate = new DCDetailInfoItemDelegate(model);
		tagList->setItemDelegate(delegate);
	}
}

void MainWidget::onClickConfirmBtn(int group, int element)
{
	DcmTagKey key = DcmTagKey(group, element);
	currentModel->addNewTag(key);
	refresh();
	return;
}

void MainWidget::DCAddNewScopeOnClickConfirmBtn(std::string scopeName)
{
	DCDBManager::getInstance().addNewScope(scopeName);
	refresh();
}

void MainWidget::addNewScope() {
	DCAddNewScopeDialog *dialog = new DCAddNewScopeDialog(this, this);
	dialog->show();
}

void MainWidget::removeScope()
{
	if (DCDBManager::getInstance().removeScope(currentModel->getName())) {
		refresh();
	}
	else {
		std::cout << "Fail to remove Scope: " << currentModel->getName() << std::endl;
	}
}
