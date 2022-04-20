#pragma once
#include <vector>
#include <QAbstractListModel>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "DCDicomFileModel.h"
#include "DCDBManager.h"

// �Զ����tag���У���֮Ϊscope
class DCScopeModel : public QAbstractListModel
{
public:
	DCScopeModel::DCScopeModel(std::vector<DcmTagKey> tagArray, std::string name = "");
	DCScopeModel::DCScopeModel(std::string name = "");

	DCScopeModel(const DCScopeModel &model);

public:
	/**
	 * ��ȡչʾ��table�ϵı�ͷ��.
	 * 
	 * @return 
	 */
	std::vector<std::string> getTableHeaderLabels();

	// @brief ���е�dicom�ļ�����Ķ�Ӧtagֵ, tag��ֵ���ַ�����ʾ
	std::vector<std::vector<std::string>> tagValueArray;
	// @brief ��ȡ��scopeչʾ���б��е�����
	std::string getName() const;
	// @brief ���ø�scopeչʾ���б��е�����
	void setName(std::string name);

public:
	int DCScopeModel::rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant DCScopeModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	const std::vector<DcmTagKey> getTagInfoArray();

	/**
	 * ���������ļ�������ָ����tag������.
	 * 
	 * @param fileArray Ҫ���ص��ļ�
	 * @param tagInfos Ҫ��ȡ��tag�б�
	 */
	void loadAllData(std::vector<DCDicomFileModel> fileArray);

private:


	/**
	 * �ӵ���dicom�ļ��ж�ȡ����
	 * 
	 * @param file Ҫ��ȡ��dicom�ļ�
	 * @param tagInfos Ҫ��ȡ��tag�б�
	 * @return ��ȡ��������ַ�������ʽ����
	 */
	std::vector<std::string> getValueFrom(DCDicomFileModel file, std::vector<DcmTagKey> tagInfos);

	/// ��ʾ��seriesList������
	std::string displayName;
	DCDBManager *dbManager;

	// @brief ��Ҫ��ʾ������tag
	std::vector<DcmTagKey> tagInfoArray;
};

