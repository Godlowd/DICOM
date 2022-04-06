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
	bool addNewTag(std::string scopeName, const DcmTagKey tagKey);

	// @brief 移除指定位置的key
	// @param scope 要移除的scope
	// @param pos 要移除的key的位置
	//void removeTag(DCScopeModel *scope, DcmTagKey &tagKey, int pos);

	// @brief 一键删除数据库中所有表
	void deleteAllTables();

	// @brief 从数据库中加载传入的scopeName对应的tagkey
	// @param scopeName 要加载的scopeName
	std::vector<DcmTagKey> loadScopeTag(std::string scopeName);

	// @brief 从数据库加载所有scope
	std::vector<DCScopeModel *> loadAllScope();
private:
	DCDBManager::DCDBManager();
	DCDBManager::~DCDBManager();

	static QSqlDatabase db;
	static QSqlQuery sqlQuery;

	static bool loadTestData();

	// @brief 返回某个查询结果的大小, sqlite3没有size()函数
	int resultSize(QSqlQuery query);
};

