#pragma once

#include <QSqlDataBase>
#include <QSqlQuery>
#include <string>

class DcmTagKey;
class DCDBManager
{
public:
	// @brief ��ȡ����
	static DCDBManager & getInstance();

	// @brief ����µ�scope��ĩβ
	// @param name Ҫ��ӵ�scope������
	void addNewScope(std::string name);

	// @brief �Ƴ�ָ��λ�õ�scope
	// @param name Ҫ�Ƴ���scope������
	void removeScope(std::string name);

	// @brief ����µ�key��scope��ĩβ
	// @param scopeName Ҫ��ӵ�Ŀ��scope������
	// @param tagKey Ҫ��ӵ�key
	void addNewTag(std::string scopeName, const DcmTagKey tagKey);

	// @brief �Ƴ�ָ��λ�õ�key
	// @param scope Ҫ�Ƴ���scope
	// @param pos Ҫ�Ƴ���key��λ��
	//void removeTag(DCScopeModel *scope, DcmTagKey &tagKey, int pos);

private:
	DCDBManager::DCDBManager();
	DCDBManager::~DCDBManager();

	QSqlDatabase db;
	QSqlQuery sqlQuery;
};

