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

	// @brief ����ָ����tag��ȡ����, ���DICOM�ļ��ж�Ӧ��tag�������򷵻ص�DcmElementΪnull
	// @param tags Ҫ��ѯ�����ݶ�Ӧ��tag
	std::vector<DcmElement *> DCDicomFileModel::getValueForTags(std::vector<DcmTagKey> tags);
	DcmElement *DCDicomFileModel::getValueForTag(DcmTagKey tag);
	std::string DCDicomFileModel::getStringForTag(DcmTagKey tag);

private:
	std::shared_ptr<DcmFileFormat> dataSet;
};

DCDetailInfo dcmElement2DetailInfo(DcmElement *element);