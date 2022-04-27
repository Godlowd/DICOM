#include "DCDicomFileModel.h"
DCDicomFileModel::DCDicomFileModel()
{
	this->fileFormat = nullptr;
}

DCDicomFileModel::DCDicomFileModel(std::string filePath)
{
	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(filePath.c_str());
	if (status.good()) {
		fileFormat = std::make_shared<DcmFileFormat>(fileformat.getDataset());
		fileName = filePath;
	}
}

DCDicomFileModel::DCDicomFileModel(DcmFileFormat fileFormat)
{
	this->fileFormat = std::make_shared<DcmFileFormat>(fileFormat);
}

void DCDicomFileModel::setFileFormat(DcmFileFormat fileFormat)
{
	
		this->fileFormat = std::make_shared<DcmFileFormat>(fileFormat);
	

	return ;
}

std::vector<std::string> DCDicomFileModel::getValueForTags(std::vector<DcmTagKey> tags)
{
	std::vector<std::string> result;
	for (auto key : tags) {
		std::string value = this->getStringForTag(key);
		result.push_back(value);
	}

	return result;
}

std::string DCDicomFileModel::getStringForTag(DcmTagKey tag)
{
	OFString result = "";
	if (this->fileFormat->getDataset()->tagExists(tag)) {
		OFCondition status = fileFormat->getDataset()->findAndGetOFString(tag, result);
		if (!status.good()) {
			// TODO: log error
		}
	}
	else {
		// TODO: log error
	}


	return result.c_str();
}

std::shared_ptr<DcmFileFormat> DCDicomFileModel::getFileFormat()
{
	return fileFormat;
}

const std::string DCDicomFileModel::getFileName()
{
	return fileName;
}

bool DCDicomFileModel::applyChanges(string newFileName)
{
	map<DcmTagKey, string>::iterator it;

	for (it = tempChanges.begin(); it != tempChanges.end(); it++) {
		auto tagKey = it->first;
		auto value = it->second;

		DcmTag tag = DcmTag(tagKey.getGroup(), tagKey.getElement());
		this->fileFormat->getDataset()->putAndInsertString(tag, value.c_str(), true);
	}

	if (newFileName == "")
		return fileFormat->saveFile(fileName.c_str(), EXS_LittleEndianExplicit).good();
	else
		return fileFormat->saveFile(newFileName.c_str(), EXS_LittleEndianExplicit).good();
}

void DCDicomFileModel::updateTempChange(DcmTagKey key, string value)
{
	tempChanges[key] = value;
}
