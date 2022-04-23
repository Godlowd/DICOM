#pragma once
#include <string>
using namespace std;
class DCImageConvertManager
{
public:
	bool convertToBMP(string sourceFileName, string dstFileName, int quality = 99);
};

