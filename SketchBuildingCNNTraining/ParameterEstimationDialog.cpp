#include "ParameterEstimationDialog.h"
#include <QFileDialog>

ParameterEstimationDialog::ParameterEstimationDialog(QWidget *parent) : QDialog(parent) {
	ui.setupUi(this);

	ui.comboBoxObjectType->addItem("Building mass");
	ui.comboBoxObjectType->addItem("Roof");
	ui.comboBoxObjectType->addItem("Window");
	ui.comboBoxObjectType->addItem("Ledge");

	ui.lineEditCGADirectory->setText("../cga/building");
	ui.lineEditTestDataDirectory->setText("C:/Anaconda/caffe/building/data");
	ui.lineEditClassificationDirectory->setText("C:/Anaconda/caffe/building");
	ui.lineEditRegressionDirectory->setText("C:/Anaconda/caffe/building_regression");
	ui.lineEditOutputDirectory->setText("results");

	connect(ui.comboBoxObjectType, SIGNAL(currentIndexChanged(int)), this, SLOT(onObjectTypeChanged()));
	connect(ui.pushButtonCGADirectory, SIGNAL(clicked()), this, SLOT(onCGADirectory()));
	connect(ui.pushButtonTestDataDirectory, SIGNAL(clicked()), this, SLOT(onTestDataDirectory()));
	connect(ui.pushButtonClassificationDirectory, SIGNAL(clicked()), this, SLOT(onClassificationDirectory()));
	connect(ui.pushButtonRegressionDirectory, SIGNAL(clicked()), this, SLOT(onRegressionDirectory()));
	connect(ui.pushButtonOutputDirectory, SIGNAL(clicked()), this, SLOT(onOutputDirectory()));
	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

ParameterEstimationDialog::~ParameterEstimationDialog() {
}

void ParameterEstimationDialog::onObjectTypeChanged() {
	if (ui.comboBoxObjectType->currentText() == "Building mass") {
		ui.lineEditCGADirectory->setText("../cga/building/");
		ui.lineEditTestDataDirectory->setText("C:/Anaconda/caffe/building/data");
		ui.lineEditClassificationDirectory->setText("C:/Anaconda/caffe/building");
		ui.lineEditRegressionDirectory->setText("C:/Anaconda/caffe/building_regression");
	}
	else if (ui.comboBoxObjectType->currentText() == "Roof") {
		ui.lineEditCGADirectory->setText("../cga/roof/");
		ui.lineEditTestDataDirectory->setText("C:/Anaconda/caffe/roof/data");
		ui.lineEditClassificationDirectory->setText("C:/Anaconda/caffe/roof");
		ui.lineEditRegressionDirectory->setText("C:/Anaconda/caffe/roof_regression");
	}
	else if (ui.comboBoxObjectType->currentText() == "Window") {
		ui.lineEditCGADirectory->setText("../cga/window/");
		ui.lineEditTestDataDirectory->setText("C:/Anaconda/caffe/window/data");
		ui.lineEditClassificationDirectory->setText("C:/Anaconda/caffe/window");
		ui.lineEditRegressionDirectory->setText("C:/Anaconda/caffe/window_regression");
	}
	else if (ui.comboBoxObjectType->currentText() == "Ledge") {
		ui.lineEditCGADirectory->setText("../cga/ledge/");
		ui.lineEditTestDataDirectory->setText("C:/Anaconda/caffe/ledge/data");
		ui.lineEditClassificationDirectory->setText("C:/Anaconda/caffe/ledge");
		ui.lineEditRegressionDirectory->setText("C:/Anaconda/caffe/ledge_regression");
	}
}

void ParameterEstimationDialog::onCGADirectory() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui.lineEditCGADirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		ui.lineEditCGADirectory->setText(dir);
	}
}

void ParameterEstimationDialog::onTestDataDirectory() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui.lineEditTestDataDirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		ui.lineEditTestDataDirectory->setText(dir);
	}
}

void ParameterEstimationDialog::onClassificationDirectory() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui.lineEditClassificationDirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		ui.lineEditClassificationDirectory->setText(dir);
	}
}

void ParameterEstimationDialog::onRegressionDirectory() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui.lineEditRegressionDirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		ui.lineEditRegressionDirectory->setText(dir);
	}
}

void ParameterEstimationDialog::onOutputDirectory() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui.lineEditOutputDirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		ui.lineEditOutputDirectory->setText(dir);
	}
}

void ParameterEstimationDialog::onOK() {
	accept();
}

void ParameterEstimationDialog::onCancel() {
	reject();
}
