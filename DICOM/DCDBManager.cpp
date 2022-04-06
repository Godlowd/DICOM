#include "DCDBManager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <sstream>
#include <iostream>
#include "dcmtk/dcmdata/dctk.h"
#include <vector>
#include <QVariant>
#include "DCScopeModel.h"
#include "CommonDefine.h"
#include "Utils.h"

#define SERIES_TABLE_NAME "SeriesTable"
#define TAGLIST_TABLE_NAME "TagListTable"
#define RELATION_TABLE_NAME "SeriesTagRelationTable"

std::string insertTemplateSQL(std::string tableName, std::vector<std::string> fields, std::vector<std::string> values);
std::string selectTemplateSQL(const std::vector<std::string> fields, const std::string tableName);

QSqlDatabase DCDBManager::db = QSqlDatabase::addDatabase("QSQLITE");

DCDBManager &DCDBManager::getInstance()
{
	static DCDBManager manager = DCDBManager();
	
	QSqlQuery query = QSqlQuery(DCDBManager::db);
	DCDBManager::db.setDatabaseName("Dicom.db");
	if (DCDBManager::db.open()) {
		// 判断是否当前数据库中含有SeriesTable与TagListTable, 如果没有则创建
		if (query.exec("select name from sqlite_master where type='table' order by name;")) {
			bool hasSeriesTable = false;
			bool hasTagListTable = false;
			bool hasRelationTable = false;

			while (query.next())
			{
				if (query.value(0).toString().compare(SERIES_TABLE_NAME) == 0) {
					hasSeriesTable = true;
				}

				if (query.value(0).toString().compare(TAGLIST_TABLE_NAME) == 0) {
					hasTagListTable = true;
				}

				if (query.value(0).toString().compare(RELATION_TABLE_NAME) == 0) {
					hasRelationTable = true;
				}
			}

			if (hasSeriesTable && hasRelationTable && hasTagListTable) {
				return manager;
			}
			else {
				if (!hasSeriesTable) {
					std::string sql = "CREATE TABLE ";
					query.exec(sql
						.append(SERIES_TABLE_NAME)
						.append(" (ScopeName TEXT primary key NOT NULL unique)")
						.c_str());
				}

				if (!hasTagListTable) {
					std::stringstream ss;
					ss << "create table " << TAGLIST_TABLE_NAME << " ( [ID] INTEGER PRIMARY KEY AUTOINCREMENT, [Group] integer NOT NULL, [Element] integer NOT NULL)" << std::endl;
					query.exec(ss.str()
						.c_str());
				}

				if (!hasRelationTable) {
					std::stringstream ss;
					ss << "CREATE TABLE " << RELATION_TABLE_NAME << "(ScopeName INT NOT NULL, TagID INT NOT NULL); " << std::endl;
					query.exec(ss.str().c_str());
				}

			}

		}
	}
	else {
		std::cout << "Failed to open Database" << std::endl;
	}

	return manager;
}

void DCDBManager::addNewScope(std::string name)
{
	QSqlQuery query = QSqlQuery(DCDBManager::db);
	std::vector<std::string> fields = { "ScopeName" };
	std::vector<std::string> values = { name };
	query.exec(insertTemplateSQL(SERIES_TABLE_NAME, fields, values).c_str());
}

void DCDBManager::removeScope(std::string name)
{
	QSqlQuery query = QSqlQuery(DCDBManager::db);
	std::stringstream ss;
	ss << "DELETE FROM" << SERIES_TABLE_NAME << " WHERE ScopeName = " << name << std::endl;
	query.exec(
		ss.str()
		.c_str());
}

bool DCDBManager::addNewTag(std::string scopeName, const DcmTagKey tagKey)
{
	std::string group = std::to_string(tagKey.getGroup());
	std::string element = std::to_string(tagKey.getElement());
	std::vector<std::string> tagFields = { "Group", "Element" };
	std::vector<std::string> tagValues = { 
		group,
		element
	};
	
	std::stringstream ss;
	ss << selectTemplateSQL(tagFields, TAGLIST_TABLE_NAME) << " WHERE [Group] = " << group << " AND " << "Element = " << element << " ;" << std::endl;

	QSqlQuery query = QSqlQuery(DCDBManager::db);
	// 插入新的tag前查询数据库中是否已有相同的tag
	if (!query.exec(
		ss.str()
		.c_str())) {
		return false;
	}
	
	if (!DCDBManager::resultSize(query)) {
		if (!query.exec(insertTemplateSQL(TAGLIST_TABLE_NAME, tagFields, tagValues).c_str())) {
			return false;
		}
	}

	// 获取tag在数据库中对应的id
	ss.str("");
	ss << "select ID from " << TAGLIST_TABLE_NAME << " WHERE [Group] = " << group << " AND " << "Element = " << element << " ;" << std::endl;
	std::string tagId = "";
	if (query.exec(
		ss.str()
		.c_str())) {
		query.next();
		tagId = std::to_string(query.value(0).toInt());
	}
	else {
		return false;
	}

	// 插入新值到relation表中
	std::vector<std::string> relationFields = { "ScopeName", "TagID" };
	std::vector<std::string> relationValues = { scopeName, tagId };

	if (!query.exec(insertTemplateSQL(RELATION_TABLE_NAME, relationFields, relationValues).c_str())) {
		return false;
	}

	return true;
}

void DCDBManager::deleteAllTables()
{
	QSqlQuery query = QSqlQuery(DCDBManager::db);
	query.exec("select name from sqlite_master where type='table' order by name;");

	std::vector<std::string> nameVec;
	while (query.next()) {
		std::string tableName = query.value(0).toString().toStdString();
		nameVec.push_back(tableName);
	}

	for (std::string name : nameVec) {

		std::stringstream ss;
		ss << "DROP TABLE " << name << "; " << std::endl;
		if (query.exec(ss.str().c_str())) {
			std::cout << "delete table" << name << "success" << std::endl;
		}
		else {
			std::cout << "delete table" << name << "failed" << std::endl;
		}
	}

}

std::vector<DcmTagKey> DCDBManager::loadScopeTag(std::string scopeName)
{
	std::vector<DcmTagKey> result;
	std::stringstream ss;
	ss << "select [Group], Element from " << RELATION_TABLE_NAME << ", " << TAGLIST_TABLE_NAME 
		<< " WHERE " << RELATION_TABLE_NAME << ".TagID = " << TAGLIST_TABLE_NAME << ".ID " 
		<< "AND " << "ScopeName = '" << scopeName << "'" << std::endl;
	QSqlQuery query = QSqlQuery(DCDBManager::db);
	if (query.exec(ss.str().c_str())) {
			while (query.next()) {
				int group = query.value(0).toInt();
				int element = query.value(1).toInt();

				DcmTagKey key = DcmTagKey(group, element);
				result.push_back(key);
			}
	}
	else {

	}

	return result;
}

std::vector<DCScopeModel*> DCDBManager::loadAllScope()
{
	std::vector<DCScopeModel*> result;
	std::stringstream ss;
	ss << "select distinct ScopeName From " << SERIES_TABLE_NAME << std::endl;
	QSqlQuery query = QSqlQuery(DCDBManager::db);
	if (query.exec(ss.str().c_str())) {
		if (!DCDBManager::resultSize(query)) {
			if (DCDBManager::getInstance().loadTestData()) {
				getInstance().loadAllScope();
			}
		}
		else {
			int size = DCDBManager::resultSize(query);
			
			while (query.next()) {
				std::string scopeName = query.value(0).toString().toStdString();
				std::vector<DcmTagKey> tagVec = getInstance().loadScopeTag(scopeName);
				DCScopeModel *model = new DCScopeModel(tagVec, scopeName);
				result.push_back(model);
			}
		}
	}
	else {

	}

	return result;
}

DCDBManager::DCDBManager(){}

DCDBManager::~DCDBManager()
{
	getInstance().DCDBManager::db.close();
}

bool DCDBManager::loadTestData()
{
	std::vector<std::string> values = { "PATIENT_TAGS", "STUDY_TAGS","SERIES_TAGS" };
	std::vector<std::string> fields = { "ScopeName" };
	for (int index = 0; index < values.size(); index++) {
		std::string scopeName = values.at(index);
		std::stringstream ss;
		ss << insertTemplateSQL(SERIES_TABLE_NAME, fields, { scopeName }) << std::endl;
		QSqlQuery query = QSqlQuery(DCDBManager::db);
		if (query.exec(ss.str().c_str())) {
			std::vector<DcmTagKey> tagVec = CommonTag::PREDEFINED_TAGS.at(index);
			for (auto tag : tagVec) {
				if (!DCDBManager::getInstance().addNewTag(scopeName, tag)) {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}

	return true;
}

std::string insertTemplateSQL(std::string tableName, std::vector<std::string> fields, std::vector<std::string> values) {
	std::stringstream ss;

	if (fields.size() == values.size()) {
		ss << "INSERT INTO " << tableName << "(";

		for (int index = 0; index < fields.size(); index++) {
			ss << " [" << fields.at(index) << "] ";

			if (index != fields.size() - 1) {
				ss << ", ";
			}
		}
		ss << ") VALUES ( ";

		for (int index = 0; index < values.size(); index++) {
			ss << "'" << values.at(index) << "'";
			if (index != values.size() - 1) {
				ss << ", ";
			}
		}

		ss << ");";
	}

	return ss.str();
}

std::string selectTemplateSQL(const std::vector<std::string> fields, const std::string tableName) {
	std::stringstream ss;
	ss << "select ";
	for (int index = 0; index < fields.size(); index++) {
		ss << "[" << fields.at(index) << "]";
		if (index != fields.size() - 1) {
			ss << ", ";
		}
	}
	ss << " FROM " << tableName;

	return ss.str();
}

int DCDBManager::resultSize(QSqlQuery query) {
	return Utils::resultSize(query);
}