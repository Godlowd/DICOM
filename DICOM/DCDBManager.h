#pragma once
#include "DCScopeModel.h"
#include <QSqlDataBase>
#include <QSqlQuery>

class DCDBManager
{
public:
	// @brief ��ȡ����
	DCDBManager & getInstance();

	// @brief ����µ�scope��ĩβ
	// @param name Ҫ��ӵ�scope������
	void addNewScope(std::string name);

	// @brief �Ƴ�ָ��λ�õ�scope
	// @param name Ҫ�Ƴ���scope������
	void removeScope(std::string name);

	// @brief ����µ�key��scope��ĩβ
	// @param scope Ҫ��ӵ�Ŀ��scope
	// @param tagKey Ҫ��ӵ�key
	void addNewTag(DCScopeModel &scope, DcmTagKey &tagKey);

	// @brief �Ƴ�ָ��λ�õ�key
	// @param scope Ҫ�Ƴ���scope
	// @param pos Ҫ�Ƴ���key��λ��
	void removeTag(DCScopeModel &scope, DcmTagKey &tagKey, int pos);

private:
	DCDBManager::DCDBManager();
	DCDBManager::~DCDBManager();

	QSqlDatabase db;
	QSqlQuery sqlQuery;
};

