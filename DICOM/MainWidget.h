#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QFile>
#include <QDataStream>
#include <qwidget.h>
#include "DCDicomFileModel.h"
#include "DCScopeModel.h"
#include "DCAddNewTagProtocal.h"
#include "DCAddNewScopeProtocol.h"
#include "DCTabelWidget.h"
class MainWidget : public QWidget, public DCAddNewTagProtocol, public DCAddNewScopeProtocol
{
    Q_OBJECT
public:
    MainWidget();
public slots:

	// @brief 打开文件
	void openFile();

	// @brief 添加新的tag
	void showAddNewTagDialog();

	void convertImgToJpeg();

	/**
	 * @brief Compress the selected dicom file.
	 * 
	 */
	void saveCompressedFile();

	/**
	 * @brief Decompress the selected dicom file.
	 * 
	 */
	void saveDecompressedFile();

	/**
	 * @brief update table with file.
	 * 
	 * @param fileArray
	 */
	void updateView(std::vector<DCDicomFileModel> fileArray);

	/**
	 * @brief when user select a row in one of the three tables, select those rows which index are the same.
	 * 
	 * @param index selected index
	 */
	void sectionChoose(int index);

	/**
	 * @brief open a folder, read the dicom file in it and refresh three table.
	 * 
	 */
	void readFileinFolder();

private:
	std::vector<std::vector<DcmTagKey>> seriesVec;
	std::vector<DCTabelWidget *> tableVec;
	std::vector<DCScopeModel *> scopeVector;

	std::string lastPath;

	std::vector<DCDicomFileModel> fileModelArray;
	std::vector<DCDicomFileModel> filteredModelArray;
	

	DCDicomFileModel selectedDicomFile;
	void onClickConfirmBtn(int group, int element) override;

	void DCAddNewScopeOnClickConfirmBtn(std::string scopeName) override;

	/**
	 * @brief set up the menu bar.
	 * 
	 */
	void setupMenu();

	bool isFiltered();

	/**
	 * @brief Compress dicom image.
	 * 
	 * @param newFilePath the place where to save the compressed file
	 */
	void compressImg(std::string newFilePath);

	/**
	 * @brief Decompress dicom image.
	 *
	 * @param newFilePath the place where to save the decompressed file
	 */
	void decompressImg(std::string newFilePath);
};

#endif // MAINWIDGET_H
