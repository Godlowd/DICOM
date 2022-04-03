#include "DCDicomFileModel.h"

DCDicomFileModel::DCDicomFileModel()
{
	this->dataSet = nullptr;
}

DCDicomFileModel::DCDicomFileModel(DcmFileFormat dataSet)
{
	this->dataSet = std::make_shared<DcmFileFormat>(dataSet);
}

void DCDicomFileModel::setDataSet(DcmFileFormat dataSet)
{
	
		this->dataSet = std::make_shared<DcmFileFormat>(dataSet);
	

	return ;
}

std::vector<DcmElement *> DCDicomFileModel::getValueForTags(std::vector<DcmTagKey> tags)
{
	std::vector<DcmElement *> result;
	for (auto key : tags) {
		DcmElement *element = this->getValueForTag(key);
		result.push_back(element);
	}

	return result;
}

DcmElement * DCDicomFileModel::getValueForTag(DcmTagKey tagKey)
{
	DcmElement *result = nullptr;
	if (this->dataSet->getDataset()->tagExists(tagKey)) {
		OFCondition status = dataSet->getDataset()->findAndGetElement(tagKey, result);
		if (!status.good()) {
			// TODO: log error
		}
	}
	else {
		// TODO: log error
	}
	
	return result;
}

std::string DCDicomFileModel::getStringForTag(DcmTagKey tag)
{
	OFString result = "";
	if (this->dataSet->getDataset()->tagExists(tag)) {
		OFCondition status = dataSet->getDataset()->findAndGetOFString(tag, result);
		if (!status.good()) {
			// TODO: log error
		}
	}
	else {
		// TODO: log error
	}


	return result.c_str();
}



DCDetailInfo dcmElement2DetailInfo(DcmElement * element)
{
	DCDetailInfo info = DCDetailInfo();
	if (element != nullptr) {
		info.groupId = element->getTag().getGroup();
		info.tagId = element->getTag().getElement();

		if (element->isaString()) {
			char * temp;
			element->getString(temp);
			info.value = temp;
		}
		else {

		}
	}
	else {
		// TODO: log error
	}
	return info;
}
