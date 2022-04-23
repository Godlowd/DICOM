#pragma once
class DCImageManager
{
public:
	bool losslessDefaultCompress(const char *sourceFileName, const char *dstFileName);
	bool deCompress(const char *sourceFileName, const char *dstFileName);
};

