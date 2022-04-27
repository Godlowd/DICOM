#pragma once
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include <vector>
#include <memory>
#include <map>

using namespace std;
class DCDicomFileModel
{
public:
	DCDicomFileModel::DCDicomFileModel();
	DCDicomFileModel::DCDicomFileModel(string filePath);
	DCDicomFileModel::DCDicomFileModel(DcmFileFormat fileFormat);
	void DCDicomFileModel::setFileFormat(DcmFileFormat fileFormat);

	// @brief 根据指定的tag读取数据, 如果DICOM文件中对应的tag不存在则返回的DcmElement为null
	// @param tags 要查询的数据对应的tag
	vector<string> DCDicomFileModel::getValueForTags(vector<DcmTagKey> tags);

	/**
	 * 在当前文件中，查询给定的tag的值.
	 * 
	 * @param tag 要查询的tag
	 * @return 查询的结果以字符串的形式返回
	 */
	string DCDicomFileModel::getStringForTag(DcmTagKey tag);

	shared_ptr<DcmFileFormat> getFileFormat();

	/**
	 * @brief 获取DicomFileModel对应的文件名..
	 * 
	 * @return 
	 */
	const string getFileName();

	/**
	 * @brief 将改动应用于文件.
	 * 
	 * @param newFileName 默认应用于当前文件，如果不为空，生成指定文件名的新文件并应用改动
	 */
	bool applyChanges(string newFileName = "");

	void updateTempChange(DcmTagKey key, string value);
private:
	shared_ptr<DcmFileFormat> fileFormat;

	/**
	 * DicomFileModel对应的文件名.
	 */
	string fileName;

	/**
	 * 对文件的临时改动.
	 */
	map<DcmTagKey, string> tempChanges;
};