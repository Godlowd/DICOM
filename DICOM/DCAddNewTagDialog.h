#pragma once
#include <qdialog.h>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class DCAddNewTagDialog :
	public QDialog
{
	
public:
	DCAddNewTagDialog::DCAddNewTagDialog(QWidget *parent);

private:
	QLabel *mainTitleLabel;
	QLabel *groupLabel;
	QLabel *elementLabel;
	QLineEdit *groupInputField;
	QLineEdit *elementInputField;

	QPushButton *confirmBtn;
	QPushButton *cancelBtn;
};

