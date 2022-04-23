#include "FilterWidget.h"

#include <QVBoxLayout>
#include <QEventLoop>


//Items：所有项
//showItems：显示的项
//col：列号
FilterWidget::FilterWidget(QStringList Items, QStringList showItems, int col, QWidget *parent/*=0*/) :
	QWidget(parent),
	m_col(col),
	delegate(nullptr)
{
	this->setFixedWidth(100);
	this->setFixedHeight(200);

	m_ListWidget = new QListWidget(this);

	QVBoxLayout* lay = new QVBoxLayout(this);
	lay->setMargin(0);
	lay->setSpacing(0);
	setLayout(lay);
	for each (QString var in Items)
	{
		QCheckBox* checkBox = new QCheckBox(this);
		checkBox->setText(var);
		vector<string>::iterator it = std::find(m_showList.begin(), m_showList.end(), var.toStdString());
		bool first = it != m_showList.end();
		bool second = showItems.contains(var);
		if (showItems.contains(var))
		{
			m_showList.push_back(var.toStdString());
			checkBox->setChecked(true);
		}

		connect(checkBox, &QCheckBox::stateChanged, this, &FilterWidget::slot_stateChanged);
		QListWidgetItem *item = new QListWidgetItem();
		m_ListWidget->addItem(item);
		m_ListWidget->setItemWidget(item, checkBox);
	}
	lay->addWidget(m_ListWidget);

}

FilterWidget::~FilterWidget()
{
}

vector<string> FilterWidget::getShowList()
{
	return m_showList;
}

int FilterWidget::getColumn()
{
	return m_col;
}

void FilterWidget::exec(const QPoint & pos)
{
	setAttribute(Qt::WA_ShowModal, true);
	move(pos);
	show();
	QRect rect = geometry();
	QEventLoop loop;
	loop.exec();
}

void FilterWidget::slot_stateChanged()
{
	m_showList.clear();
	//遍历
	for (int i = 0; i < m_ListWidget->count(); i++)
	{
		//先获取QListWidgetItem
		QListWidgetItem *item = m_ListWidget->item(i);
		//找到第i个Item对应的CheckBox
		QCheckBox *checkBox = static_cast<QCheckBox *>(m_ListWidget->itemWidget(item));

		if (checkBox->isChecked())
			m_showList.push_back(checkBox->text().toStdString());
	}

	if (delegate != nullptr)
		delegate->updateFilterCondition(m_col, m_showList);
}