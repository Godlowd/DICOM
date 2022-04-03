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

	// @brief 根据指定的tag读取数据, 如果DICOM文件中对应的tag不存在则返回的DcmElement为null
	// @param tags 要查询的数据对应的tag
	std::vector<DcmElement *> DCDicomFileModel::getValueForTags(std::vector<DcmTagKey> tags);
	DcmElement *DCDicomFileModel::getValueForTag(DcmTagKey tag);
	std::string DCDicomFileModel::getStringForTag(DcmTagKey tag);

private:
	std::shared_ptr<DcmFileFormat> dataSet;
};

DCDetailInfo dcmElement2DetailInfo(DcmElement *element);