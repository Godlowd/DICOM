#include "DCDBManager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#define SERIES_TABLE_NAME "SeriesTable"
#define TAGLIST_TABLE_NAME "TagListTable"
#define RELATION_TABLE_NAME "SeriesTagRelationTable"

DCDBManager & DCDBManager::getInstance()
{
	static DCDBManager manager = DCDBManager();
	manager.db = QSqlDatabase::addDatabase("QSQLITE");
	manager.sqlQuery = QSqlQuery(manager.db);
	db.setDatabaseName("Dicom.db");
	if (db.open()) {
		// 判断是否当前数据库中含有SeriesTable与TagListTable, 如果没有则创建
		manager.sqlQuery.exec("select name from sqlite_master where type='table' order by name;");
		bool hasSeriesTable = false;
		bool hasTagListTable = false;
		bool hasRelationTable = false;

		while (manager.sqlQuery.next())
		{
			if (manager.sqlQuery.value(0).toString().compare(SERIES_TABLE_NAME)) {
				hasSeriesTable = true;
			}

			if (manager.sqlQuery.value(0).toString().compare(TAGLIST_TABLE_NAME)) {
				hasTagListTable = true;
			}

			if (manager.sqlQuery.value(0).toString().compare(RELATION_TABLE_NAME)) {
				hasRelationTable = true;
			}
		}

		if (!hasSeriesTable) {
			std::string sql = "CREATE TABLE ";
			manager.sqlQuery.exec(sql
				.append(SERIES_TABLE_NAME)
				.append("(ID INT PRIMARY KEY NOT NULL,DisplayName TEXT NOT NULL)")
				.c_str());
		}

		if (!hasTagListTable) {
			std::string sql = "CREATE TABLE ";
			manager.sqlQuery.exec(sql
				.append(TAGLIST_TABLE_NAME)
				.append("(ID INT PRIMARY KEY NOT NULL,GROUP INT NOT NULL,ELEMENT INT NOT NULL, NAME TEXT)")
				.c_str());
		}

		if (!hasRelationTable) {
			std::string sql = "CREATE TABLE ";
			manager.sqlQuery.exec(sql
				.append(RELATION_TABLE_NAME)
				.append("(SeriesID INT NOT NULL, TagID INT NOT NULL)")
				.c_str());
		}
	}

	return manager;
}

void DCDBManager::addNewScope(DCScopeModel & scope)
{
}

void DCDBManager::insertScope(DCScopeModel & scope, int pos)
{
}

void DCDBManager::removeScope(DCScopeModel & scope, int pos)
{
}

void DCDBManager::addNewTag(DCScopeModel & scope, DcmTagKey & tagKey)
{
}

void DCDBManager::insertNewTag(DCScopeModel & scope, DcmTagKey & tagKey, int pos)
{
}

void DCDBManager::removeTag(DCScopeModel & scope, DcmTagKey & tagKey, int pos)
{
}

DCDBManager::~DCDBManager()
{
	getInstance().db.close();
}
