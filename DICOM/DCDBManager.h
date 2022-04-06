#pragma once

#include <QSqlDataBase>
#include <QSqlQuery>
#include <string>
#include <vector>

class DcmTagKey;
class DCScopeModel;
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
	bool addNewTag(std::string scopeName, const DcmTagKey tagKey);

	// @brief �Ƴ�ָ��λ�õ�key
	// @param scope Ҫ�Ƴ���scope
	// @param pos Ҫ�Ƴ���key��λ��
	//void removeTag(DCScopeModel *scope, DcmTagKey &tagKey, int pos);

	// @brief һ��ɾ�����ݿ������б�
	void deleteAllTables();

	// @brief �����ݿ��м��ش����scopeName��Ӧ��tagkey
	// @param scopeName Ҫ���ص�scopeName
	std::vector<DcmTagKey> loadScopeTag(std::string scopeName);

	// @brief �����ݿ��������scope
	std::vector<DCScopeModel *> loadAllScope();
private:
	DCDBManager::DCDBManager();
	DCDBManager::~DCDBManager();

	static QSqlDatabase db;
	static QSqlQuery sqlQuery;

	static bool loadTestData();

	// @brief ����ĳ����ѯ����Ĵ�С, sqlite3û��size()����
	int resultSize(QSqlQuery query);
};

