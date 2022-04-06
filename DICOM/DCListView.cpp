#include "DCListView.h"
DCListView::DCListView(QWidget *parent):QListView(parent) {
	this->updateSize(500, 300);
	selectedRow = 0;
}

void DCListView::updateSize(int width, int height)
{
	this->resize(width, height);
}
