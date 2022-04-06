#pragma once
#include <vector>
#include <QAbstractListModel>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "DCDetailInfoStruct.h"
#include "DCDicomFileModel.h"
#include "DCDBManager.h"

// �Զ����tag���У���֮Ϊscope
class DCScopeModel : public QAbstractListModel
{
public:
	DCScopeModel::DCScopeModel(std::vector<DcmTagKey> tagArray, std::string name);

public:
	// @brief ���ָ����key������ĩβ
	// @param key Ҫ��ӵ�key
	void addNewTag(DcmTagKey key);

	// @brief �Ƴ�ָ��λ�õ�tag
	// @param key Ҫ�Ƴ���key
	void removeTag(DcmTagKey key);

	void removeTag(int pos);

	// @brief ��ȡ��scopeչʾ���б��е�����
	std::string getName() const;
	// @brief ���ø�scopeչʾ���б��е�����
	void setName(std::string name);

	// @brief ��ָ����fileModel�м������scope�����е�tag��Ӧ������
	void DCScopeModel::loadDetailInfo(DCDicomFileModel *fileModel);
	// @brief ��ȡtag��Ӧ������
	const std::vector<DCDetailInfo> DCScopeModel::getDetailInfoArray();

public:
	int DCScopeModel::rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant DCScopeModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
	std::vector<DcmTagKey> tagArray;
	std::vector<DCDetailInfo> detailInfoArray;
	/// ��ʾ��seriesList������
	std::string displayName;
	DCDBManager *dbManager;
};

