#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <string>
class DCListWidgetItem :
	public QWidget
{
public:
	int groupId;
	int tagId;
	std::string tagDesc;
	std::string value;

	DCListWidgetItem::DCListWidgetItem(int groupId, int tagId, std::string tagDesc, std::string value);
};



