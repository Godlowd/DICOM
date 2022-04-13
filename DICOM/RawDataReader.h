#pragma once
#include <vector>
#include <string> 

// byte endian
enum EndianType
{
	EndianTypeLittle,
	EndianTypeBig
};

class RawDataReader
{
public:
	RawDataReader::RawDataReader();
	RawDataReader::RawDataReader(int x, int y, int z);
	RawDataReader::RawDataReader(int x, int y, int z, EndianType type);
	
	float *** readRawDataFromFile(std::string filePath, std::string elementType);

	long GetFileLength(std::ifstream & ifs);

	void setVolumeSize(int x, int y, int z);
	void setEndianType(EndianType type);

private:
	int xSize;
	int ySize;
	int zSize;
	EndianType endianType;
	
	/// 返回给的数据类型的长度
	int elementTypeMapper();
};
