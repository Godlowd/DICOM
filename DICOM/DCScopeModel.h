#pragma once
#include <vector>
#include <QAbstractListModel>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "DCDetailInfoStruct.h"
#include "DCDicomFileModel.h"
#include "DCDBManager.h"

// 自定义的tag序列，称之为scope
class DCScopeModel : public QAbstractListModel
{
public:
	DCScopeModel::DCScopeModel(std::vector<DcmTagKey> tagArray, std::string name);

public:
	// @brief 添加指定的key到队列末尾
	// @param key 要添加的key
	void addNewTag(DcmTagKey key);

	// @brief 移除指定位置的tag
	// @param key 要移除的key
	void removeTag(DcmTagKey key);

	void removeTag(int pos);

	// @brief 获取该scope展示在列表中的名字
	std::string getName() const;
	// @brief 设置该scope展示在列表中的名字
	void setName(std::string name);

	// @brief 从指定的fileModel中加载这个scope的所有的tag对应的数据
	void DCScopeModel::loadDetailInfo(DCDicomFileModel *fileModel);
	// @brief 获取tag对应的数据
	const std::vector<DCDetailInfo> DCScopeModel::getDetailInfoArray();

public:
	int DCScopeModel::rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant DCScopeModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
	std::vector<DcmTagKey> tagArray;
	std::vector<DCDetailInfo> detailInfoArray;
	/// 显示在seriesList的名称
	std::string displayName;
	DCDBManager *dbManager;
};

