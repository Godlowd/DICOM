#pragma once
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include <vector>
#include <memory>

class DCDicomFileModel
{
public:
	DCDicomFileModel::DCDicomFileModel();
	DCDicomFileModel::DCDicomFileModel(std::string filePath);
	DCDicomFileModel::DCDicomFileModel(DcmFileFormat fileFormat);
	void DCDicomFileModel::setFileFormat(DcmFileFormat fileFormat);

	// @brief ����ָ����tag��ȡ����, ���DICOM�ļ��ж�Ӧ��tag�������򷵻ص�DcmElementΪnull
	// @param tags Ҫ��ѯ�����ݶ�Ӧ��tag
	std::vector<std::string> DCDicomFileModel::getValueForTags(std::vector<DcmTagKey> tags);

	/**
	 * �ڵ�ǰ�ļ��У���ѯ������tag��ֵ.
	 * 
	 * @param tag Ҫ��ѯ��tag
	 * @return ��ѯ�Ľ�����ַ�������ʽ����
	 */
	std::string DCDicomFileModel::getStringForTag(DcmTagKey tag);

	std::shared_ptr<DcmFileFormat> getFileFormat();
private:
	std::shared_ptr<DcmFileFormat> fileFormat;
};