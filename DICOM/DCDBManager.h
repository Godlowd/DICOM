#pragma once

#include <QSqlDataBase>
#include <QSqlQuery>
#include <string>

class DcmTagKey;
class DCDBManager
{
public:
	// @brief 获取单例
	static DCDBManager & getInstance();

	// @brief 添加新的scope到末尾
	// @param name 要添加的scope的名字
	void addNewScope(std::string name);

	// @brief 移除指定位置的scope
	// @param name 要移除的scope的名字
	void removeScope(std::string name);

	// @brief 添加新的key到scope的末尾
	// @param scopeName 要添加的目标scope的名字
	// @param tagKey 要添加的key
	void addNewTag(std::string scopeName, const DcmTagKey tagKey);

	// @brief 移除指定位置的key
	// @param scope 要移除的scope
	// @param pos 要移除的key的位置
	//void removeTag(DCScopeModel *scope, DcmTagKey &tagKey, int pos);

private:
	DCDBManager::DCDBManager();
	DCDBManager::~DCDBManager();

	QSqlDatabase db;
	QSqlQuery sqlQuery;
};

