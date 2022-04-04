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
	// @param scope 要添加的scope
	void addNewScope(DCScopeModel &scope);

	// @brief 插入新的scope到指定位置
	// @param scope 要添加的scope
	// @param pos 要插入的位置
	void insertScope(DCScopeModel &scope, int pos);

	// @brief 移除指定位置的scope
	// @param scope 要移除的scope
	// @param pos 要移除的scope的位置
	void removeScope(DCScopeModel &scope, int pos);

	// @brief 添加新的key到scope的末尾
	// @param scope 要添加的目标scope
	// @param tagKey 要添加的key
	void addNewTag(DCScopeModel &scope, DcmTagKey &tagKey);

	// @brief 添加新的key到scope的指定位置
	// @param scope 要添加的目标scope
	// @param tagKey 要添加的key
	// @param pos 要添加的位置
	void insertNewTag(DCScopeModel &scope, DcmTagKey &tagKey, int pos);

	// @brief 移除指定位置的key
	// @param scope 要移除的scope
	// @param pos 要移除的key的位置
	void removeTag(DCScopeModel &scope, DcmTagKey &tagKey, int pos);

private:
	DCDBManager();
	~DCDBManager();

	QSqlDatabase db;
	QSqlQuery sqlQuery;
};

