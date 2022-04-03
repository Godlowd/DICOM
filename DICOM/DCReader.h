#pragma once
#include <string>
class DCDicomFileModel;
class DCReader
{
public:
	DCReader::DCReader();
	 //@brief 读取指定路径下的DICOM文件，并将数据加载到指定的fileModel中
	 //@param fileName 待读取文件路径
	 //@param model DICOM数据存放模型
	void DCReader::readFromFile(std::string fileName, DCDicomFileModel *model);

	// @brief 更新上次打开的文件路径
	void updatePath(std::string path);
	std::string getPath();
private:
	std::string lastPath;
};

