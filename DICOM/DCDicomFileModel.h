#pragma once
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include <vector>
#include <memory>
#include "DCDetailInfoStruct.h"
class DCDicomFileModel
{
public:
	DCDicomFileModel::DCDicomFileModel();
	DCDicomFileModel::DCDicomFileModel(DcmFileFormat dataSet);
	void DCDicomFileModel::setDataSet(DcmFileFormat dataSet);

	std::vector<DcmElement *> DCDicomFileModel::getValueForTags(std::vector<DcmTagKey> tags);
	DcmElement *DCDicomFileModel::getValueForTag(DcmTagKey tag);
	std::string DCDicomFileModel::getStringForTag(DcmTagKey tag);
private:
	std::shared_ptr<DcmFileFormat> dataSet;
};

DCDetailInfo dcmElement2DetailInfo(DcmElement *element);