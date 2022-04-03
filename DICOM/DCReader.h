#pragma once
#include <string>
class DCDicomFileModel;
class DCReader
{
public:
	DCReader::DCReader();
	 //@brief ��ȡָ��·���µ�DICOM�ļ����������ݼ��ص�ָ����fileModel��
	 //@param fileName ����ȡ�ļ�·��
	 //@param model DICOM���ݴ��ģ��
	void DCReader::readFromFile(std::string fileName, DCDicomFileModel *model);

	// @brief �����ϴδ򿪵��ļ�·��
	void updatePath(std::string path);
	std::string getPath();
private:
	std::string lastPath;
};

