#include "DCScopeModel.h"

DCScopeModel::DCScopeModel(std::vector<DcmTagKey> tagArray, std::string name):tagInfoArray(tagArray), displayName(name)
{
}

DCScopeModel::DCScopeModel(std::string name):displayName(name)
{
}

DCScopeModel::DCScopeModel(const DCScopeModel & model)
{
	tagInfoArray = model.tagInfoArray;
	tagValueArray = model.tagValueArray;
	displayName = model.displayName;
	dbManager = model.dbManager;
}

std::vector<std::string> DCScopeModel::getTableHeaderLabels()
{
	std::vector<std::string> result;
	for each (DcmTagKey tagKey in tagInfoArray)
	{
		DcmTag tag = DcmTag(tagKey);
		result.push_back(tag.getTagName());
	}
	return result;
}

std::string DCScopeModel::getName() const
{
	return displayName;
}

void DCScopeModel::setName(std::string name)
{
	displayName = name;
}


int DCScopeModel::rowCount(const QModelIndex & parent) const
{
	return min(tagValueArray.size(), 8)+ 1;
}

QVariant DCScopeModel::data(const QModelIndex & index, int role) const
{
	return QVariant();
}

const std::vector<DcmTagKey> DCScopeModel::getTagInfoArray()
{
	return tagInfoArray;
}

void DCScopeModel::loadAllData(std::vector<DCDicomFileModel *> fileArray)
{
	tagValueArray.clear();
	for (auto file : fileArray) {
		std::vector<std::string> result = this->getValueFrom(file, tagInfoArray);
		tagValueArray.push_back(result);
	}
}

std::vector<std::string> DCScopeModel::getValueFrom(DCDicomFileModel * file, std::vector<DcmTagKey> tagInfos)
{
	return file->getValueForTags(tagInfos);
}

