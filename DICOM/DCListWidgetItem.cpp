#include "DCListWidgetItem.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>


DCListWidgetItem::DCListWidgetItem(int groupId, int tagId, std::string tagDesc, std::string value)
{
	this->groupId = groupId;
	this->tagId = tagId;
	this->tagDesc = tagDesc;
	this->value = value;

	QLabel *groupLabel = new QLabel();
	std::string groupStr = "groupId: ";
	groupLabel->setText(QString::fromStdString(groupStr.append(std::to_string(groupId))));

	QLabel *tagLabel = new QLabel();
	std::string tagIdStr = "tagId: ";
	tagLabel->setText(QString::fromStdString(tagIdStr.append(std::to_string(tagId))));

	QLabel *tagDescLabel = new QLabel();
	std::string tagDescStr = "tagDesc: ";
	tagDescLabel->setText(QString::fromStdString(tagDescStr.append(tagDesc)));

	QLabel *valueLabel = new QLabel();
	std::string valueStr = "value: ";
	valueLabel->setText(QString::fromStdString(valueStr.append(value)));

	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(groupLabel);
	layout->addWidget(tagLabel);
	layout->addWidget(tagDescLabel);
	layout->addWidget(valueLabel);

	this->setLayout(layout);
}
