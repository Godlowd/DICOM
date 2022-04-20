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

	// @brief 根据指定的tag读取数据, 如果DICOM文件中对应的tag不存在则返回的DcmElement为null
	// @param tags 要查询的数据对应的tag
	std::vector<std::string> DCDicomFileModel::getValueForTags(std::vector<DcmTagKey> tags);

	/**
	 * 在当前文件中，查询给定的tag的值.
	 * 
	 * @param tag 要查询的tag
	 * @return 查询的结果以字符串的形式返回
	 */
	std::string DCDicomFileModel::getStringForTag(DcmTagKey tag);

	std::shared_ptr<DcmFileFormat> getFileFormat();
private:
	std::shared_ptr<DcmFileFormat> fileFormat;
};