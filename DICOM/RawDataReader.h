#pragma once
#include <vector>
#include <string> 

class RawDataReader
{
public:
	RawDataReader::RawDataReader();
	RawDataReader::RawDataReader(int x, int y, int z);
	
	float * readRawDataFromFile(std::string filePath);

	long GetFileLength(std::ifstream & ifs) ;

	float getBiggestValue() const;

private:
	int xSize;
	int ySize;
	int zSize;

	float *rawData;
};
