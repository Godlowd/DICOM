#include <QStandardItemModel>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QStringList>
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <qpieslice.h>
#include <QPieSlice>
#include <QtCharts>
#include <QLabel>

#include "dcmtk/dcmjpeg/djencode.h"
#include "dcmtk/dcmjpeg/djrplol.h"
#include "dcmtk/dcmdata/dcrledrg.h"
#include "dcmtk/dcmjpeg/dipijpeg.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmjpeg/dipijpeg.h"

#include "MainWidget.h"
#include "DCDicomFileModel.h"
#include "DCScopeModel.h"
#include "DCAddNewTagDialog.h"
#include "DCAddNewScopeDialog.h"
#include "DCImageConvertManager.h"
#include "DCImageManager.h"
#include "CommonDefine.h"
#include "FilterWidget.h"
#include <set>
#include "DCAllTagTable.h"
#include "DCExcelReader.h"
#include "DCJsonExporter.h"
#include "DCJsonImporter.h"
#include "DCXMLWriter.h"
#include "DCXMLReader.h"

#define PROGRAM_WIDTH 1920
#define PROGRAM_HEIGHT 1080

#define LIST_TOP_MARGIN 30
#define LIST_BOTTOM_MARGIN 10

MainWidget::MainWidget():scopeVector(), tableVec(), fileModelArray(), selectedRow(-1)
{
    setWindowTitle("DICOM");
    this->resize(PROGRAM_WIDTH, PROGRAM_HEIGHT);
	m_filterWidget = nullptr;

	setupMenu();

	setupAbbrePic();

	DCScopeModel *patientScope = new DCScopeModel(CommonTag::PATIENT_TAGS);
	scopeVector.push_back(patientScope);
	patientTable = new DCTableWidget(this, patientScope->getTableHeaderLabels(), 0, this, false);
	patientTable->move(0, LIST_TOP_MARGIN);
	tableVec.push_back(patientTable);
	QObject::connect(patientTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onPatientHeaderClicked(int)));

	DCScopeModel *studyScope = new DCScopeModel(CommonTag::STUDY_TAGS);
	scopeVector.push_back(studyScope);
	studyTable = new DCTableWidget(this, studyScope->getTableHeaderLabels(), 1, this, false);
	studyTable->move(0, LIST_TOP_MARGIN + patientTable->height() + LIST_BOTTOM_MARGIN + LIST_TOP_MARGIN );
	tableVec.push_back(studyTable);
	QObject::connect(studyTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onStudyHeaderClicked(int)));

	DCScopeModel *seriesScope = new DCScopeModel(CommonTag::SERIES_TAGS);
	scopeVector.push_back(seriesScope);
	seriesTable = new DCTableWidget(this, seriesScope->getTableHeaderLabels(), 2, this);
	seriesTable->move(0, LIST_TOP_MARGIN + patientTable->height() + LIST_BOTTOM_MARGIN + LIST_TOP_MARGIN + studyTable->height() + LIST_BOTTOM_MARGIN + LIST_TOP_MARGIN);
	tableVec.push_back(seriesTable);
	QObject::connect(seriesTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onSeriesHeaderClicked(int)));
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
void MainWidget::setupAbbrePic() {
	abbrePictureLabel = new QLabel(this);
	abbrePictureLabel->resize(800, 600);
	abbrePictureLabel->move(1000, 20);
}

void MainWidget::compressImg(std::string newFilePath)
{
	if (nullptr == selectedDicomFile())
		return;

	DJEncoderRegistration::registerCodecs(); // register JPEG codecs
	
	std::shared_ptr<DcmFileFormat> fileformat = selectedDicomFile()->getFileFormat();
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
	if (nullptr == selectedDicomFile())
		return;

	DJEncoderRegistration::registerCodecs(); // register JPEG codecs

	std::shared_ptr<DcmFileFormat> fileformat = selectedDicomFile()->getFileFormat();
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

void MainWidget::convertImgToJpeg(std::string originFilePath, std::string newFilePath)
{
	DcmRLEDecoderRegistration::registerCodecs(); // 寄存器RLE解压编解码器
	DJDecoderRegistration::registerCodecs(EDC_photometricInterpretation); // 注册JPEG解压缩编解码器
	DCImageConvertManager *convertManager = new DCImageConvertManager();
	bool result = convertManager->convertToBMP(originFilePath, newFilePath);
	if (result) {

	}
	else {

	}
	DcmRLEDecoderRegistration::cleanup(); // 注销RLE解压缩编解码器
	DJDecoderRegistration::cleanup(); // 注销JPEG解压缩编解码器
}

void MainWidget::selectFileToConvert() {
	QString filePath = QFileDialog::getOpenFileName(
		this,
		tr("choose a .dcm file"),
		"C:/",
		tr("DICOM(*.dcm)"));
	if (!filePath.isEmpty()) {
		QString path = QFileDialog::getSaveFileName(this, "save", "./", "JPEG(*.jpeg)");
		if (!path.isEmpty()) {
			this->convertImgToJpeg(filePath.toStdString(), path.toStdString());
		}
	}

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
			DCDicomFileModel *model = new DCDicomFileModel(info.absoluteFilePath().toStdString());
			fileModelArray.push_back(model);
		}

		updateView(fileModelArray);
	}
}

void MainWidget::updateView(std::vector<DCDicomFileModel *> fileArray, bool isFiltering) {
	if (scopeVector.size() != tableVec.size())
		return;

	for (int index = 0; index < scopeVector.size(); index++) {
		auto scope = scopeVector.at(index);
		auto table = tableVec.at(index);
		table->isFiltering = isFiltering;
		scope->loadAllData(fileArray);
		table->updateTable(scope->tagValueArray);
	}

}

void MainWidget::filterTable() {
	std::set<int> rowSet;
	for (int index = 0; index < tableVec.size(); index++) {
		auto table = tableVec.at(index);

		std::vector<int> correctRows = table->filter();
		if (index == 0)
			std::copy(correctRows.begin(), correctRows.end(), std::inserter(rowSet, rowSet.end()));
		else {
			set<int> tempSet;
			std::copy(correctRows.begin(), correctRows.end(), std::inserter(tempSet, tempSet.end()));
			set<int> tempInterSet;
			set_intersection(rowSet.begin(), rowSet.end(), tempSet.begin(), tempSet.end(), inserter(tempInterSet, tempInterSet.begin()));
			rowSet = tempInterSet;
		}
	}

	std::vector<DCDicomFileModel *> copyFileArray;
	std::set<int>::iterator iter;
	for (int index = 0; index < fileModelArray.size(); index++) {
		if (rowSet.count(index))
			copyFileArray.push_back(fileModelArray.at(index));
	}
	updateView(copyFileArray, true);
}

void MainWidget::sectionChoose(int index)
{
	for each (DCTableWidget * table in tableVec)
	{
		table->setSelectionBehavior(QAbstractItemView::SelectRows);
		table->setCurrentCell(index, QItemSelectionModel::Select);
	}

	if (!isFiltered())
		updateAbbrePicture(fileModelArray.at(index));
	else
		updateAbbrePicture(filteredModelArray.at(index));
	
	selectedRow = index;
}

DCDicomFileModel * MainWidget::selectedDicomFile() {
	if (selectedRow != -1) {
		if (isFiltered())
			return filteredModelArray.at(selectedRow);
		else
			return fileModelArray.at(selectedRow);
	}

	return nullptr;
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

void MainWidget::updateTempChanges(int tableIndex, int row, int col, string newValue)
{
	DCTableWidget *table = tableVec.at(tableIndex);
	DCScopeModel *scope = scopeVector.at(tableIndex);
	selectedRow = row;
	DcmTagKey tagKey = scope->getTagInfoArray().at(col);

	selectedDicomFile()->updateTempChange(tagKey, newValue);
}

void MainWidget::onHorizontalClicked(int col, DCTableWidget * table)
{
	//所有item
	set<string> items;
	vector<string> dataMap = table->getDataMapAtCol(col);
	map<string, int> timesCount;
	for (auto data : dataMap) {
		auto iter = timesCount.find(data);
		if (iter != timesCount.end()) {
			iter->second += 1;
		}
		else {
			timesCount[data] = 1;
		}
	}
	updatePieChart(timesCount);

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
	m_filterWidget = new FilterWidget(strItems, strShowItems, col, table);
	m_filterWidget->delegate = table;

	QPoint pos(cursor().pos());
	pos.setX(pos.x() + 10);
	m_filterWidget->exec(table->mapFromGlobal(pos));
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
void MainWidget::filter(int col, QStringList showList)
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

void MainWidget::saveAction() {
	applyChangesToFile(selectedDicomFile());
}

void MainWidget::saveAsAction() {
	QString path = QFileDialog::getSaveFileName(this, "save", "./", "DICOM(*.dcm)");
	if (!path.isEmpty()) {
		applyChangesToFile(selectedDicomFile(), path.toStdString());
	}
}

void MainWidget::importAction()
{
	DCJsonImporter importer;

	QString filePath = QFileDialog::getOpenFileName(
		this,
		tr("choose a midware file"),
		"C:/",
		tr("DICOM(*.json *.xml)"));
	QString rawDataPath = QFileDialog::getOpenFileName(
		this,
		tr("choose a raw data file"),
		"C:/",
		tr("DICOM(*.dat)"));

	if (!filePath.isEmpty()) {
		vector<DCDicomFileModel *> fileModel;
		if (QFileInfo(filePath).suffix() == "xml") {
			DCXMLReader xmlReader;
			fileModel = xmlReader.genDcmFromXML(filePath.toStdString());
		}
		else if (QFileInfo(filePath).suffix() == "json") {
			if (!rawDataPath.isEmpty())
				fileModel = importer.genDcmFromJson(filePath.toStdString(), rawDataPath.toStdString());
			else
				fileModel = importer.genDcmFromJson(filePath.toStdString());
			fileModelArray.insert(fileModelArray.end(), fileModel.begin(), fileModel.end());
		}
		

		updateView(fileModelArray);
	}
}

bool MainWidget::applyChangesToFile(DCDicomFileModel *filemodel, string newFileName) {
	auto status = filemodel->applyChanges(newFileName);
	if (status) {
		for each (auto table in tableVec)
		{
			
			auto colCount = table->columnCount();
			for (int col = 0; col < colCount; col++) {
				auto item = table->item(selectedRow, col);
				item->setBackground(QBrush(QColor(255, 255, 255)));
			}
		}
	}

	return status;
}

void MainWidget::showAllTag() {
	DCExcelReader excelReader;
	vector<vector<string>> tagData = excelReader.loadCSV("DicomTag.csv");
	vector<DcmTagKey *> tagVec;
	vector<string> tagNameArray;
	for each (auto valueVec in tagData)
	{
		auto groupId = valueVec.at(0).substr(2, 6);
		auto elementId = valueVec.at(1).substr(0, 4);

		int group, element;
		stringstream ss;
		ss << std::hex << groupId;  
		ss >> group;

		stringstream sss;
		sss << std::hex << elementId;
		sss >> element;

		valueVec.at(0) = to_string(group);
		valueVec.at(1) = to_string(element);
		auto tagName = valueVec.at(2);
		DcmTagKey *tagKey = new DcmTagKey(group, element);
		tagVec.push_back(tagKey);
		tagNameArray.push_back(tagName);
	}

	vector<string> headerName = { "Tag", "Type", "Value", "Name" };
	DCAllTagTable *allTagTable = new DCAllTagTable(this, headerName, 3, selectedDicomFile());
	allTagTable->setTagArray(tagVec);
	allTagTable->setTagNameArray(tagNameArray);
	allTagTable->loadAllTag(selectedDicomFile());
	QDesktopWidget *desktop = QApplication::desktop();
	allTagTable->move((desktop->width() - allTagTable->width()) / 2, (desktop->height() - allTagTable->height()) / 2);
	allTagTable->show();

	DCJsonExporter *exporter = new DCJsonExporter(tagVec);
	vector<DCDicomFileModel *> model;
	model.push_back(selectedDicomFile());
	exporter->exportDcmAsJson("testForW.json", model);

	DCXMLWriter *xmlWriter = new DCXMLWriter(tagVec);
	xmlWriter->genXMLFromFileModel(model, "test.xml");
}

void MainWidget::setupMenu(){
	QMenuBar *menuBar = new QMenuBar(this);

	QMenu *openMenu = new QMenu(QString::fromLocal8Bit("文件"), menuBar);
	QAction *openAction = new QAction(QString::fromLocal8Bit("打开文件夹"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(readFileinFolder()));
	openMenu->addAction(openAction);
	QAction *importAction = new QAction(QString::fromLocal8Bit("导入"));
	connect(importAction, SIGNAL(triggered()), this, SLOT(importAction()));
	openMenu->addAction(importAction);
	QAction *saveAction = new QAction(QString::fromLocal8Bit("保存"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveAction()));
	openMenu->addAction(saveAction);
	QAction *saveAsAction = new QAction(QString::fromLocal8Bit("另存为"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsAction()));
	openMenu->addAction(saveAsAction);
	QAction *filterAction = new QAction("Filter");
	connect(filterAction, SIGNAL(triggered()), this, SLOT(filterTable()));
	openMenu->addAction(filterAction);
	QAction *showAllTagAction = new QAction("DICOM");
	connect(showAllTagAction, SIGNAL(triggered()), this, SLOT(showAllTag()));
	openMenu->addAction(showAllTagAction);

	QMenu *imageMenu = new QMenu("Image Process", menuBar);
	QAction *compressAction = new QAction("Compress");
	QAction *deCompressAction = new QAction("Decompress");
	QAction *convertToJpegAction = new QAction("Convert to JPEG");
	connect(compressAction, SIGNAL(triggered()), this, SLOT(saveCompressedFile()));
	connect(deCompressAction, SIGNAL(triggered()), this, SLOT(saveDecompressedFile()));
	connect(convertToJpegAction, SIGNAL(triggered()), this, SLOT(selectFileToConvert()));
	imageMenu->addAction(compressAction);
	imageMenu->addAction(deCompressAction);
	imageMenu->addAction(convertToJpegAction);

	menuBar->addMenu(openMenu);
	menuBar->addMenu(imageMenu);
}

bool MainWidget::isFiltered() {
	return false;
}

void MainWidget::exporter() {
	
}

void MainWidget::updatePieChart(map<string, int> mapData) {
	auto totalCount = fileModelArray.size();
	QPieSeries *series = new QPieSeries(this);
	for (auto pair : mapData) {
		QPieSlice *slice = new QPieSlice(pair.first.c_str(), pair.second, this);
		slice->setLabelVisible(true);
		series->append(slice);
	}

	QChart *chart = new QChart();
	chart->addSeries(series);

	QChartView *chartview = new QChartView(this);
	chartview->show();
	chartview->setChart(chart);
	chartview->move(1000, 500);
	chartview->resize(600, 400);
}

void MainWidget::updateAbbrePicture(DCDicomFileModel *file) {
	DicomImage *dicomImg = new DicomImage(file->getFileFormat()->getDataset(), EXS_LittleEndianImplicit);
	double winWidth, winCenter;
	file->getFileFormat()->getDataset()->findAndGetFloat64(DCM_WindowWidth, winWidth);
	file->getFileFormat()->getDataset()->findAndGetFloat64(DCM_WindowCenter, winCenter);
	dicomImg->setWindow(winCenter, winWidth);

	Uint16 bitsAllocated;
	file->getFileFormat()->getDataset()->findAndGetUint16(DCM_BitsAllocated, bitsAllocated);
	Uint16 *pixelData = (Uint16*)(dicomImg->getOutputData(bitsAllocated));

	QPixmap pixmap;
	void *pDIB = nullptr;
	int size = 0;
	bool res;
	if (dicomImg->isMonochrome())
	{
		// 灰度图像
		size = dicomImg->createWindowsDIB(pDIB, 0, 0, 8, 1, 1);
		if (!pDIB)
			return ;

		res = ucharArrayToPixmap((uchar *)pDIB, dicomImg->getWidth(), dicomImg->getHeight(), size, pixmap, 8);
	}
	else
	{
		// RGB图像
		size = dicomImg->createWindowsDIB(pDIB, 0, 0, 24, 1, 1);
		if (!pDIB)
			return ;

		res = ucharArrayToPixmap((uchar *)pDIB, dicomImg->getWidth(), dicomImg->getHeight(), size, pixmap, bitsAllocated);
	}
	delete pDIB;
	abbrePictureLabel->setPixmap(pixmap);
}

bool MainWidget::ucharArrayToPixmap(uchar *data, int w, int h, int bitSize, QPixmap & pixmap, int biBitCount)
{
	//位图文件由四部分依序组成：BITMAPFILEHEADER,BITMAPINFOHEADER,调色板,Image Data。
	BITMAPFILEHEADER lpfh;// 文件头  固定的14个字节, 描述文件的有关信息
	BITMAPINFOHEADER lpih;// 固定的40个字节，描述图像的有关信息

	RGBQUAD palette[256];// 调色板RGBQUAD的大小就是256
	memset(palette, 0, sizeof(palette));
	for (int i = 0; i < 256; ++i) {
		palette[i].rgbBlue = i;
		palette[i].rgbGreen = i;
		palette[i].rgbRed = i;
	}

	memset(&lpfh, 0, sizeof(BITMAPFILEHEADER));
	lpfh.bfType = 0x4d42;//'B''M' must be 0x4D42.

	//the sum bits of BITMAPFILEHEADER,BITMAPINFOHEADER and RGBQUAD;the index byte of the image data.
	lpfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(palette);

	memset(&lpih, 0, sizeof(BITMAPINFOHEADER));
	lpih.biSize = sizeof(BITMAPINFOHEADER); //the size of this struct. it is 40 bytes.
	lpih.biWidth = w;
	lpih.biHeight = h;
	lpih.biCompression = BI_RGB;
	lpih.biPlanes = 1; //must be 1. 

	void *pDIB = data;
	int size = bitSize;
	lpih.biBitCount = biBitCount;

	//the size of the whole bitmap file.
	lpfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(palette) + size;

	QByteArray bmp;
	bmp.append((char*)&lpfh, sizeof(BITMAPFILEHEADER));
	bmp.append((char*)&lpih, sizeof(BITMAPINFOHEADER));
	bmp.append((char*)palette, sizeof(palette));
	bmp.append((char*)pDIB, size);

	return pixmap.loadFromData(bmp);
}

