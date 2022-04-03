#include "DCDetailInfoModel.h"

DCDetailInfoModel::DCDetailInfoModel(std::vector<DCDetailInfo> infoVector):infoVector(infoVector)
{
}

int DCDetailInfoModel::rowCount(const QModelIndex & parent) const
{
	int count = this->infoVector.size();
	return count;
}

QVariant DCDetailInfoModel::data(const QModelIndex & index, int role) const
{
	if (index.isValid()) {

	}
	else {

	}

	return QVariant();
}
