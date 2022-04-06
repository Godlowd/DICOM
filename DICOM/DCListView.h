#pragma once
#include <qlistview.h>

class DCListView : public QListView
{

public:
	int selectedRow;

	DCListView::DCListView(QWidget *parent = nullptr);

	void updateSize(int width, int height);
};

