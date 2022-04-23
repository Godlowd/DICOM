#include "MainWidget.h"
#include "DCDicomFileModel.h"
#include "DCScopeModel.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include "DCAddNewTagDialog.h"
#include "DCAddNewScopeDialog.h"
#include "dcmtk/dcmjpeg/djencode.h"
#include "dcmtk/dcmjpeg/djrplol.h"
#include "DCImageConvertManager.h"
#include "dcmtk/dcmdata/dcrledrg.h"
#include "dcmtk/dcmjpeg/dipijpeg.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "DCImageManager.h"
#include "CommonDefine.h"
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "FilterWidget.h"
#include <QMouseEvent>
#include <set>
#include <QStringList>

#define PROGRAM_WIDTH 1920
#define PROGRAM_HEIGHT 1080
#define LIST_WIDTH 1500
#define LIST_HEIGHT 280
#define LIST_TOP_MARGIN 30
#define LIST_BOTTOM_MARGIN 10

MainWidget::MainWidget():scopeVector(), tableVec(), fileModelArray(), selectedDicomFile()
{
    setWindowTitle("DICOM");
    this->resize(PROGRAM_WIDTH, PROGRAM_HEIGHT);
	m_filterWidget = nullptr;

	setupMenu();

	DCScopeModel *patientScope = new DCScopeModel(CommonTag::PATIENT_TAGS);
	scopeVector.push_back(patientScope);
	patientTable = new DCTabelWidget(this, patientScope->getTableHeaderLabels(), false);
	patientTable->move(0, LIST_TOP_MARGIN);
	patientTable->resize(LIST_WIDTH, LIST_HEIGHT);
	tableVec.push_back(patientTable);
	connect(patientTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionChoose(int)));
	connect(patientTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onPatientHeaderClicked(int)));

	DCScopeModel *studyScope = new DCScopeModel(CommonTag::STUDY_TAGS);
	scopeVector.push_back(studyScope);
	studyTable = new DCTabelWidget(this, studyScope->getTableHeaderLabels(), false);
	studyTable->move(0, LIST_TOP_MARGIN + patientTable->height() + LIST_BOTTOM_MARGIN + LIST_TOP_MARGIN );
	studyTable->resize(LIST_WIDTH, LIST_HEIGHT);
	tableVec.push_back(studyTable);
	connect(studyTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionChoose(int)));
	connect(studyTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onStudyHeaderClicked(int)));

	DCScopeModel *seriesScope = new DCScopeModel(CommonTag::SERIES_TAGS);
	scopeVector.push_back(seriesScope);
	seriesTable = new DCTabelWidget(this, seriesScope->getTableHeaderLabels());
	seriesTable->move(0, LIST_TOP_MARGIN + patientTable->height() + LIST_BOTTOM_MARGIN + LIST_TOP_MARGIN + studyTable->height() + LIST_BOTTOM_MARGIN + LIST_TOP_MARGIN);
	seriesTable->resize(LIST_WIDTH, LIST_HEIGHT);
	tableVec.push_back(seriesTable);
	connect(seriesTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionChoose(int)));
	connect(seriesTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onSeriesHeaderClicked(int)));
}

void MainWidget::openFile()
{
	//QString filePath = QFileDialog::getOpenFileName(
	//	this, 
	//	tr("choose a .dcm file"), 
	//	reader->getPath() != "" ? QString::fromStdString(reader->getPath()) : "C:/", 
	//	tr("DICOM(*.dcm)"));

	//if (!filePath.isEmpty()) {
	//	reader->readFromFile(filePath.toStdString(), fileModel);
	//	refresh();
	//}
	//else {
	//	return;
	//}

	//QString filePath = QFileDialog::getOpenFileName(
	//	this,
	//	tr("choose a dat file"),
	//	reader->getPath() != "" ? QString::fromStdString(reader->getPath()) : "C:/",
	//	tr("DICOM(*.dcm)"));

	//if (!filePath.isEmpty()) {
	//	RawDataReader *rawReader = new RawDataReader(160, 160, 210);
	//	rawReader->readRawDataFromFile(filePath.toStdString().c_str(), "float");
	//}
	//else {
	//	return;
	//}
}

//void MainWidget::removeTag()
//{
//	currentModel->removeTag(tagList->selectedRow);
//	refresh();
//}


//void MainWidget::refresh() {
//	if (fileModel != nullptr) {
//		scopeVector = DCDBManager::getInstance().loadAllScope();
//
//		currentModel = scopeVector.at(0);
//
//		for (auto scope : scopeVector) {
//			scope->loadDetailInfo(fileModel);
//		}
//		genSeriesData(scopeVector);
//
//		DCDetailInfoModel *model = new DCDetailInfoModel(currentModel->getDetailInfoArray());
//		tagList->setModel(model);
//		delegate = new DCDetailInfoItemDelegate(model);
//		tagList->setItemDelegate(delegate);
//	}
//}

//void MainWidget::onClickConfirmBtn(int group, int element)
//{
//	DcmTagKey key = DcmTagKey(group, element);
//	currentModel->addNewTag(key);
//	refresh();
//	return;
//}

void MainWidget::compressImg(std::string newFilePath)
{
	DJEncoderRegistration::registerCodecs(); // register JPEG codecs
	
	std::shared_ptr<DcmFileFormat> fileformat = selectedDicomFile.getFileFormat();
	auto dataset = fileformat->getDataset();
		DJ_RPLossless params; // codec parameters, we use the defaults

		// this causes the lossless JPEG version of the dataset to be created
		if (dataset->chooseRepresentation(EXS_JPEGProcess14SV1, &params).good() &&
			dataset->canWriteXfer(EXS_JPEGProcess14SV1))
		{
			// store in lossless JPEG format
			fileformat->saveFile(newFilePath.c_str(), EXS_JPEGProcess14SV1);
		}
	
	DJEncoderRegistration::cleanup(); // deregister JPEG codecs
}

void MainWidget::decompressImg(std::string newFilePath)
{
	DJEncoderRegistration::registerCodecs(); // register JPEG codecs

	std::shared_ptr<DcmFileFormat> fileformat = selectedDicomFile.getFileFormat();
	auto dataset = fileformat->getDataset();
	DJ_RPLossless params; // codec parameters, we use the defaults

	// this causes the lossless JPEG version of the dataset to be created
	if (dataset->chooseRepresentation(EXS_LittleEndianExplicit, &params).good() &&
		dataset->canWriteXfer(EXS_LittleEndianExplicit))
	{
		// store in lossless JPEG format
		fileformat->saveFile(newFilePath.c_str(), EXS_LittleEndianExplicit);
	}

	DJEncoderRegistration::cleanup(); // deregister JPEG codecs
}

void MainWidget::convertImgToJpeg()
{
	DcmRLEDecoderRegistration::registerCodecs(); // 寄存器RLE解压编解码器
	DJDecoderRegistration::registerCodecs(EDC_photometricInterpretation); // 注册JPEG解压缩编解码器
	DCImageConvertManager *convertManager = new DCImageConvertManager();
	bool result = convertManager->convertToBMP("CT000000_jpg.dcm", "2.jpg");
	if (result) {

	}
	else {

	}
	DcmRLEDecoderRegistration::cleanup(); // 注销RLE解压缩编解码器
	DJDecoderRegistration::cleanup(); // 注销JPEG解压缩编解码器
}

void MainWidget::saveCompressedFile() {
	QString path = QFileDialog::getSaveFileName(this, "save", "./", "DICOM(*.dcm)");
	if (!path.isEmpty()) {
		this->compressImg(path.toStdString());
	}
}

void MainWidget::saveDecompressedFile() {
	QString path = QFileDialog::getSaveFileName(this, "save", "./", "DICOM(*.dcm)");
	if (!path.isEmpty()) {
		this->decompressImg(path.toStdString());
	}
}

void MainWidget::readFileinFolder()
{
	QString dirStr = QFileDialog::getExistingDirectory(this);
	if (!dirStr.isEmpty()) {
		lastPath = dirStr.toStdString();
		QDir dir = QDir(dirStr);
		QStringList filter;
		filter << "*.dcm";
		dir.setNameFilters(filter);
		QList<QFileInfo> fileInfo = QList<QFileInfo>(dir.entryInfoList(filter));

		fileModelArray.clear();
		foreach(QFileInfo info, fileInfo) {
			DCDicomFileModel model = DCDicomFileModel(info.absoluteFilePath().toStdString());
			fileModelArray.push_back(model);
		}

		updateView(fileModelArray);
	}
}
void MainWidget::updateView(std::vector<DCDicomFileModel> fileArray) {
	if (scopeVector.size() != tableVec.size())
		return;

	for (int index = 0; index < scopeVector.size(); index++) {
		auto scope = scopeVector.at(index);
		auto table = tableVec.at(index);
		scope->loadAllData(fileArray);
		table->updateTable(scope->tagValueArray);
	}

}

void MainWidget::filterTable() {
	if (tableVec.size() != filterVec.size())
		return;

	std::set<int> rowSet;
	for (int index = 0; index < tableVec.size(); index++) {
		auto table = tableVec.at(index);
		auto  = filterVec.at(index);

		std::vector<int> correctRows = table->filter();
		std::copy(correctRows.begin(), correctRows.end(), std::inserter(rowSet, rowSet.end()));
	}

	std::vector<DCDicomFileModel> copyFileArray(fileModelArray);
	std::set<int>::iterator iter;
	for (iter = rowSet.begin(); iter != rowSet.end(); iter++) {
		int pos = *iter;
		copyFileArray.erase(copyFileArray.begin() + pos);
	}
	updateView(copyFileArray);
}

void MainWidget::sectionChoose(int index)
{
	for each (DCTabelWidget * table in tableVec)
	{
		table->setSelectionBehavior(QAbstractItemView::SelectRows);
		table->setCurrentCell(index, QItemSelectionModel::Select);
	}

	if (isFiltered())
		selectedDicomFile = filteredModelArray.at(index);
	else 
		selectedDicomFile = fileModelArray.at(index);
	
}

void MainWidget::onPatientHeaderClicked(int row) {
	selectedTable = patientTable;
	onHorizontalClicked(row, patientTable);
}

void MainWidget::onStudyHeaderClicked(int row)
{
	selectedTable = studyTable;
	onHorizontalClicked(row, studyTable);
}

void MainWidget::onSeriesHeaderClicked(int row)
{
	selectedTable = seriesTable;
	onHorizontalClicked(row, seriesTable);
}

void MainWidget::onHorizontalClicked(int col, DCTabelWidget * table)
{
	//所有item
	set<string> items;
	vector<string> dataMap = table->getDataMapAtCol(col);
	std::copy(dataMap.begin(), dataMap.end(), std::inserter(items, items.end()));
	//显示的item
	set<string> showItems(table->getShowItemsSetAtCol(col));
	//转为QStringList
	QStringList strItems;
	QStringList strShowItems;
	for each (string var in items)
	{
		strItems.append(var.c_str());
	}

	for each (string var in showItems)
	{
		strShowItems.append(var.c_str());
	}
	//关闭筛选框
	closeFilterWidget();
	//新建筛选框
	m_filterWidget = new FilterWidget(strItems, strShowItems, col, selectedTable);
	m_filterWidget->delegate = table;

	m_filterWidget->exec(table->mapFromGlobal(cursor().pos()));
}

void MainWidget::updateFilterCondition(set<string> filters)
{

}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
	{
		if (m_filterWidget != nullptr)
			closeFilterWidget();
	}

}

//筛选
void MainWidget::(int col, QStringList showList)
{
	m_map[col] = showList;
	for (int i = 0; i < selectedTable->rowCount(); i++)
	{
		bool hidden = false;

		//遍历map
		for each (int  v_col in m_map.keys())
		{
			QString strItem = selectedTable->item(i, v_col)->text();
			if (!m_map.value(v_col).contains(strItem))
			{
				hidden = true;
				break;
			}
		}

		selectedTable->setRowHidden(i, hidden);
	}
}


//关闭筛选框
void MainWidget::closeFilterWidget()
{
	//清空筛选框
	if (m_filterWidget != nullptr)
	{
		delete m_filterWidget;
		m_filterWidget = nullptr;
	}
}

void MainWidget::setupMenu(){
	QMenuBar *menuBar = new QMenuBar(this);

	QMenu *openMenu = new QMenu("Open", menuBar);
	QAction *openAction = new QAction("open folder");
	connect(openAction, SIGNAL(triggered()), this, SLOT(readFileinFolder()));
	openMenu->addAction(openAction);

	QMenu *imageMenu = new QMenu("Image Process", menuBar);
	QAction *compressAction = new QAction("Compress");
	QAction *deCompressAction = new QAction("Decompress");
	connect(compressAction, SIGNAL(triggered()), this, SLOT(saveCompressedFile()));
	connect(deCompressAction, SIGNAL(triggered()), this, SLOT(saveDecompressedFile()));
	imageMenu->addAction(compressAction);
	imageMenu->addAction(deCompressAction);

	menuBar->addMenu(openMenu);
	menuBar->addMenu(imageMenu);
}

bool MainWidget::isFiltered() {
	return false;
}
