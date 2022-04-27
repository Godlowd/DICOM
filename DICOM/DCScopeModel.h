#pragma once
#include <vector>
#include <QAbstractListModel>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "DCDicomFileModel.h"
#include "DCDBManager.h"

// 自定义的tag序列，称之为scope
class DCScopeModel : public QAbstractListModel
{
public:
	DCScopeModel::DCScopeModel(std::vector<DcmTagKey> tagArray, std::string name = "");
	DCScopeModel::DCScopeModel(std::string name = "");

	DCScopeModel(const DCScopeModel &model);

public:
	/**
	 * 获取展示在table上的表头名.
	 * 
	 * @return 
	 */
	std::vector<std::string> getTableHeaderLabels();

	// @brief 所有的dicom文件对象的对应tag值, tag的值用字符串表示
	std::vector<std::vector<std::string>> tagValueArray;
	// @brief 获取该scope展示在列表中的名字
	std::string getName() const;
	// @brief 设置该scope展示在列表中的名字
	void setName(std::string name);

public:
	int DCScopeModel::rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant DCScopeModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	const std::vector<DcmTagKey> getTagInfoArray();

	/**
	 * 加载所有文件中所有指定的tag的数据.
	 * 
	 * @param fileArray 要加载的文件
	 * @param tagInfos 要读取的tag列表
	 */
	void loadAllData(std::vector<DCDicomFileModel *> fileArray);

private:


	/**
	 * 从单个dicom文件中读取数据
	 * 
	 * @param file 要读取的dicom文件
	 * @param tagInfos 要读取的tag列表
	 * @return 读取结果，以字符串的形式返回
	 */
	std::vector<std::string> getValueFrom(DCDicomFileModel * file, std::vector<DcmTagKey> tagInfos);

	/// 显示在seriesList的名称
	std::string displayName;
	DCDBManager *dbManager;

	// @brief 需要显示的所有tag
	std::vector<DcmTagKey> tagInfoArray;
};

