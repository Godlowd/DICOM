#include "MainWidget.h"
#include "DCDicomFileModel.h"
#include "DCScopeModel.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include "DCAddNewTagDialog.h"
#include "DCAddNewScopeDialog.h"
#include "RawDataReader.h"
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

#define PROGRAM_WIDTH 1920
#define PROGRAM_HEIGHT 1080
#define LIST_WIDTH 1500
#define LIST_HEIGHT 280
#define LIST_TOP_MARGIN 20
#define LIST_BOTTOM_MARGIN 10

MainWidget::MainWidget():scopeVector(), tableVec(), fileModelArray(), selectedDicomFile()
{
    setWindowTitle("DICOM");
    this->resize(PROGRAM_WIDTH, PROGRAM_HEIGHT);

	setupMenu();
	DCScopeModel *patientScope = new DCScopeModel(CommonTag::PATIENT_TAGS);
	scopeVector.push_back(patientScope);
	DCTabelWidget *patientTable = new DCTabelWidget(this, patientScope->getTableHeaderLabels(), false);
	patientTable->move(0, LIST_TOP_MARGIN);
	patientTable->resize(LIST_WIDTH, LIST_HEIGHT);
	tableVec.push_back(patientTable);
	connect(patientTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionChoose(int)));

	DCScopeModel *studyScope = new DCScopeModel(CommonTag::STUDY_TAGS);
	scopeVector.push_back(studyScope);
	DCTabelWidget *studyTable = new DCTabelWidget(this, studyScope->getTableHeaderLabels(), false);
	studyTable->move(0, LIST_TOP_MARGIN + patientTable->height() + LIST_TOP_MARGIN + LIST_BOTTOM_MARGIN);
	studyTable->resize(LIST_WIDTH, LIST_HEIGHT);
	tableVec.push_back(studyTable);
	connect(studyTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionChoose(int)));

	DCScopeModel *seriesScope = new DCScopeModel(CommonTag::SERIES_TAGS);
	scopeVector.push_back(seriesScope);
	DCTabelWidget *seriesTable = new DCTabelWidget(this, seriesScope->getTableHeaderLabels());
	seriesTable->move(0, LIST_TOP_MARGIN + patientTable->height() + LIST_BOTTOM_MARGIN + LIST_TOP_MARGIN + studyTable->height() + LIST_BOTTOM_MARGIN);
	seriesTable->resize(LIST_WIDTH, LIST_HEIGHT);
	tableVec.push_back(seriesTable);
	connect(seriesTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionChoose(int)));
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

void MainWidget::showAddNewTagDialog() {
	DCAddNewTagDialog *dialog = new DCAddNewTagDialog(this, this);
	dialog->show();
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

void MainWidget::onClickConfirmBtn(int group, int element)
{
}

void MainWidget::DCAddNewScopeOnClickConfirmBtn(std::string scopeName)
{
	//DCDBManager::getInstance().addNewScope(scopeName);
	//refresh();
}

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
	DcmRLEDecoderRegistration::registerCodecs(); // ¼Ä´æÆ÷RLE½âÑ¹±à½âÂëÆ÷
	DJDecoderRegistration::registerCodecs(EDC_photometricInterpretation); // ×¢²áJPEG½âÑ¹Ëõ±à½âÂëÆ÷
	DCImageConvertManager *convertManager = new DCImageConvertManager();
	bool result = convertManager->convertToBMP("CT000000_jpg.dcm", "2.jpg");
	if (result) {

	}
	else {

	}
	DcmRLEDecoderRegistration::cleanup(); // ×¢ÏúRLE½âÑ¹Ëõ±à½âÂëÆ÷
	DJDecoderRegistration::cleanup(); // ×¢ÏúJPEG½âÑ¹Ëõ±à½âÂëÆ÷
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
