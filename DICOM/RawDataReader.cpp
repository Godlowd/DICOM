#include "RawDataReader.h"
#include <iostream>
#include <fstream>

#define DEFAULT_X_SIZE 160
#define DEFAULT_Y_SIZE 160
#define DEFAULT_Z_SIZE 210

RawDataReader::RawDataReader() :xSize(DEFAULT_X_SIZE), ySize(DEFAULT_Y_SIZE), zSize(DEFAULT_Z_SIZE)
{
}

RawDataReader::RawDataReader(int x, int y, int z):xSize(x), ySize(y), zSize(z)
{
}

float RawDataReader::getBiggestValue() const
{
	//find biggest value from rawData
	float biggestValueTotal = 0;
	for (int i = 0; i < xSize * ySize * zSize; ++i)
	{
		if (rawData[i] > biggestValueTotal)
		{
			biggestValueTotal = rawData[i];
		}
	}

	return biggestValueTotal;
}



float * RawDataReader::readRawDataFromFile(std::string filePath){

	std::ifstream inFile(filePath.c_str(), std::ios::binary);
	if (!inFile) {
		std::cout << "error" << std::endl;
		return 0;
	}

	int idealFileSize = xSize * ySize * zSize * sizeof(float);
	int realFileSize = GetFileLength(inFile);
	if (idealFileSize != realFileSize) {
		return nullptr;
	}

	float* rawData = new float[idealFileSize];
	inFile.read(reinterpret_cast<char *>(rawData), idealFileSize);
	inFile.close();

	this->rawData = rawData;
	return rawData;


	char value;
	long index = 0;
	float ***result = new float **[zSize];
	for (int row = 0; row < xSize; row++)
		result[row] = new float *[ySize];

	for (int row = 0; row < xSize; row++)
		for (int col = 0; col < ySize; col++)
			result[row][col] = new float[zSize];
		
	while (inFile.read(&value, sizeof(float)))
	{
		int x = index / (ySize * zSize);
		int y = ((int) (index / zSize)) % ySize;
		int z = index % zSize;

		result[x][y][z] = value;

		index += 1;
	}

	return rawData;
}

long RawDataReader::GetFileLength(std::ifstream & ifs){
	long tmppos;
	long respos;
	tmppos = ifs.tellg();//先保存最初位置
	ifs.seekg(0, std::ios::end);//将指针移到未见末尾
	respos = ifs.tellg();//保存末尾位置，即为文件大小
	ifs.seekg(tmppos, std::ios::beg);//恢复文件指针
	return respos;  //返回文件大小
}