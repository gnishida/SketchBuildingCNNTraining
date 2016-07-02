#pragma once

#include <glew.h>
#include "Shader.h"
#include "Vertex.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "Camera.h"
#include "ShadowMapping.h"
#include "RenderManager.h"
#include "CGA.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const int BUILDING_MASS_MIN_X = -24;
const int BUILDING_MASS_MIN_Y = -24;
const int BUILDING_MASS_MAX_X = 24;
const int BUILDING_MASS_MAX_Y = 24;
const int BUILDING_MASS_MIN_WIDTH = 6;
const int BUILDING_MASS_MIN_DEPTH = 6;
const int BUILDING_MASS_MAX_WIDTH = 28;
const int BUILDING_MASS_MAX_DEPTH = 28;
const float CAMERA_DEFAULT_HEIGHT = 0.0f;
const float CAMERA_DEFAULT_DEPTH = 80.0f;

class MainWindow;

class GLWidget3D : public QGLWidget {
public:
	GLWidget3D(MainWindow *parent = 0);

	void render();
	void drawScene();
	void loadCGA(char* filename);

	void generateBuildingImages(const QString& cga_dir, const QString& output_dir, int image_width, int image_height, bool grayscale, int edge_displacement, bool sketchy_line, bool remove_invalid);
	void generateRoofImages(const QString& cga_dir, const QString& output_dir, int image_width, int image_height, bool grayscale, int edge_displacement, bool sketchy_line);
	void generateWindowImages(const QString& cga_dir, const QString& output_dir, int image_width, int image_height, bool grayscale, int edge_displacement, bool sketchy_line);
	void generateLedgeImages(const QString& cga_dir, const QString& output_dir, int image_width, int image_height, bool grayscale, int edge_displacement, bool sketchy_line);

	void test();
	void EDLine(const cv::Mat& source, cv::Mat& result, int edge_replacement, bool sketchy_line, bool grayscale);
	void draw2DPolyline(cv::Mat& img, const glm::vec2& p0, const glm::vec2& p1, int polyline_index);
	bool isImageValid(const cv::Mat& image);
	void predictBuildingImages(const QString& cga_dir, const QString& testdata_dir, const QString& classification_dir, const QString& regression_dir, const QString& output_dir);
	void predictRoofImages(const QString& cga_dir, const QString& testdata_dir, const QString& classification_dir, const QString& regression_dir, const QString& output_dir);
	void predictWindowImages(const QString& cga_dir, const QString& testdata_dir, const QString& classification_dir, const QString& regression_dir, const QString& output_dir);
	void predictLedgeImages(const QString& cga_dir, const QString& testdata_dir, const QString& classification_dir, const QString& regression_dir, const QString& output_dir);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public slots:
	void camera_update();

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();    
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent* e);

public:
	static enum { RENDERING_MODE_REGULAR = 0, RENDERING_MODE_LINE };

public:
	MainWindow* mainWin;
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;
	bool shiftPressed;

	RenderManager renderManager;

	cga::CGA system;
	std::vector<std::vector<glm::vec2> > style_polylines;
	std::vector<boost::shared_ptr<glutils::Face> > faces;

	boost::shared_ptr<QTimer> rotationTimer;
};

