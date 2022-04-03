#pragma once
#include <vector>
#include <QAbstractListModel>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "DCDetailInfoStruct.h"
#include "DCDicomFileModel.h"

// 自定义的tag序列，称之为scope
class DCScopeModel : public QAbstractListModel
{
public:
	DCScopeModel::DCScopeModel(std::vector<DcmTagKey> tagArray, std::string name);

public:
	void addNewTag(DcmTagKey &key);
	void removeTag(DcmTagKey &key);

	std::string getName();
	void setName(std::string name);

	void DCScopeModel::loadDetailInfo(DCDicomFileModel *fileModel);

	const std::vector<DCDetailInfo> DCScopeModel::getDetailInfoArray();

public:
	int DCScopeModel::rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant DCScopeModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
	std::vector<DcmTagKey> tagArray;
	std::vector<DCDetailInfo> detailInfoArray;
	/// 显示在seriesList的名称
	std::string displayName;	
};

