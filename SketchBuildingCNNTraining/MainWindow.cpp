#include "MainWindow.h"
#include <QFileDialog>
#include "OBJWriter.h"
#include "ImageGenerationDialog.h"
#include "ParameterEstimationDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	QActionGroup* renderingModeGroup = new QActionGroup(this);
	renderingModeGroup->addAction(ui.actionViewBasicRendering);
	renderingModeGroup->addAction(ui.actionViewSSAO);
	renderingModeGroup->addAction(ui.actionViewLineRendering);
	renderingModeGroup->addAction(ui.actionViewHatching);
	renderingModeGroup->addAction(ui.actionViewSketchyRendering);

	ui.actionViewShadow->setChecked(true);
	ui.actionViewBasicRendering->setChecked(true);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpenCGA, SIGNAL(triggered()), this, SLOT(onOpenCGA()));
	connect(ui.actionSaveGeometry, SIGNAL(triggered()), this, SLOT(onSaveGeometry()));
	connect(ui.actionViewShadow, SIGNAL(triggered()), this, SLOT(onViewShadow()));
	connect(ui.actionViewBasicRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSSAO, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewLineRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewHatching, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSketchyRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewRefresh, SIGNAL(triggered()), this, SLOT(onViewRefresh()));

	connect(ui.actionGenerateTrainingImages, SIGNAL(triggered()), this, SLOT(onGenerateTrainingImages()));
	connect(ui.actionPrediction, SIGNAL(triggered()), this, SLOT(onPrediction()));

	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);

	fileLoaded = false;
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	glWidget->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	glWidget->keyReleaseEvent(e);
}

void MainWindow::onOpenCGA() {
	QString new_filename = QFileDialog::getOpenFileName(this, tr("Open CGA file..."), "", tr("CGA Files (*.xml)"));
	if (new_filename.isEmpty()) return;

	fileLoaded = true;
	filename = new_filename;
	glWidget->loadCGA(filename.toUtf8().data());
	this->setWindowTitle("CGA Shape Grammar - " + new_filename);
}

void MainWindow::onSaveGeometry() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save OBJ file..."), "", tr("OBJ Files (*.obj)"));
	if (filename.isEmpty()) return;

	OBJWriter::write(glWidget->faces, filename.toUtf8().constData());
}

void MainWindow::onViewShadow() {
	glWidget->renderManager.useShadow = ui.actionViewShadow->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewRendering() {
	if (ui.actionViewBasicRendering->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_BASIC;
	}
	else if (ui.actionViewSSAO->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_SSAO;
	}
	else if (ui.actionViewLineRendering->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;
	}
	else if (ui.actionViewHatching->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_HATCHING;
	}
	else {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_SKETCHY;
	}
	glWidget->updateGL();
}

void MainWindow::onViewRefresh() {
	if (fileLoaded) {
		glWidget->loadCGA(filename.toUtf8().data());
	}
}

void MainWindow::onGenerateTrainingImages() {
	ImageGenerationDialog dlg;
	if (!dlg.exec()) return;

	QString cga_dir = dlg.ui.lineEditCGADirectory->text();
	QString output_dir = dlg.ui.lineEditOutputDirectory->text();
	int image_size = dlg.ui.lineEditImageSize->text().toInt();
	bool grayscale = dlg.ui.checkBoxGrayscale->isChecked();
	int edge_displacement = dlg.ui.lineEditEdgeDisplacement->text().toInt();
	bool sketchy_line = dlg.ui.checkBoxSketchyLine->isChecked();
	bool remove_invalid = dlg.ui.checkBoxRemoveInvalid->isChecked();

	if (dlg.ui.comboBoxObjectType->currentText() == "Building mass") {
		glWidget->generateBuildingImages(cga_dir, output_dir, image_size, image_size, grayscale, edge_displacement, sketchy_line, remove_invalid);
	}
	else if (dlg.ui.comboBoxObjectType->currentText() == "Roof") {
		glWidget->generateRoofImages(cga_dir, output_dir, image_size, image_size, grayscale, edge_displacement, sketchy_line);
	}
	else if (dlg.ui.comboBoxObjectType->currentText() == "Window") {
		glWidget->generateWindowImages(cga_dir, output_dir, image_size, image_size, grayscale, edge_displacement, sketchy_line);
	}
	else if (dlg.ui.comboBoxObjectType->currentText() == "Ledge") {
		glWidget->generateLedgeImages(cga_dir, output_dir, image_size, image_size, grayscale, edge_displacement, sketchy_line);
	}
}

void MainWindow::onPrediction() {
	ParameterEstimationDialog dlg;
	if (!dlg.exec()) return;

	QString cga_dir = dlg.ui.lineEditCGADirectory->text();
	QString testdata_dir = dlg.ui.lineEditTestDataDirectory->text();
	QString classification_dir = dlg.ui.lineEditClassificationDirectory->text();
	QString regression_dir = dlg.ui.lineEditRegressionDirectory->text();
	QString output_dir = dlg.ui.lineEditOutputDirectory->text();

	if (dlg.ui.comboBoxObjectType->currentText() == "Building mass") {
		glWidget->predictBuildingImages(cga_dir, testdata_dir, classification_dir, regression_dir, output_dir);
	}
	else if (dlg.ui.comboBoxObjectType->currentText() == "Roof") {
		glWidget->predictRoofImages(cga_dir, testdata_dir, classification_dir, regression_dir, output_dir);
	}
	else if (dlg.ui.comboBoxObjectType->currentText() == "Window") {
		glWidget->predictWindowImages(cga_dir, testdata_dir, classification_dir, regression_dir, output_dir);
	}
	else if (dlg.ui.comboBoxObjectType->currentText() == "Ledge") {
		glWidget->predictLedgeImages(cga_dir, testdata_dir, classification_dir, regression_dir, output_dir);
	}
}

void MainWindow::camera_update() {
	glWidget->camera.yrot += 0.02;
	glWidget->camera.updateMVPMatrix();
	glWidget->updateGL();
}