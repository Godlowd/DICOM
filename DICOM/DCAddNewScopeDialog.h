#pragma once
#include <qdialog.h>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "DCAddNewScopeProtocol.h"
class DCAddNewScopeDialog :
	public QDialog
{
	Q_OBJECT
public:
	DCAddNewScopeDialog::DCAddNewScopeDialog(QWidget *parent, DCAddNewScopeProtocol *delegate);

public slots:
	void closeDialog();
	void onClickConfirmBtn();

private:
	QLabel *mainTitleLabel;
	QLabel *scopeNameLabel;
	QLineEdit *scopeNameInputField;

	QPushButton *confirmBtn;
	QPushButton *cancelBtn;

	DCAddNewScopeProtocol *delegate;
};

