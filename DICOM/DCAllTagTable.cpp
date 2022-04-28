#include "DCAllTagTable.h"
#include "DCDicomFileModel.h"
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>

#define MAIN_WIDTH 930
#define MAIN_HEIGHT 780
#define TABLE_WIDTH 850
#define TABLE_HEIGHT 630
DCAllTagTable::DCAllTagTable(QWidget *parent, vector<string> headerName, int tableIndex, MainWidget * delegate):
	QWidget(parent)
{
	resize(MAIN_WIDTH, MAIN_HEIGHT);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	setupTitleLabel();
	setupMainTable(headerName, tableIndex, delegate);
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
		if (vrName != "??")
			vrName = "UN";
		vector<string> valueVec{ tagKey->toString().c_str(), vrName, value , tagNameArray.at(row) };

		valueArray.push_back(valueVec);
	}

	mainTable->updateTable(valueArray);
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