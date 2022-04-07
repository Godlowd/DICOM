#include "DCDicomFileModel.h"
static std::string vrEnumConverter(DcmEVR evr) {
	switch (evr)
	{
	case EVR_AE:
		return "AE";
	case EVR_AS:
		return "AS";
	case EVR_AT:
		return "AT";
	case EVR_CS:
		return "CS";
	case EVR_DA:
		return "DA";
	case EVR_DS:
		return "DS";
	case EVR_DT:
		return "DT";
	case EVR_FL:
		return "FL";
	case EVR_FD:
		return "FD";
	case EVR_IS:
		return "IS";
	case EVR_LO:
		return "LO";
	case EVR_LT:
		return "LT";
	case EVR_OB:
		return "OB";
	case EVR_OD:
		return "OD";
	case EVR_OF:
		return "OF";
	case EVR_OL:
		return "OL";
	case EVR_OV:
		return "OV";
	case EVR_OW:
		return "OW";
	case EVR_PN:
		return "PN";
	case EVR_SH:
		return "SH";
	case EVR_SL:
		return "SL";
	case EVR_SQ:
		return "SQ";
	case EVR_SS:
		return "SS";
	case EVR_ST:
		return "ST";
	case EVR_SV:
		return "SV";
	case EVR_TM:
		return "TM";
	case EVR_UC:
		return "UC";
	case EVR_UI:
		return "UI";
	case EVR_UL:
		return "UL";
	case EVR_UR:
		return "UR";
	case EVR_US:
		return "US";
	case EVR_UT:
		return "UT";
	case EVR_UV:
		return "UV";
	case EVR_ox:
		return "ox";
	case EVR_px:
		return "px";
	case EVR_xs:
		return "xs";
	case EVR_lt:
		return "lt";
	case EVR_na:
		return "na";
	case EVR_up:
		return "up";
	case EVR_item:
		return "item";
	case EVR_metainfo:
		return "metainfo";
	case EVR_dataset:
		return "dataset";
	case EVR_fileFormat:
		return "fileFormat";
	case EVR_dicomDir:
		return "dicomDir";
	case EVR_dirRecord:
		return "dirRecord";
	case EVR_pixelSQ:
		return "pixelSQ";
	case EVR_pixelItem:
		return "pixelItem";
	case EVR_UNKNOWN:
		return "UNKNOWN";
	case EVR_UN:
		return "UN";
	case EVR_PixelData:
		return "PixelData";
	case EVR_OverlayData:
		return "OverlayData";
	case EVR_UNKNOWN2B:
		return "UNKNOWN2B";
	default:
		break;
	}
}

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

			DcmTag tag = DcmTag(info.groupId, info.tagId);
			info.valueType = vrEnumConverter(tag.getEVR());
		}
		else {

		}
	}
	else {
		// TODO: log error
	}
	return info;
}
