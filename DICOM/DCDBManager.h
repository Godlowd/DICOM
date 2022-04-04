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
	// @param scope Ҫ��ӵ�scope
	void addNewScope(DCScopeModel &scope);

	// @brief �����µ�scope��ָ��λ��
	// @param scope Ҫ��ӵ�scope
	// @param pos Ҫ�����λ��
	void insertScope(DCScopeModel &scope, int pos);

	// @brief �Ƴ�ָ��λ�õ�scope
	// @param scope Ҫ�Ƴ���scope
	// @param pos Ҫ�Ƴ���scope��λ��
	void removeScope(DCScopeModel &scope, int pos);

	// @brief ����µ�key��scope��ĩβ
	// @param scope Ҫ��ӵ�Ŀ��scope
	// @param tagKey Ҫ��ӵ�key
	void addNewTag(DCScopeModel &scope, DcmTagKey &tagKey);

	// @brief ����µ�key��scope��ָ��λ��
	// @param scope Ҫ��ӵ�Ŀ��scope
	// @param tagKey Ҫ��ӵ�key
	// @param pos Ҫ��ӵ�λ��
	void insertNewTag(DCScopeModel &scope, DcmTagKey &tagKey, int pos);

	// @brief �Ƴ�ָ��λ�õ�key
	// @param scope Ҫ�Ƴ���scope
	// @param pos Ҫ�Ƴ���key��λ��
	void removeTag(DCScopeModel &scope, DcmTagKey &tagKey, int pos);

private:
	DCDBManager();
	~DCDBManager();

	QSqlDatabase db;
	QSqlQuery sqlQuery;
};

