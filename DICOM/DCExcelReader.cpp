#include "DCExcelReader.h"
#include <QFile>
#include <QStringList>
#include <iostream>
#include <QTextStream>
#include <QTextCodec>

vector<vector<string>> DCExcelReader::loadCSV(string csvName)
{
	QFile inFile(csvName.c_str());
	QStringList lines;/*行数据*/
	vector <vector<string>> value;
	if (inFile.open(QIODevice::ReadOnly))
	{
		QTextStream stream_text(&inFile);
		stream_text.setCodec(QTextCodec::codecForName("UTF8"));
		while (!stream_text.atEnd())
		{
		
			lines.push_back(stream_text.readLine());
		}
		for (int j = 0; j < lines.size(); j++)
		{
			QString line = lines.at(j);
			QStringList split = line.split(",");/*列数据*/
			vector<string> tempVec;
			for (int col = 0; col < split.size(); col++)
			{
				string str = split.at(col).toLocal8Bit();
				if (str != "")
					tempVec.push_back(str);

			}
			value.push_back(tempVec);
		}
		inFile.close();
	}

	return value;
}
