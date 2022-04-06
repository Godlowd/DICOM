#include "DCAddNewScopeDialog.h"

DCAddNewScopeDialog::DCAddNewScopeDialog(QWidget * parent, DCAddNewScopeProtocol * delegate):QDialog(parent), delegate(delegate)
{
	this->resize(320, 150);
	this->setAttribute(Qt::WA_DeleteOnClose, true);

	mainTitleLabel = new QLabel(this);
	mainTitleLabel->setText("Please input the new scope name");
	mainTitleLabel->adjustSize();
	mainTitleLabel->move((this->width()) / 2 - (mainTitleLabel->width()) / 2, 16);

	scopeNameLabel = new QLabel(this);
	scopeNameLabel->setText("scopeName");
	scopeNameLabel->adjustSize();
	scopeNameLabel->move(16, 16 + mainTitleLabel->height() + 16);

	scopeNameInputField = new QLineEdit(this);
	scopeNameInputField->move(16 + scopeNameLabel->width() + 32, scopeNameLabel->y());

	confirmBtn = new QPushButton(this);
	confirmBtn->setText("Confirm");
	confirmBtn->adjustSize();
	confirmBtn->move((this->width()) / 2 - confirmBtn->width() - 8, scopeNameInputField->y() + scopeNameInputField->height() + 16);
	connect(confirmBtn, SIGNAL(clicked()), this, SLOT(onClickConfirmBtn()));

	cancelBtn = new QPushButton(this);
	cancelBtn->setText("Cancel");
	cancelBtn->adjustSize();
	cancelBtn->move((this->width()) / 2 + 8, scopeNameInputField->y() + scopeNameInputField->height() + 16);
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(closeDialog()));
}

void DCAddNewScopeDialog::closeDialog()
{
	this->close();
}

void DCAddNewScopeDialog::onClickConfirmBtn()
{
	delegate->DCAddNewScopeOnClickConfirmBtn(scopeNameInputField->text().toStdString());
	this->close();
}
