#include "DCAddNewTagDialog.h"
#include <QRegExpValidator>
#include <string>

DCAddNewTagDialog::DCAddNewTagDialog(QWidget *parent, DCAddNewTagProtocol *delegate) :QDialog(parent), delegate(delegate) {
	this->resize(320, 180);
	this->setAttribute(Qt::WA_DeleteOnClose, true);

	mainTitleLabel = new QLabel(this);
	mainTitleLabel->setText("Please input GroupId and ElementId");
	mainTitleLabel->adjustSize();
	mainTitleLabel->move((this->width())/2 - (mainTitleLabel->width())/2, 16);

	groupLabel = new QLabel(this);
	groupLabel->setText("GroupId");
	groupLabel->adjustSize();
	groupLabel->move(16, 16 + mainTitleLabel->height() + 16);

	groupInputField = new QLineEdit(this);
	groupInputField->move(16 + groupLabel->width() + 32, groupLabel->y());
	groupInputField->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));

	elementLabel = new QLabel(this);
	elementLabel->setText("ElementId");
	elementLabel->adjustSize();
	elementLabel->move(16, 16 + mainTitleLabel->height() + 16 + groupLabel->height() + 16);
	
	elementInputField = new QLineEdit(this);
	elementInputField->move(groupInputField->x(), elementLabel->y());
	elementInputField->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));

	confirmBtn = new QPushButton(this);
	confirmBtn->setText("Confirm");
	confirmBtn->adjustSize();
	confirmBtn->move((this->width()) / 2 - confirmBtn->width() - 8, elementInputField->y() + elementInputField->height() + 16);
	connect(confirmBtn, SIGNAL(clicked()), this, SLOT(onClickConfirmBtn()));

	cancelBtn = new QPushButton(this);
	cancelBtn->setText("Cancel");
	cancelBtn->adjustSize();
	cancelBtn->move((this->width()) / 2 + 8, elementInputField->y() + elementInputField->height() + 16);
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(closeDialog()));
}

void DCAddNewTagDialog::closeDialog()
{
	this->close();
}

void DCAddNewTagDialog::onClickConfirmBtn()
{
	delegate->onClickConfirmBtn(std::stoi(groupInputField->text().toStdString()), std::stoi(elementInputField->text().toStdString()));
	this->close();
}



DCAddNewTagDialog::~DCAddNewTagDialog()
{
}
