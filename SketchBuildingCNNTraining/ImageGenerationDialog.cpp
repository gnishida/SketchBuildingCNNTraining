#include "ImageGenerationDialog.h"
#include <QFileDialog>
#include <QComboBox>

ImageGenerationDialog::ImageGenerationDialog(QWidget *parent) : QDialog(parent) {
	ui.setupUi(this);

	ui.comboBoxObjectType->addItem("Building mass");
	ui.comboBoxObjectType->addItem("Roof");
	ui.comboBoxObjectType->addItem("Window");
	ui.comboBoxObjectType->addItem("Ledge");

	ui.lineEditCGADirectory->setText("../cga/building/");
	ui.lineEditOutputDirectory->setText("C:/Anaconda/caffe/building/data/images/");
	ui.lineEditImageSize->setText("227");
	ui.checkBoxGrayscale->setChecked(false);
	ui.lineEditEdgeDisplacement->setText("4");
	ui.checkBoxSketchyLine->setChecked(true);
	ui.checkBoxRemoveInvalid->setChecked(false);

	connect(ui.comboBoxObjectType, SIGNAL(currentIndexChanged(int)), this, SLOT(onObjectTypeChanged()));
	connect(ui.pushButtonCGADirectory, SIGNAL(clicked()), this, SLOT(onCGADirectory()));
	connect(ui.pushButtonOutputDirectory, SIGNAL(clicked()), this, SLOT(onOutputDirectory()));
	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

ImageGenerationDialog::~ImageGenerationDialog() {
}

void ImageGenerationDialog::onObjectTypeChanged() {
	if (ui.comboBoxObjectType->currentText() == "Building mass") {
		ui.lineEditCGADirectory->setText("../cga/building/");
		ui.lineEditOutputDirectory->setText("C:/Anaconda/caffe/building/data/images/");
	}
	else if (ui.comboBoxObjectType->currentText() == "Roof") {
		ui.lineEditCGADirectory->setText("../cga/roof/");
		ui.lineEditOutputDirectory->setText("C:/Anaconda/caffe/roof/data/images/");
	}
	else if (ui.comboBoxObjectType->currentText() == "Window") {
		ui.lineEditCGADirectory->setText("../cga/window/");
		ui.lineEditOutputDirectory->setText("C:/Anaconda/caffe/window/data/images/");
	}
	else if (ui.comboBoxObjectType->currentText() == "Ledge") {
		ui.lineEditCGADirectory->setText("../cga/ledge/");
		ui.lineEditOutputDirectory->setText("C:/Anaconda/caffe/ledge/data/images/");
	}
}

void ImageGenerationDialog::onCGADirectory() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui.lineEditOutputDirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		ui.lineEditCGADirectory->setText(dir);
	}
}

void ImageGenerationDialog::onOutputDirectory() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui.lineEditOutputDirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		ui.lineEditOutputDirectory->setText(dir);
	}
}

void ImageGenerationDialog::onOK() {
	accept();
}

void ImageGenerationDialog::onCancel() {
	reject();
}
