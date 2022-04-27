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
#include "FilterWidget.h"
#include "DCTableWidgetProtocol.h"

class MainWidget : public QWidget, public DCTableWidgetProtocol
{
    Q_OBJECT
public:
    MainWidget();
	FilterWidget* m_filterWidget;
	QMap<int, QStringList> m_map;

	DCTabelWidget *selectedTable;

// DCFilterWidgetProtocol
public:
	void updateFilterCondition(set<string> filters);

protected:
	void mousePressEvent(QMouseEvent *event);
public slots:

	// @brief 打开文件
	void openFile();


	/**
	 * @brief Compress the selected dicom file.
	 * 
	 */
	void selectFileToConvert();
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
	void updateView(std::vector<DCDicomFileModel *> fileArray, bool isFiltering = false);

	void filterTable();

	/**
	 * @brief when user select a row in one of the three tables, select those rows which index are the same.
	 * 
	 * @param index selected index
	 */
	void sectionChoose(int index) override;

	DCDicomFileModel * selectedDicomFile();

	/**
	 * @brief open a folder, read the dicom file in it and refresh three table.
	 * 
	 */
	void readFileinFolder();

	void onPatientHeaderClicked(int row) override;
	void onStudyHeaderClicked(int row) override;
	void onSeriesHeaderClicked(int row) override;
	void updateTempChanges(int tableIndex, int row, int col, string newValue) override;

	void onHorizontalClicked(int col, DCTabelWidget * table);


	void filter(int col, QStringList showList);

	void closeFilterWidget();

	void saveAction();

	void saveAsAction();

	bool applyChangesToFile(DCDicomFileModel *filemodel, string newFileName = "");

private:
	std::vector<std::vector<DcmTagKey>> seriesVec;
	std::vector<DCTabelWidget *> tableVec;
	std::vector<DCScopeModel *> scopeVector;

	std::string lastPath;

	int selectedRow;
	std::vector<DCDicomFileModel *> fileModelArray;
	std::vector<DCDicomFileModel *> filteredModelArray;
	
	DCTabelWidget *patientTable;
	DCTabelWidget *studyTable;
	DCTabelWidget *seriesTable;

	/**
	 * 三个表各自的筛选条件的集合，每一列的筛选条件对应map中的一个键值对.
	 */
	std::vector<std::map<int, std::set<std::string>>> filterVec;
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

	/**
	 * @brief 将选定的图片转换成JPEG格式.
	 * 
	 * @param originFilePath 待转换图片路径
	 * @param newFilePath 转换后的图片路径
	 */
	void convertImgToJpeg(std::string originFilePath, std::string newFilePath);
};

#endif // MAINWIDGET_H
