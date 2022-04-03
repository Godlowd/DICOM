#pragma once
#include <QStyledItemDelegate>
#include "DCDetailInfoModel.h"
class DCDetailInfoItemDelegate :
	public QStyledItemDelegate
{

public:
	DCDetailInfoModel *model;
	DCDetailInfoItemDelegate::DCDetailInfoItemDelegate(DCDetailInfoModel *model);

	// painting
	void DCDetailInfoItemDelegate::paint(QPainter *painter,
		const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize DCDetailInfoItemDelegate::sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
};