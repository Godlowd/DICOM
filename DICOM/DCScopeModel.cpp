#include "DCScopeModel.h"

DCScopeModel::DCScopeModel(std::vector<DcmTagKey> tagArray, std::string name):tagArray(tagArray), displayName(name)
{
}

void DCScopeModel::addNewTag(DcmTagKey & key)
{
	addNewTag(key, tagArray.size() - 1);
}

void DCScopeModel::addNewTag(DcmTagKey & key, int pos)
{
}

void DCScopeModel::removeTag(int pos)
{
	
}

std::string DCScopeModel::getName()
{
	return displayName;
}


void DCScopeModel::setName(std::string name)
{
	displayName = name;
}

void DCScopeModel::loadDetailInfo(DCDicomFileModel *fileModel)
{
	detailInfoArray.clear();
	for (auto tagKey : tagArray) {
		DcmElement *element = fileModel->getValueForTag(tagKey);
		DCDetailInfo info = dcmElement2DetailInfo(element);
		detailInfoArray.push_back(info);
	}
}

const std::vector<DCDetailInfo> DCScopeModel::getDetailInfoArray()
{
	return detailInfoArray;
}

int DCScopeModel::rowCount(const QModelIndex & parent) const
{
	return tagArray.size();
}

QVariant DCScopeModel::data(const QModelIndex & index, int role) const
{
	return QVariant();
}

