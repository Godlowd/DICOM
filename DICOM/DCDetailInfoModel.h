#pragma once
#include <qabstractitemmodel.h>
#include "DCDetailInfoStruct.h"
#include <vector>

class DCDetailInfoModel :
	public QAbstractListModel
{
public:
	std::vector<DCDetailInfo> infoVector;
	DCDetailInfoModel::DCDetailInfoModel(std::vector<DCDetailInfo> infoVector);


	int DCDetailInfoModel::rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

