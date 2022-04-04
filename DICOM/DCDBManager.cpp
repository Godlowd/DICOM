#include "DCDBManager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <sstream>
#include <iostream>

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
		// �ж��Ƿ�ǰ���ݿ��к���SeriesTable��TagListTable, ���û���򴴽�
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
				.append("(ID INT PRIMARY KEY auto_increment NOT NULL,DisplayName TEXT NOT NULL)")
				.c_str());
		}

		if (!hasTagListTable) {
			std::string sql = "CREATE TABLE ";
			manager.sqlQuery.exec(sql
				.append(TAGLIST_TABLE_NAME)
				.append("(ID INT PRIMARY KEY auto_increment NOT NULL,Group INT NOT NULL,Element INT NOT NULL)")
				.c_str());
		}

		if (!hasRelationTable) {
			std::string sql = "CREATE TABLE ";
			manager.sqlQuery.exec(sql
				.append(RELATION_TABLE_NAME)
				.append("(SeriesID INT NOT NULL, TagID INT NOT NULL);")
				.c_str());
		}
	}

	return manager;
}

void DCDBManager::addNewScope(std::string name)
{
	std::vector<std::string> fields = { "DisplayName" };
	std::vector<std::string> values = { name };
	getInstance().sqlQuery.exec(insertTemplateSQL(SERIES_TABLE_NAME, fields, values).c_str());
}

void DCDBManager::removeScope(std::string name)
{
	std::string sql = "DELETE FROM";
	getInstance().sqlQuery.exec(sql
		.append(SERIES_TABLE_NAME)
		.append("WHERE DisplayName = ")
		.append(name)
		.c_str());
}

void DCDBManager::addNewTag(DCScopeModel & scope, DcmTagKey & tagKey)
{
	std::string group = std::to_string(tagKey.getGroup());
	std::string element = std::to_string(tagKey.getElement());
	std::vector<std::string> tagFields = { "Group", "Element" };
	std::vector<std::string> tagValues = { 
		group,
		element
	};
	
	std::string condition = " WHERE ";
	condition.append("Group = ");
	condition.append(group);
	condition.append(" AND ");
	condition.append("Element = ");
	condition.append(element);
	condition.append(" ;");

	// �����µ�tagǰ��ѯ���ݿ����Ƿ�������ͬ��tag
	getInstance().sqlQuery.exec(
		selectTemplateSQL(tagFields, TAGLIST_TABLE_NAME).append(condition)
		.c_str());
	
	if (!getInstance().sqlQuery.size()) {
		getInstance().sqlQuery.exec(insertTemplateSQL(TAGLIST_TABLE_NAME, tagFields, tagValues).c_str());
	}

	getInstance().sqlQuery.exec(
		selectTemplateSQL(tagFields, TAGLIST_TABLE_NAME).append(condition)
		.c_str());
	std::string tagId = getInstance().sqlQuery.value(0).toString().toStdString();

	stringstream ss;

}

void DCDBManager::removeTag(DCScopeModel & scope, DcmTagKey & tagKey, int pos)
{
}



DCDBManager::~DCDBManager()
{
	getInstance().db.close();
}

std::string insertTemplateSQL(std::string tableName, std::vector<std::string> fields, std::vector<std::string> values) {
	std::string sql = "";

	if (fields.size() == values.size()) {
		sql.append("INSERT INTO ");
		sql.append(tableName);

		sql.append("(");
		for (int index = 0; index < fields.size(); index++) {
			sql.append(fields.at(index));

			if (index != fields.size() - 1) {
				sql.append(", ");
			}
		}
		sql.append(")");

		sql.append(" VALUES ");

		sql.append("(");
		for (int index = 0; index < values.size(); index++) {
			sql.append(values.at(index));

			if (index != values.size() - 1) {
				sql.append(", ");
			}
		}
		sql.append(");");
	}

	return sql;
}

std::string selectTemplateSQL(const std::vector<std::string> fields, const std::string tableName) {
	std::string sql = "SELECT ";
	for (int index = 0; index < fields.size(); index++) {
		sql.append(fields.at(index));
		if (index != fields.size() - 1) {
			sql.append(", ");
		}
	}

	sql.append(" FROM");
	sql.append(tableName);

	return sql;
}