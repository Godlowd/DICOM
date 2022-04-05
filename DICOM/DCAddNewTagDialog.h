#pragma once
#include <qdialog.h>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "DCAddNewTagProtocal.h"

class DCAddNewTagDialog :
	public QDialog
{
	Q_OBJECT
public:
	DCAddNewTagDialog::DCAddNewTagDialog(QWidget *parent, DCAddNewTagProtocol *delegate);

public slots:
	void closeDialog();
	void onClickConfirmBtn();

private:
	DCAddNewTagDialog::~DCAddNewTagDialog();

private:
	QLabel *mainTitleLabel;
	QLabel *groupLabel;
	QLabel *elementLabel;
	QLineEdit *groupInputField;
	QLineEdit *elementInputField;

	QPushButton *confirmBtn;
	QPushButton *cancelBtn;

	DCAddNewTagProtocol *delegate;
};

