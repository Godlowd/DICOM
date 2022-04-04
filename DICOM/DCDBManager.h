#pragma once
#include "DCScopeModel.h"
#include <QSqlDataBase>
#include <QSqlQuery>

class DCDBManager
{
public:
	// @brief 获取单例
	DCDBManager & getInstance();

	// @brief 添加新的scope到末尾
	// @param name 要添加的scope的名字
	void addNewScope(std::string name);

	// @brief 移除指定位置的scope
	// @param name 要移除的scope的名字
	void removeScope(std::string name);

	// @brief 添加新的key到scope的末尾
	// @param scope 要添加的目标scope
	// @param tagKey 要添加的key
	void addNewTag(DCScopeModel &scope, DcmTagKey &tagKey);

	// @brief 移除指定位置的key
	// @param scope 要移除的scope
	// @param pos 要移除的key的位置
	void removeTag(DCScopeModel &scope, DcmTagKey &tagKey, int pos);

private:
	DCDBManager::DCDBManager();
	DCDBManager::~DCDBManager();

	QSqlDatabase db;
	QSqlQuery sqlQuery;
};

