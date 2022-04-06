#include "DCScopeModel.h"

DCScopeModel::DCScopeModel(std::vector<DcmTagKey> tagArray, std::string name):tagArray(tagArray), displayName(name)
{
}

void DCScopeModel::addNewTag(DcmTagKey key)
{
	tagArray.push_back(key);
	dbManager->addNewTag(this->getName(), key);
}

void DCScopeModel::removeTag(DcmTagKey key)
{
	for (int index = 0; index < tagArray.size(); index++) {
		DcmTagKey temp = tagArray.at(index);
		if (temp.getElement() == key.getElement() && temp.getGroup() == key.getGroup()) {
			tagArray.erase(tagArray.begin() + index);
		}
	}
}

void DCScopeModel::removeTag(int pos) {
	tagArray.erase(tagArray.begin() + pos);
}

std::string DCScopeModel::getName() const
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

