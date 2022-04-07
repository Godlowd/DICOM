#include "DCDetailInfoItemDelegate.h"
#include "DCDetailInfoStruct.h"
#include <QPainter>

#define CELL_WIDTH 600
#define CELL_HEIGHT 40
DCDetailInfoItemDelegate::DCDetailInfoItemDelegate(DCDetailInfoModel *model) :model(model) {

}

QSize DCDetailInfoItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QSize(CELL_WIDTH, CELL_HEIGHT);
}

void DCDetailInfoItemDelegate::paint(QPainter *painter,
	const QStyleOptionViewItem &option, const QModelIndex &index) const {
	if (index.isValid()) {
		painter->save();

		DCDetailInfo info = this->model->infoVector.at(index.row());

		QRect rect = option.rect;
		painter->setPen(QPen(Qt::black));

		QRect groupRect = QRect(option.rect.left(), option.rect.y(), CELL_WIDTH / 6, CELL_HEIGHT);
		std::string groupStr = "GroupId: ";
		painter->drawText(groupRect, QString::fromStdString(groupStr.append(std::to_string(info.groupId))));

		QRect tagRect = QRect(groupRect.right(), option.rect.y(), CELL_WIDTH / 6, CELL_HEIGHT);
		std::string tagStr = "tagId: ";
		painter->drawText(tagRect, QString::fromStdString(tagStr.append(std::to_string(info.tagId))));

		QRect valueTypeRect = QRect(tagRect.right(), option.rect.y(), CELL_WIDTH / 3, CELL_HEIGHT);
		std::string valueTypeStr = "ValueType: ";
		painter->drawText(valueTypeRect, QString::fromStdString(valueTypeStr.append(info.valueType)));

		QRect valueRect = QRect(valueTypeRect.right(), option.rect.y(), CELL_WIDTH / 3, CELL_HEIGHT);
		std::string valueStr = "Value: ";
		painter->drawText(valueRect, QString::fromStdString(valueStr.append(info.value)));
		painter->restore();
	}
}


