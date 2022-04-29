#include "DCAllTagTable.h"
#include "DCDicomFileModel.h"
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#define MAIN_WIDTH 930
#define MAIN_HEIGHT 780
#define TABLE_WIDTH 850
#define TABLE_HEIGHT 630
DCAllTagTable::DCAllTagTable(QWidget *parent, vector<string> headerName, int tableIndex, DCDicomFileModel * fileModel):
	QWidget(parent),
	fileModel(fileModel)
{
	resize(MAIN_WIDTH, MAIN_HEIGHT);
	filterArray.resize(headerName.size());
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	setupTitleLabel();
	setupMainTable(headerName, tableIndex, nullptr);
	setupCloseBtn();

	
	QPalette pal(this->palette());
	//设置背景黑色
	pal.setColor(QPalette::Background, Qt::white);
	//pal.setColor(QPalette::Background, QColor(39, 39, 43));
	this->setAutoFillBackground(true);
	this->setPalette(pal);
}

void DCAllTagTable::loadAllTag(DCDicomFileModel *fileModel)
{
	if (tagNameArray.size() != tagArray.size())
		return;

	vector<vector<string>> valueArray;
	for (int row = 0; row < tagArray.size(); row++) {
		auto tagKey = tagArray.at(row);
		DcmTag tag = DcmTag(*tagKey);
		
		auto value = fileModel->getStringForTag(*tagKey);
		auto vrName = tag.getVRName();
		if (vrName == "??")
			vrName = "UN";
		vector<string> valueVec{ tagKey->toString().c_str(), vrName, value , tagNameArray.at(row) };

		valueArray.push_back(valueVec);
	}


	mainTable->updateTable(valueArray, 1);
}

void DCAllTagTable::setTagArray(vector<DcmTagKey*> tagArray)
{
	this->tagArray = tagArray;
}

void DCAllTagTable::setTagNameArray(vector<string> tagNameArray)
{
	this->tagNameArray = tagNameArray;
}

void DCAllTagTable::setupTitleLabel() {
	QLabel *titleLabel = new QLabel(this);
	titleLabel->setText("DICOM");
	QFont font = QFont("Microsoft YaHei", 18, 50);
	titleLabel->setFont(font);
	titleLabel->adjustSize();
	titleLabel->move((MAIN_WIDTH - titleLabel->width()) / 2, 20);
	QPalette pe;
	pe.setColor(QPalette::WindowText, QColor(188, 29, 65));
	titleLabel->setPalette(pe);
}

void DCAllTagTable::setupMainTable(vector<string> headerName, int tableIndex, MainWidget * delegate) {
	mainTable = new DCTableWidget(this, headerName, tableIndex, delegate, false);
	mainTable->resize(TABLE_WIDTH, TABLE_HEIGHT);
	mainTable->move((MAIN_WIDTH - TABLE_WIDTH) / 2, 60);
	mainTable->disableCellForCol({ 0, 1, 3 });
	mainTable->setRowCount(1);
	mainTable->horizontalHeader()->setStyleSheet("QHeaderView::section{ background:rgb(56, 56, 66); }");
	disconnect(mainTable, SIGNAL(sectionClicked(int)), 0, 0);
	disconnect(mainTable, SIGNAL(cellChanged(int, int)), 0, 0);

	connect(mainTable, &QTableWidget::cellChanged, this, [=](int row, int col) {
		// 只关注value列的更改
		if (col != 2)
			return;
		// 因为有一个筛选框的行的偏移
		auto realRow = row - 1;
		auto tagKey = tagArray.at(realRow);
		auto value = mainTable->item(row, col)->text().toStdString();

		if (nullptr != fileModel)
			fileModel->updateTempChange(*tagKey, value);
	});

	//connect(mainTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onSectionClicked(int)));

	for (int col = 0; col < headerName.size(); col++) {
		auto header = headerName.at(col);
		QLineEdit *searchBar = new QLineEdit();
		searchBar->setPlaceholderText(header.c_str());
		connect(searchBar, &QLineEdit::textChanged, this, [=](const QString &text) {
			filterArray.at(col) = text.toStdString();
			vector<int> rows = this->filter();
			// rows 代表的是table的dataMap中的行号，需要加上一个搜索框的偏移量
			mainTable->showRowsOnly(rows, 1);
		});
		mainTable->setCellWidget(0, col, searchBar);
	}

	QPalette mainPal(mainTable->palette());
	//设置背景黑色
	mainPal.setColor(QPalette::Background, QColor(50, 50, 55));

	mainTable->setAutoFillBackground(true);
	mainTable->setPalette(mainPal);
}

void DCAllTagTable::setupCloseBtn() {
	closeBtn = new QPushButton(this);
	closeBtn->setText("CLOSE");
	QFont font = QFont("Microsoft YaHei", 15, 50);
	closeBtn->setFont(font);
	closeBtn->resize(130, 60);
	closeBtn->move(780, 60 + mainTable->height() + 20);
	connect(closeBtn, &QPushButton::clicked, this, [&]() {
		this->close();
	});
}

vector<int> DCAllTagTable::filter() {
	vector<int> row;
	for (int index = 0; index < filterArray.size(); index++) {
		vector<int> result(mainTable->searchTextInCol(index, filterArray.at(index)));

		set<int> setRow(row.begin(), row.end());
		set<int> setResult(result.begin(), result.end());
		set<int> intersectionResult;
		if (index != 0) {
			std::set_intersection(setRow.begin(), setRow.end(), setResult.begin(), setResult.end(), inserter(intersectionResult, intersectionResult.begin()));
			row.clear();
			copy(intersectionResult.begin(), intersectionResult.end(), inserter(row, row.end()));
		}
			
		else
			row = result;
	}
	sort(row.begin(), row.end());
	row.erase(unique(row.begin(), row.end()), row.end());

	return row;
}

void DCAllTagTable::onCloseBtnPressed()
{
	this->close();
}