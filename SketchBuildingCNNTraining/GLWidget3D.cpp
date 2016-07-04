#include "GLWidget3D.h"
#include "MainWindow.h"
#include "OBJLoader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "GrammarParser.h"
#include <map>
#include "Rectangle.h"
#include "Circle.h"
#include "Polygon.h"
#include "UShape.h"
#include "GLUtils.h"
#include <QDir>
#include <QTextStream>
#include <iostream>
#include <QProcess>
#include "Utils.h"
#include "Classifier.h"
#include "Regression.h"
#include <boost/algorithm/string.hpp>
#include <opencv2/opencv.hpp>

#ifndef M_PI
#define M_PI	3.141592653
#endif

#ifndef SQR
#define SQR(x)	((x) * (x))
#endif

GLWidget3D::GLWidget3D(MainWindow *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
	this->mainWin = parent;
	shiftPressed = false;

	// 光源位置をセット
	// ShadowMappingは平行光源を使っている。この位置から原点方向を平行光源の方向とする。
	light_dir = glm::normalize(glm::vec3(-4, -5, -8));

	// シャドウマップ用のmodel/view/projection行列を作成
	glm::mat4 light_pMatrix = glm::ortho<float>(-50, 50, -50, 50, 0.1, 200);
	glm::mat4 light_mvMatrix = glm::lookAt(-light_dir * 50.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	light_mvpMatrix = light_pMatrix * light_mvMatrix;

	// initialize stylized polylines
	style_polylines.resize(10);
	style_polylines[0].push_back(glm::vec2(0, 0));
	style_polylines[0].push_back(glm::vec2(0.3, 0.035));
	style_polylines[0].push_back(glm::vec2(0.6, 0.05));
	style_polylines[0].push_back(glm::vec2(0.85, 0.04));
	style_polylines[0].push_back(glm::vec2(1, 0));

	style_polylines[1].push_back(glm::vec2(0, 0));
	style_polylines[1].push_back(glm::vec2(0.13, -0.01));
	style_polylines[1].push_back(glm::vec2(0.27, -0.02));
	style_polylines[1].push_back(glm::vec2(0.7, -0.02));
	style_polylines[1].push_back(glm::vec2(0.81, 0));
	style_polylines[1].push_back(glm::vec2(1, 0));

	style_polylines[2].push_back(glm::vec2(0, 0));
	style_polylines[2].push_back(glm::vec2(0.12, 0.01));
	style_polylines[2].push_back(glm::vec2(0.37, 0.02));
	style_polylines[2].push_back(glm::vec2(0.6, 0.02));
	style_polylines[2].push_back(glm::vec2(0.77, 0.01));
	style_polylines[2].push_back(glm::vec2(0.91, 0.005));
	style_polylines[2].push_back(glm::vec2(1, 0));

	style_polylines[3].push_back(glm::vec2(0, 0));
	style_polylines[3].push_back(glm::vec2(0.57, -0.01));
	style_polylines[3].push_back(glm::vec2(0.8, -0.01));
	style_polylines[3].push_back(glm::vec2(1, 0));

	style_polylines[4].push_back(glm::vec2(0, 0));
	style_polylines[4].push_back(glm::vec2(0.13, -0.01));
	style_polylines[4].push_back(glm::vec2(0.23, -0.02));
	style_polylines[4].push_back(glm::vec2(0.31, -0.02));
	style_polylines[4].push_back(glm::vec2(0.38, -0.01));
	style_polylines[4].push_back(glm::vec2(0.46, 0.0));
	style_polylines[4].push_back(glm::vec2(0.61, 0.02));
	style_polylines[4].push_back(glm::vec2(0.68, 0.03));
	style_polylines[4].push_back(glm::vec2(0.8, 0.03));
	style_polylines[4].push_back(glm::vec2(0.88, 0.02));
	style_polylines[4].push_back(glm::vec2(1, 0));

	style_polylines[5].push_back(glm::vec2(0, 0));
	style_polylines[5].push_back(glm::vec2(0.29, -0.03));
	style_polylines[5].push_back(glm::vec2(0.47, -0.01));
	style_polylines[5].push_back(glm::vec2(0.59, 0.02));
	style_polylines[5].push_back(glm::vec2(0.75, 0.03));
	style_polylines[5].push_back(glm::vec2(1, 0));

	style_polylines[6].push_back(glm::vec2(0, 0));
	style_polylines[6].push_back(glm::vec2(0.16, -0.01));
	style_polylines[6].push_back(glm::vec2(0.42, -0.06));
	style_polylines[6].push_back(glm::vec2(0.65, -0.07));
	style_polylines[6].push_back(glm::vec2(0.83, -0.04));
	style_polylines[6].push_back(glm::vec2(1, 0));

	style_polylines[7].push_back(glm::vec2(0, 0));
	style_polylines[7].push_back(glm::vec2(0.24, 0.02));
	style_polylines[7].push_back(glm::vec2(0.59, 0.03));
	style_polylines[7].push_back(glm::vec2(0.79, 0.01));
	style_polylines[7].push_back(glm::vec2(0.91, -0.01));
	style_polylines[7].push_back(glm::vec2(1, 0));

	style_polylines[8].push_back(glm::vec2(0, 0));
	style_polylines[8].push_back(glm::vec2(0.15, 0.0));
	style_polylines[8].push_back(glm::vec2(0.28, 0.02));
	style_polylines[8].push_back(glm::vec2(0.44, 0.01));
	style_polylines[8].push_back(glm::vec2(0.59, 0.0));
	style_polylines[8].push_back(glm::vec2(0.74, -0.03));
	style_polylines[8].push_back(glm::vec2(0.81, -0.04));
	style_polylines[8].push_back(glm::vec2(0.89, -0.04));
	style_polylines[8].push_back(glm::vec2(1, 0));

	style_polylines[9].push_back(glm::vec2(0, 0));
	style_polylines[9].push_back(glm::vec2(0.41, -0.03));
	style_polylines[9].push_back(glm::vec2(0.56, -0.04));
	style_polylines[9].push_back(glm::vec2(0.68, -0.03));
	style_polylines[9].push_back(glm::vec2(0.78, -0.02));
	style_polylines[9].push_back(glm::vec2(0.85, -0.01));
	style_polylines[9].push_back(glm::vec2(0.94, 0.0));
	style_polylines[9].push_back(glm::vec2(1, 0));
}

/**
 * This event handler is called when the mouse press events occur.
 */
void GLWidget3D::mousePressEvent(QMouseEvent *e) {
	camera.mousePress(e->x(), e->y());
}

/**
 * This event handler is called when the mouse release events occur.
 */
void GLWidget3D::mouseReleaseEvent(QMouseEvent *e) {
}

/**
 * This event handler is called when the mouse move events occur.
 */
void GLWidget3D::mouseMoveEvent(QMouseEvent *e) {
	if (e->buttons() & Qt::RightButton) { // Rotate
		if (shiftPressed) { // Move
			camera.move(e->x(), e->y());
		}
		else {
			camera.rotate(e->x(), e->y());
		}
	}

	updateGL();
}

void GLWidget3D::wheelEvent(QWheelEvent* e) {
	camera.zoom(e->delta() * 0.1);
	updateGL();
}

/**
 * This function is called once before the first call to paintGL() or resizeGL().
 */
void GLWidget3D::initializeGL() {
	// init glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	if (glewIsSupported("GL_VERSION_4_2"))
		printf("Ready for OpenGL 4.2\n");
	else {
		printf("OpenGL 4.2 not supported\n");
		exit(1);
	}
	const GLubyte* text = glGetString(GL_VERSION);
	printf("VERSION: %s\n", text);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_3D);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDisable(GL_TEXTURE_3D);

	glEnable(GL_TEXTURE_2D_ARRAY);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDisable(GL_TEXTURE_2D_ARRAY);

	////////////////////////////////
	renderManager.init("", "", "", true, 8192);
	renderManager.resize(this->width(), this->height());

	glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "tex0"), 0);//tex0: 0

	system.modelMat = glm::rotate(glm::mat4(), -3.1415926f * 0.5f, glm::vec3(1, 0, 0));
}

/**
 * This function is called whenever the widget has been resized.
 */
void GLWidget3D::resizeGL(int width, int height) {
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	camera.updatePMatrix(width, height);

	renderManager.resize(width, height);
}

/**
 * This function is called whenever the widget needs to be painted.
 */
void GLWidget3D::paintGL() {
	render();
}

void GLWidget3D::render() {
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PASS 1: Render to texture
	glUseProgram(renderManager.programs["pass1"]);

	glBindFramebuffer(GL_FRAMEBUFFER, renderManager.fragDataFB);
	glClearColor(0.95, 0.95, 0.95, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderManager.fragDataTex[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, renderManager.fragDataTex[1], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, renderManager.fragDataTex[2], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, renderManager.fragDataTex[3], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderManager.fragDepthTex, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, DrawBuffers); // "3" is the size of DrawBuffers
	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("+ERROR: GL_FRAMEBUFFER_COMPLETE false\n");
		exit(0);
	}

	glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["pass1"], "mvpMatrix"), 1, false, &camera.mvpMatrix[0][0]);
	glUniform3f(glGetUniformLocation(renderManager.programs["pass1"], "lightDir"), light_dir.x, light_dir.y, light_dir.z);
	glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["pass1"], "light_mvpMatrix"), 1, false, &light_mvpMatrix[0][0]);

	glUniform1i(glGetUniformLocation(renderManager.programs["pass1"], "shadowMap"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, renderManager.shadow.textureDepth);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	drawScene();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PASS 2: Create AO
	if (renderManager.renderingMode == RenderManager::RENDERING_MODE_SSAO) {
		glUseProgram(renderManager.programs["ssao"]);
		glBindFramebuffer(GL_FRAMEBUFFER, renderManager.fragDataFB_AO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderManager.fragAOTex, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderManager.fragDepthTex_AO, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("++ERROR: GL_FRAMEBUFFER_COMPLETE false\n");
			exit(0);
		}

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUniform2f(glGetUniformLocation(renderManager.programs["ssao"], "pixelSize"), 2.0f / this->width(), 2.0f / this->height());

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "tex0"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[0]);

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "tex1"), 2);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[1]);

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "tex2"), 3);
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[2]);

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "depthTex"), 8);
		glActiveTexture(GL_TEXTURE8);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDepthTex);

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "noiseTex"), 7);
		glActiveTexture(GL_TEXTURE7);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragNoiseTex);

		{
			glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["ssao"], "mvpMatrix"), 1, false, &camera.mvpMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["ssao"], "pMatrix"), 1, false, &camera.pMatrix[0][0]);
		}

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "uKernelSize"), renderManager.uKernelSize);
		glUniform3fv(glGetUniformLocation(renderManager.programs["ssao"], "uKernelOffsets"), renderManager.uKernelOffsets.size(), (const GLfloat*)renderManager.uKernelOffsets.data());

		glUniform1f(glGetUniformLocation(renderManager.programs["ssao"], "uPower"), renderManager.uPower);
		glUniform1f(glGetUniformLocation(renderManager.programs["ssao"], "uRadius"), renderManager.uRadius);

		glBindVertexArray(renderManager.secondPassVAO);

		glDrawArrays(GL_QUADS, 0, 4);
		glBindVertexArray(0);
		glDepthFunc(GL_LEQUAL);
	}
	else if (renderManager.renderingMode == RenderManager::RENDERING_MODE_LINE || renderManager.renderingMode == RenderManager::RENDERING_MODE_HATCHING) {
		glUseProgram(renderManager.programs["line"]);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUniform2f(glGetUniformLocation(renderManager.programs["line"], "pixelSize"), 1.0f / this->width(), 1.0f / this->height());
		glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["line"], "pMatrix"), 1, false, &camera.pMatrix[0][0]);
		if (renderManager.renderingMode == RenderManager::RENDERING_MODE_LINE) {
			glUniform1i(glGetUniformLocation(renderManager.programs["line"], "useHatching"), 0);
		}
		else {
			glUniform1i(glGetUniformLocation(renderManager.programs["line"], "useHatching"), 1);
		}

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "tex0"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[0]);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "tex1"), 2);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[1]);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "tex2"), 3);
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[2]);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "tex3"), 4);
		glActiveTexture(GL_TEXTURE4);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[3]);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "depthTex"), 8);
		glActiveTexture(GL_TEXTURE8);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDepthTex);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "hatchingTexture"), 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_3D, renderManager.hatchingTextures);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindVertexArray(renderManager.secondPassVAO);

		glDrawArrays(GL_QUADS, 0, 4);
		glBindVertexArray(0);
		glDepthFunc(GL_LEQUAL);
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Blur

	if (renderManager.renderingMode != RenderManager::RENDERING_MODE_LINE && renderManager.renderingMode != RenderManager::RENDERING_MODE_HATCHING) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		qglClearColor(QColor(0xFF, 0xFF, 0xFF));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUseProgram(renderManager.programs["blur"]);
		glUniform2f(glGetUniformLocation(renderManager.programs["blur"], "pixelSize"), 2.0f / this->width(), 2.0f / this->height());
		//printf("pixelSize loc %d\n", glGetUniformLocation(vboRenderManager.programs["blur"], "pixelSize"));

		glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "tex0"), 1);//COLOR
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[0]);

		glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "tex1"), 2);//NORMAL
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[1]);

		/*glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "tex2"), 3);
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[2]);*/

		glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "depthTex"), 8);
		glActiveTexture(GL_TEXTURE8);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDepthTex);

		glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "tex3"), 4);//AO
		glActiveTexture(GL_TEXTURE4);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragAOTex);

		if (renderManager.renderingMode == RenderManager::RENDERING_MODE_SSAO) {
			glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "ssao_used"), 1); // ssao used
		}
		else {
			glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "ssao_used"), 0); // no ssao
		}

		glBindVertexArray(renderManager.secondPassVAO);

		glDrawArrays(GL_QUADS, 0, 4);
		glBindVertexArray(0);
		glDepthFunc(GL_LEQUAL);

	}

	// REMOVE
	glActiveTexture(GL_TEXTURE0);


	//printf("<<\n");
	//VBOUtil::disaplay_memory_usage();
}

/**
 * Draw the scene.
 */
void GLWidget3D::drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(true);

	renderManager.renderAll();
}

void GLWidget3D::loadCGA(char* filename) {
	renderManager.removeObjects();

	float offset_x = 0.0f;
	float offset_y = 0.0f;

#if 0
	{ // for window
		camera.pos = glm::vec3(0, 0, 7);
		camera.updateMVPMatrix();
		float object_width = 2.0f;
		float object_depth = 2.0f;

		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		//cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 1
	{ // for building
		camera.xrot = 30.0f;
		camera.yrot = -45.0f;
		camera.zrot = 0.0f;
		camera.pos = glm::vec3(0, 5 * cosf(camera.xrot / 180.0f * M_PI), 80 + 5 * sinf(camera.xrot / 180.0f * M_PI));
		camera.updateMVPMatrix();

		float object_width = 15.0f;
		float object_depth = 15.0f;

		float offset_x = 0.0f;
		float offset_y = 0.0f;
		float offset_z = 0.0f;
		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, offset_z)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		//cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for roof
		float object_width = 18.0f;
		float object_depth = 12.0f;

		float offset_x = 0.0f;
		float offset_y = 0.0f;
		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		//cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for circular roof
		float object_width = 12.0f;
		float object_depth = 12.0f;

		float offset_x = 0.0f;
		float offset_y = 0.0f;
		cga::Circle* start = new cga::Circle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for ledge
		float object_width = 12.0f;
		float object_depth = 0.5f;

		float offset_x = 0.0f;
		float offset_y = 0.0f;
		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		//cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for Willis Tower
		float object_width = 10.0f;
		float object_depth = 10.0f;// 8.0f;

		cga::Rectangle* start1 = new cga::Rectangle("Start1", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 1.5, offset_y - (float)object_depth * 1.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2 = new cga::Rectangle("Start2", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 0.5, offset_y - (float)object_depth * 1.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2));
		cga::Rectangle* start3 = new cga::Rectangle("Start3", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x + (float)object_width * 0.5, offset_y - (float)object_depth * 1.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3));

		cga::Rectangle* start4 = new cga::Rectangle("Start4", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 1.5, offset_y - (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
		cga::Rectangle* start5 = new cga::Rectangle("Start5", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 0.5, offset_y - (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start5));
		cga::Rectangle* start6 = new cga::Rectangle("Start6", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x + (float)object_width * 0.5, offset_y - (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start6));

		cga::Rectangle* start7 = new cga::Rectangle("Start7", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 1.5, offset_y + (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start7));
		cga::Rectangle* start8 = new cga::Rectangle("Start8", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 0.5, offset_y + (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start8));
		cga::Rectangle* start9 = new cga::Rectangle("Start9", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x + (float)object_width * 0.5, offset_y + (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start9));
	}
#endif

#if 0
	{ // for building R
		float object_width = 32.0f;
		float object_depth = 24.0f;

		cga::UShape* start = new cga::UShape("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, 10, 8, glm::vec3(1, 1, 1));

		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for building T
		float object_depth = 14.0f;// 8.0f;

		cga::Rectangle* start1 = new cga::Rectangle("Start1", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)50 * 0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), 50, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2 = new cga::Rectangle("Start2", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)50 * 0.5f, offset_y - (float)object_depth*0.5f, 20)), glm::mat4(), 40, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2));
		cga::Rectangle* start3 = new cga::Rectangle("Start3", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)50 * 0.5f, offset_y - (float)object_depth*0.5f, 25)), glm::mat4(), 35, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3));
		cga::Rectangle* start4 = new cga::Rectangle("Start4", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)50 * 0.5f, offset_y - (float)object_depth*0.5f, 30)), glm::mat4(), 25, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
	}
#endif

#if 0
	{ // for building Paris
		float object_width = 28.0f;
		float object_depth = 20.0f;

		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for building Paris2
		float object_width = 28.0f;
		float object_depth = 20.0f;

		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for building Griffith Observatory
		float object_width = 10.0f;
		float object_depth = 10.0f;

		cga::Circle* start1 = new cga::Circle("Start1", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(- (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2 = new cga::Rectangle("Start2", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-3, -15, 0)), glm::mat4(), 6, 10, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2));
		cga::Rectangle* start3 = new cga::Rectangle("Start3", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-12, -14, 0)), glm::mat4(), 9, 6, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3));
		cga::Rectangle* start4 = new cga::Rectangle("Start4", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(3, -14, 0)), glm::mat4(), 9, 6, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
		cga::Circle* start5 = new cga::Circle("Start5", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-14, -13, -0)), glm::mat4(), 4, 4, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start5));
		cga::Circle* start6 = new cga::Circle("Start6", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(10, -13, 0)), glm::mat4(), 4, 4, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start6));

	}
#endif

#if 0
	{ // for Empire State Building
		float object_width = 10.0f;
		float object_depth = 10.0f;

		cga::Rectangle* start1 = new cga::Rectangle("Start1", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-(float)20 * 0.5f, -(float)16 * 0.5f, 0)), glm::mat4(), 20, 16, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2 = new cga::Rectangle("Start2", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-(float)18 * 0.5f, -(float)12 * 0.5f, 10)), glm::mat4(), 18, 12, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2));
		cga::Rectangle* start3 = new cga::Rectangle("Start3", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-(float)14 * 0.5f, -(float)8 * 0.5f, 20)), glm::mat4(), 14, 8, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3));
		cga::Rectangle* start4 = new cga::Rectangle("Start4", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-(float)10 * 0.5f, -(float)6 * 0.5f, 60)), glm::mat4(), 10, 6, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
		cga::Rectangle* start5 = new cga::Rectangle("Start5", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-(float)8 * 0.5f, -(float)4 * 0.5f, 70)), glm::mat4(), 8, 4, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start5));
		cga::Rectangle* start6 = new cga::Rectangle("Start6", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-(float)2 * 0.5f, -(float)2 * 0.5f, 74)), glm::mat4(), 2, 2, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start6));

		cga::Rectangle* start7a = new cga::Rectangle("Start7", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-5.5 - (float)5 * 0.5f, -(float)10 * 0.5f, 20)), glm::mat4(), 5.5, 10, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start7a));
		cga::Rectangle* start7b = new cga::Rectangle("Start7", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(5.5 - (float)5 * 0.5f, -(float)10 * 0.5f, 20)), glm::mat4(), 5.5, 10, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start7b));

		cga::Rectangle* start8a = new cga::Rectangle("Start8", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-4.5 - (float)3 * 0.5f, -(float)8 * 0.5f, 60)), glm::mat4(), 3, 8, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start8a));
		cga::Rectangle* start8b = new cga::Rectangle("Start8", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(4.5 - (float)3 * 0.5f, -(float)8 * 0.5f, 60)), glm::mat4(), 3, 8, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start8b));


	}
#endif

#if 0
	{ // for US Capitol
		float object_width = 10.0f;
		float object_depth = 10.0f;

		cga::Rectangle* start1 = new cga::Rectangle("Start1", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-6, -16, 0)), glm::mat4(), 12, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2a = new cga::Rectangle("Start2", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-19, -6.6, 0)), glm::mat4(), 14, 12, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2a));
		cga::Rectangle* start2b = new cga::Rectangle("Start2", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(5, -6.6, 0)), glm::mat4(), 14, 12, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2b));
		cga::Rectangle* start3a = new cga::Rectangle("Start3", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-30, -16, 0)), glm::mat4(), 12, 24, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3a));
		cga::Rectangle* start3b = new cga::Rectangle("Start3", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(18, -16, 0)), glm::mat4(), 12, 24, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3b));

		cga::Circle* start4 = new cga::Circle("Start4", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-6, -8, 8)), glm::mat4(), 12, 12, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
		cga::Circle* start5 = new cga::Circle("Start5", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-5.5, -7.5, 10.5)), glm::mat4(), 11, 11, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start5));
		cga::Circle* start6 = new cga::Circle("Start6", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-5, -7, 14.5)), glm::mat4(), 10, 10, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start6));
	}
#endif

#if 0
	{ // for Dime building/Chrysler house
		cga::Rectangle* start1 = new cga::Rectangle("Start1", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-16, -10, 0)), glm::mat4(), 32, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2 = new cga::Rectangle("Start2", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-16, -10, 12)), glm::mat4(), 12, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2));
		cga::Rectangle* start3 = new cga::Rectangle("Start3", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-4, -10, 12)), glm::mat4(), 8, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3));
		cga::Rectangle* start4 = new cga::Rectangle("Start4", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(4, -10, 12)), glm::mat4(), 12, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
		cga::Rectangle* start5 = new cga::Rectangle("Start5", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-16, -10, 18)), glm::mat4(), 12, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start5));
		cga::Rectangle* start6 = new cga::Rectangle("Start6", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(4, -10, 18)), glm::mat4(), 12, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start6));
		cga::Rectangle* start7 = new cga::Rectangle("Start7", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-16, -10, 58)), glm::mat4(), 12, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start7));
		cga::Rectangle* start8 = new cga::Rectangle("Start8", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(4, -10, 58)), glm::mat4(), 12, 20, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start8));
	}
#endif

#if 0
	{ // for cylinder
		float object_width = 10.0f;
		float object_depth = 10.0f;

		cga::Circle* start1 = new cga::Circle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-(float)object_width*0.5f, -(float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
	}
#endif

	try {
		cga::Grammar grammar;
		cga::parseGrammar(filename, grammar);
		//system.randomParamValues(grammar);
		system.derive(grammar, true);
		//std::vector<boost::shared_ptr<glutils::Face> > faces;
		faces.clear();
		system.generateGeometry(faces);
		renderManager.addFaces(faces);
		//renderManager.centerObjects();
	} catch (const std::string& ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	} catch (const char* ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	}
	
	// add a ground plane
	/*
	std::vector<Vertex> vertices;
	glutils::drawGrid(100, 100, 2.5, glm::vec4(0.521, 0.815, 0.917, 1), glm::vec4(0.898, 0.933, 0.941, 1), system.modelMat, vertices);
	renderManager.addObject("grid", "", vertices, false);
	*/
	renderManager.updateShadowMap(this, light_dir, light_mvpMatrix);

	updateGL();
}

#if 1
void GLWidget3D::generateBuildingImages(const QString& cga_dir, const QString& output_dir, int image_width, int image_height, bool grayscale, int edge_displacement, bool sketchy_line, bool remove_invalid) {
	if (QDir(output_dir).exists()) {
		QDir(output_dir).removeRecursively();
	}
	QDir().mkpath(output_dir);

	srand(0);
	//renderManager.depthSensitivity = 1.0f;
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	// setup grammars
	std::vector<cga::Grammar> grammars;
	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = QDir(cga_dir).entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		cga::Grammar grammar;
		cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
		grammars.push_back(grammar);
	}
	
	for (int grammar_id = 0; grammar_id < grammars.size(); ++grammar_id) {
		int count = 0;

		printf("#%d: ", grammar_id + 1);

		QString grammar_dir;
		grammar_dir.sprintf("/%02d/", grammar_id + 1);

		QString sub_dir;
		int sub_dir_id = -1;

		if (!QDir(output_dir + grammar_dir).exists()) QDir().mkdir(output_dir + grammar_dir);
		
		QFile file(output_dir + grammar_dir + "parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (int object_width = BUILDING_MASS_MIN_WIDTH; object_width <= BUILDING_MASS_MAX_WIDTH; object_width += 2) {
			for (int object_depth = BUILDING_MASS_MIN_DEPTH; object_depth <= BUILDING_MASS_MAX_DEPTH; object_depth += 2) {
				//for (int pitch_angle = 28; pitch_angle <= 32; pitch_angle += 2) {
					//for (int yaw_angle = -47; yaw_angle <= -43; yaw_angle += 2) {
						for (int offset_x = BUILDING_MASS_MIN_X; offset_x <= BUILDING_MASS_MAX_X; offset_x += 4) {
							for (int offset_y = BUILDING_MASS_MIN_Y; offset_y <= BUILDING_MASS_MAX_Y; offset_y += 4) {
								// change camera view direction
								camera.xrot = 30.0f;// pitch_angle;
								camera.yrot = -45.0f;// yaw_angle;
								camera.zrot = 0.0f;
								camera.fovy = 45;//utils::uniform_rand(40, 50);
								camera.pos = glm::vec3(0, CAMERA_DEFAULT_HEIGHT * cosf(camera.xrot / 180.0f * M_PI), CAMERA_DEFAULT_DEPTH + sinf(camera.xrot / 180.0f * M_PI));
								camera.updatePMatrix(width(), height());
								//camera.updateMVPMatrix();

								for (int k = 0; k < 1; ++k) { // 1 images (parameter values are randomly selected) for each width and height				
									std::vector<float> param_values;

									// generate a building mass
									if (grammar_id != 3) {	// cylinder
										boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1)));
										system.stack.push_back(start);
									}
									else {
										boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_width*0.5f, 0)), glm::mat4(), object_width, object_width, glm::vec3(1, 1, 1)));
										system.stack.push_back(start);
									}

									try {
										param_values = system.randomParamValues(grammars[grammar_id]);
										system.derive(grammars[grammar_id], true);
										std::vector<boost::shared_ptr<glutils::Face> > faces;
										renderManager.removeObjects();
										system.generateGeometry(faces);
										renderManager.addFaces(faces);
									}
									catch (const std::string& ex) {
										std::cout << "ERROR:" << std::endl << ex << std::endl;
									}
									catch (const char* ex) {
										std::cout << "ERROR:" << std::endl << ex << std::endl;
									}

									// render a building
									render();

									QImage img = this->grabFrameBuffer();
									cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
									cv::Mat mat;
									EDLine(source, mat, edge_displacement, sketchy_line, grayscale);

									// 画像を縮小
									utils::resizeImage(mat, cv::Size(image_width, image_height));

									// 画像が有効なら保存
									if (!remove_invalid || isImageValid(mat)) {
										// put depth, width at the begining of the param values array
										param_values.insert(param_values.begin() + 0, (float)(offset_x - BUILDING_MASS_MIN_X) / (float)(BUILDING_MASS_MAX_X - BUILDING_MASS_MIN_X));
										param_values.insert(param_values.begin() + 1, (float)(offset_y - BUILDING_MASS_MIN_Y) / (float)(BUILDING_MASS_MAX_Y - BUILDING_MASS_MIN_Y));
										param_values.insert(param_values.begin() + 2, (float)(object_width - BUILDING_MASS_MIN_WIDTH) / (float)(BUILDING_MASS_MAX_WIDTH - BUILDING_MASS_MIN_WIDTH));
										param_values.insert(param_values.begin() + 3, (float)(object_depth - BUILDING_MASS_MIN_DEPTH) / (float)(BUILDING_MASS_MAX_DEPTH - BUILDING_MASS_MIN_DEPTH));

										if (grammar_id == 4) {	// for cylinder
											param_values[3] = param_values[2];
										}

										// create a subdir (000, 001, ...)
										if (count % 100000 == 0) {
											sub_dir_id++;
											sub_dir.sprintf("%03d/", sub_dir_id);
											if (!QDir(output_dir + grammar_dir + sub_dir).exists()) QDir().mkdir(output_dir + grammar_dir + sub_dir);
										}

										// save the image to a file
										QString filename = output_dir + grammar_dir + sub_dir + QString("%1.png").arg(count, 6, 10, QChar('0'));
										cv::imwrite(filename.toUtf8().constData(), mat);

										printf("\r#%d: %d", grammar_id + 1, count + 1);

										// write all the param values to the file
										for (int pi = 0; pi < param_values.size(); ++pi) {
											if (pi > 0) {
												out << ",";
											}
											out << param_values[pi];
										}
										out << "\n";

										count++;
									}

								}


							}
						}
					//}
				//}
			}
		}

		printf("\n");
		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}
#endif

#if 1
void GLWidget3D::generateRoofImages(const QString& cga_dir, const QString& output_dir, int image_width, int image_height, bool grayscale, int edge_displacement, bool sketchy_line) {
	if (QDir(output_dir).exists()) {
		QDir(output_dir).removeRecursively();
	}
	QDir().mkpath(output_dir);

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	// setup camera
	camera.xrot = 30.0f;// pitch_angle;//35.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
	camera.yrot = -45.0f;// yaw_angle;//-45.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
	camera.zrot = 0.0f;
	camera.fovy = 45;//utils::uniform_rand(40, 50);
	camera.pos = glm::vec3(0, CAMERA_DEFAULT_HEIGHT * cosf(camera.xrot / 180.0f * M_PI), CAMERA_DEFAULT_DEPTH + sinf(camera.xrot / 180.0f * M_PI));
	camera.updatePMatrix(width(), height());

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	// setup grammars
	std::vector<cga::Grammar> grammars;
	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = QDir(cga_dir).entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		cga::Grammar grammar;
		cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
		grammars.push_back(grammar);
	}

	for (int grammar_id = 0; grammar_id < grammars.size(); ++grammar_id) {
		int count = 0;

		printf("#%d: ", grammar_id + 1);

		QString grammar_dir;
		grammar_dir.sprintf("/%02d/", grammar_id + 1);

		QString sub_dir;
		int sub_dir_id = -1;

		if (!QDir(output_dir + grammar_dir).exists()) QDir().mkdir(output_dir + grammar_dir);

		QFile file(output_dir + grammar_dir + "parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = BUILDING_MASS_MIN_WIDTH; object_width <= BUILDING_MASS_MAX_WIDTH; object_width += 4.0f) {
			for (float object_depth = BUILDING_MASS_MIN_DEPTH; object_depth <= BUILDING_MASS_MAX_DEPTH; object_depth += 4.0f) {
				int numSamples = 50;
				for (int k = 0; k < numSamples; ++k) { // 1 images (parameter values are randomly selected) for each width and height
					std::vector<float> param_values;

					// generate a roof base
					if (grammar_id != 5 && grammar_id != 6) { // rectangular base
						boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1)));
						system.stack.push_back(start);
					}
					else {	// circular base
						float radius = (object_width + object_depth) * 0.5f;
						boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Circle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-radius*0.5f, -radius*0.5f, 0)), glm::mat4(), radius, radius, glm::vec3(1, 1, 1)));
						system.stack.push_back(start);
					}

					// generate a roof
					try {
						param_values = system.randomParamValues(grammars[grammar_id]);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// render a roof
					render();

					// replace the edges by sketchy polylines
					QImage img = this->grabFrameBuffer();
					cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
					cv::Mat mat;
					EDLine(source, mat, edge_displacement, sketchy_line, grayscale);

					// 画像を縮小
					utils::resizeImage(mat, cv::Size(image_width, image_height));

					// create a subdir (000, 001, ...)
					if (count % 100000 == 0) {
						sub_dir_id++;
						sub_dir.sprintf("%03d/", sub_dir_id);
						if (!QDir(output_dir + grammar_dir + sub_dir).exists()) QDir().mkdir(output_dir + grammar_dir + sub_dir);
					}

					// save the image to a file
					QString filename = output_dir + grammar_dir + sub_dir + QString("%1.png").arg(count, 6, 10, QChar('0'));
					cv::imwrite(filename.toUtf8().constData(), mat);

					printf("\r#%d: %d", grammar_id + 1, count + 1);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";
							
					count++;
				}
			}
		}

		printf("\n");
		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}
#endif

#if 1
void GLWidget3D::generateWindowImages(const QString& cga_dir, const QString& output_dir, int image_width, int image_height, bool grayscale, int edge_displacement, bool sketchy_line) {
	if (QDir(output_dir).exists()) {
		QDir(output_dir).removeRecursively();
	}
	QDir().mkpath(output_dir);

	srand(0);
	//renderManager.depthSensitivity = 1.0f;
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	// setup camera
	camera.xrot = 90.0f;
	camera.yrot = 0.0f;
	camera.zrot = 0.0f;
	camera.pos = glm::vec3(0, 0, 5.0f);
	camera.updateMVPMatrix();

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	// setup grammars
	std::vector<cga::Grammar> grammars;
	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = QDir(cga_dir).entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		cga::Grammar grammar;
		cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
		grammars.push_back(grammar);
	}

	for (int grammar_id = 0; grammar_id < grammars.size(); ++grammar_id) {
		int count = 0;

		printf("#%d: ", grammar_id + 1);

		QString grammar_dir;
		grammar_dir.sprintf("/%02d/", grammar_id + 1);

		QString sub_dir;
		int sub_dir_id = -1;

		if (!QDir(output_dir + grammar_dir).exists()) QDir().mkdir(output_dir + grammar_dir);

		QFile file(output_dir + grammar_dir + "parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 0.3f; object_width <= 3.0f; object_width += 0.1f) {
			for (float object_height = 0.9f; object_height <= 2.0f; object_height += 0.1f) {
				for (int k = 0; k < 10; ++k) {
					std::vector<float> param_values;

					// generate a window
					boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1)));
					system.stack.push_back(start);

					try {
						param_values = system.randomParamValues(grammars[grammar_id]);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					
					// put a background plane
					std::vector<Vertex> vertices;
					glutils::drawQuad(100, 100, glm::vec4(1, 1, 1, 1), glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(0, 0, -10)), vertices);
					renderManager.addObject("background", "", vertices, true);

					// render a window
					render();

					// replace the edges by sketchy polylines
					QImage img = this->grabFrameBuffer();
					cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
					cv::Mat mat;
					EDLine(source, mat, edge_displacement, sketchy_line, grayscale);

					// 画像を縮小
					utils::resizeImage(mat, cv::Size(image_width, image_height));

					// create a subdir (000, 001, ...)
					if (count % 100000 == 0) {
						sub_dir_id++;
						sub_dir.sprintf("%03d/", sub_dir_id);
						if (!QDir(output_dir + grammar_dir + sub_dir).exists()) QDir().mkdir(output_dir + grammar_dir + sub_dir);
					}

					// save the image to a file
					QString filename = output_dir + grammar_dir + sub_dir + QString("%1.png").arg(count, 6, 10, QChar('0'));
					cv::imwrite(filename.toUtf8().constData(), mat);

					printf("\r#%d: %d", grammar_id + 1, count + 1);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";
					
					count++;
				}
			}
		}

		printf("\n");
		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::generateLedgeImages(const QString& cga_dir, const QString& output_dir, int image_width, int image_height, bool grayscale, int edge_displacement, bool sketchy_line) {
	if (QDir(output_dir).exists()) {
		QDir(output_dir).removeRecursively();
	}
	QDir().mkpath(output_dir);

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	// setup camera
	camera.xrot = 0.0f;
	camera.yrot = 0.0f;
	camera.zrot = 0.0f;
	camera.pos = glm::vec3(0, 0, 3);
	camera.updateMVPMatrix();

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	// setup grammars
	std::vector<cga::Grammar> grammars;
	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = QDir(cga_dir).entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		cga::Grammar grammar;
		cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
		grammars.push_back(grammar);
	}

	for (int grammar_id = 0; grammar_id < grammars.size(); ++grammar_id) {
		int count = 0;

		printf("#%d: ", grammar_id + 1);

		QString grammar_dir;
		grammar_dir.sprintf("/%02d/", grammar_id + 1);

		QString sub_dir;
		int sub_dir_id = -1;

		if (!QDir(output_dir + grammar_dir).exists()) QDir().mkdir(output_dir + grammar_dir);

		QFile file(output_dir + grammar_dir + "parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 3.0f; object_width <= 3.0f; object_width += 0.1f) {
			for (float object_height = 0.3f; object_height <= 1.0f; object_height += 0.1f) {
				for (int k = 0; k < 100; ++k) {
					std::vector<float> param_values;			

					// generate a ledge
					boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), 3.141592f * 0.5f, glm::vec3(0, 1, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1)));
					system.stack.push_back(start);

					try {
						param_values = system.randomParamValues(grammars[grammar_id]);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					
					// render a ledge
					render();

					// replace the edges by sketchy polylines
					QImage img = this->grabFrameBuffer();
					cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
					cv::Mat mat;
					EDLine(source, mat, edge_displacement, sketchy_line, grayscale);

					// 画像を縮小
					utils::resizeImage(mat, cv::Size(image_width, image_height));

					// create a subdir (000, 001, ...)
					if (count % 100000 == 0) {
						sub_dir_id++;
						sub_dir.sprintf("%03d/", sub_dir_id);
						if (!QDir(output_dir + grammar_dir + sub_dir).exists()) QDir().mkdir(output_dir + grammar_dir + sub_dir);
					}

					// save the image to a file
					QString filename = output_dir + grammar_dir + sub_dir + QString("%1.png").arg(count, 6, 10, QChar('0'));
					cv::imwrite(filename.toUtf8().constData(), mat);

					printf("\r#%d: %d", grammar_id + 1, count + 1);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";
					
					count++;
				}
			}
		}

		printf("\n");
		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}
#endif

/**
 * http://www.ceng.anadolu.edu.tr/CV/EDLines/
 */
void GLWidget3D::EDLine(const cv::Mat& source, cv::Mat& result, int edge_replacement, bool sketchy_line, bool grayscale) {
	// extract contour vectors
	std::vector<std::pair<glm::vec2, glm::vec2>> edges;
	utils::extractEdges(source, edges);
	
	if (grayscale) {
		result = cv::Mat(source.rows, source.cols, CV_8U, cv::Scalar(255));
	}
	else {
		result = cv::Mat(source.rows, source.cols, CV_8UC3, cv::Scalar(255, 255, 255));
	}

	for (int i = 0; i < edges.size(); ++i) {
		int x1 = edges[i].first.x + utils::uniform_rand(-edge_replacement, edge_replacement);
		int y1 = edges[i].first.y + utils::uniform_rand(-edge_replacement, edge_replacement);
		int x2 = edges[i].second.x + utils::uniform_rand(-edge_replacement, edge_replacement);
		int y2 = edges[i].second.y + utils::uniform_rand(-edge_replacement, edge_replacement);

		if (sketchy_line) {
			int polyline_index = rand() % style_polylines.size();
			draw2DPolyline(result, glm::vec2(x1, y1), glm::vec2(x2, y2), polyline_index);
		}
		else {
			cv::line(result, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0), 1);
		}
	}
}

void GLWidget3D::draw2DPolyline(cv::Mat& img, const glm::vec2& p0, const glm::vec2& p1, int polyline_index) {
	float theta = atan2(p1.y - p0.y, p1.x - p0.x);
	float scale = glm::length(p1 - p0);

	cv::Mat_<float> R(2, 2);
	R(0, 0) = scale * cosf(theta);
	R(0, 1) = -scale * sinf(theta);
	R(1, 0) = scale * sinf(theta);
	R(1, 1) = scale * cosf(theta);

	cv::Mat_<float> A(2, 1);
	A(0, 0) = p0.x;
	A(1, 0) = p0.y;

	for (int i = 0; i < style_polylines[polyline_index].size() - 1; ++i) {
		cv::Mat_<float> X0(2, 1);
		X0(0, 0) = style_polylines[polyline_index][i].x;
		X0(1, 0) = style_polylines[polyline_index][i].y;
		cv::Mat_<float> T0 = R * X0 + A;

		cv::Mat_<float> X1(2, 1);
		X1(0, 0) = style_polylines[polyline_index][i+1].x;
		X1(1, 0) = style_polylines[polyline_index][i+1].y;
		cv::Mat_<float> T1 = R * X1 + A;

		cv::line(img, cv::Point(T0(0, 0), T0(1, 0)), cv::Point(T1(0, 0), T1(1, 0)), cv::Scalar(0), 1, CV_AA);
	}
}

bool GLWidget3D::isImageValid(const cv::Mat& image) {
	cv::Mat tmp;

	if (image.channels() == 1) {
		cv::reduce(image, tmp, 0, CV_REDUCE_MIN);
		if (tmp.at<uchar>(0, 0) == 0) return false;
		if (tmp.at<uchar>(0, tmp.cols - 1) == 0) return false;

		cv::reduce(image, tmp, 1, CV_REDUCE_MIN);
		if (tmp.at<uchar>(0, 0) == 0) return false;
		if (tmp.at<uchar>(tmp.rows - 1, 0) == 0) return false;

		// at least one pixel has to be black
		cv::reduce(tmp, tmp, 0, CV_REDUCE_MIN);
		if (tmp.at<uchar>(0, 0) == 0) return true;
		else return false;
	}
	else {
		cv::reduce(image, tmp, 0, CV_REDUCE_MIN);
		if (tmp.at<cv::Vec3b>(0, 0)[0] == 0) return false;
		if (tmp.at<cv::Vec3b>(0, tmp.cols - 1)[0] == 0) return false;

		cv::reduce(image, tmp, 1, CV_REDUCE_MIN);
		if (tmp.at<cv::Vec3b>(0, 0)[0] == 0) return false;
		if (tmp.at<cv::Vec3b>(tmp.rows - 1, 0)[0] == 0) return false;

		// at least one pixel has to be black
		cv::reduce(tmp, tmp, 0, CV_REDUCE_MIN);
		if (tmp.at<cv::Vec3b>(0, 0)[0] == 0) return true;
		else return false;
	}
}

#if 1
void GLWidget3D::predictBuildingImages(const QString& cga_dir, const QString& testdata_dir, const QString& classification_dir, const QString& regression_dir, const QString& output_dir) {
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	// setup grammars
	std::vector<cga::Grammar> grammars;
	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = QDir(cga_dir).entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		cga::Grammar grammar;
		cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
		grammars.push_back(grammar);
	}

	// load pretrained models
	Classifier classifier((classification_dir + "/model/deploy.prototxt").toUtf8().constData(), (classification_dir + "/model/train_iter_20000.caffemodel").toUtf8().constData(), (classification_dir + "/data/mean.binaryproto").toUtf8().constData());
	std::vector<Regression*> regressions(6);
	for (int i = 0; i < regressions.size(); ++i) {
		QString deploy_name = regression_dir + QString("/model/deploy_%1.prototxt").arg(i + 1, 2, 10, QChar('0'));
		QString model_name = regression_dir + QString("/model/train_%1_iter_80000.caffemodel").arg(i + 1, 2, 10, QChar('0'));
		regressions[i] = new Regression(deploy_name.toUtf8().constData(), model_name.toUtf8().constData());
	}

	// setup the camera
	camera.xrot = 30.0f;
	camera.yrot = -45.0f;
	camera.zrot = 0.0f;
	camera.fovy = 45;
	camera.pos = glm::vec3(0, CAMERA_DEFAULT_HEIGHT * cosf(camera.xrot / 180.0f * M_PI), CAMERA_DEFAULT_DEPTH + sinf(camera.xrot / 180.0f * M_PI));
	camera.updatePMatrix(width(), height());

	// read the ground truth of parameter values
	std::map<int, std::vector<std::vector<float>>> params_truth;
	for (int i = 0; i < regressions.size(); ++i) {
		params_truth[i] = std::vector<std::vector<float>>();

		std::ifstream in_params((testdata_dir + QString("/images/%1/parameters.txt").arg(i + 1, 2, 10, QChar('0'))).toUtf8().constData());
		while (true) {
			std::string line;
			std::getline(in_params, line);
			if (line.empty()) break;

			std::vector<std::string> strs;
			boost::split(strs, line, boost::is_any_of(","));

			std::vector<float> values;
			for (int j = 0; j < strs.size(); ++j) {
				values.push_back(stof(strs[j]));
			}

			params_truth[i].push_back(values);
		}
	}

	int correct_classification = 0;
	int incorrect_classification = 0;
	std::map<int, std::vector<float>> rmse;
	std::map<int, int> rmse_count;

	// read test data
	std::ifstream in((testdata_dir + "/test.txt").toUtf8().constData());
	int iter = 0;
	while (true) {
		std::cout << "iter: " << iter << std::endl;

		std::string line;
		std::getline(in, line);
		int index = line.find(" ");
		if (index == std::string::npos) break;

		std::string file_path = line.substr(0, index);
		int grammar_id = std::stoi(line.substr(index + 1));
		std::cout << "  grammar_id: " << grammar_id << std::endl;

		// obtain file id
		int index1 = file_path.rfind("/");
		int index2 = file_path.find(".", index1);
		int file_id = stoi(file_path.substr(index1 + 1, index2 - index1 - 1));
		std::cout << "  file_id: " << file_id << std::endl;

		// read the test image
		cv::Mat img = cv::imread((std::string(testdata_dir.toUtf8().constData()) + "/images/" + file_path).c_str());

		// convert the image to grayscale with 128x128 size
		cv::Mat grayImg;
		cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
		cv::resize(grayImg, grayImg, cv::Size(128, 128));
		cv::threshold(grayImg, grayImg, 230, 255, cv::THRESH_BINARY);

		// classification
		std::vector<Prediction> predictions = classifier.Classify(img, grammars.size());
		if (predictions[0].first == grammar_id) correct_classification++;
		else incorrect_classification++;

		// parameter estimation
		std::vector<float> predicted_params = regressions[grammar_id]->Predict(grayImg);

		// 誤差を計算
		if (rmse[grammar_id].size() == 0) {
			rmse[grammar_id].resize(predicted_params.size(), 0);
			rmse_count[grammar_id] = 0;
		}

		for (int i = 0; i < predicted_params.size(); ++i) {
			rmse[grammar_id][i] += SQR(params_truth[grammar_id][file_id][i] - predicted_params[i]);
		}
		rmse_count[grammar_id]++;

		// generate a building mass
		float offset_x = predicted_params[0] * (BUILDING_MASS_MAX_X - BUILDING_MASS_MIN_X) + BUILDING_MASS_MIN_X;
		float offset_y = predicted_params[1] * (BUILDING_MASS_MAX_Y - BUILDING_MASS_MIN_Y) + BUILDING_MASS_MIN_Y;
		float object_width = predicted_params[2] * (BUILDING_MASS_MAX_WIDTH - BUILDING_MASS_MIN_WIDTH) + BUILDING_MASS_MIN_WIDTH;
		float object_depth = predicted_params[3] * (BUILDING_MASS_MAX_DEPTH - BUILDING_MASS_MIN_DEPTH) + BUILDING_MASS_MIN_DEPTH;
		predicted_params.erase(predicted_params.begin(), predicted_params.begin() + 4);
		if (grammar_id != 3) {	// not cylinder
			boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1)));
			system.stack.push_back(start);
		}
		else {
			object_width = (object_width + object_depth) * 0.5f;
			boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_width*0.5f, 0)), glm::mat4(), object_width, object_width, glm::vec3(1, 1, 1)));
			system.stack.push_back(start);
		}

		try {
			system.setParamValues(grammars[grammar_id], predicted_params);
			system.derive(grammars[grammar_id], true);
			std::vector<boost::shared_ptr<glutils::Face> > faces;
			renderManager.removeObjects();
			system.generateGeometry(faces);
			renderManager.addFaces(faces);
		}
		catch (const std::string& ex) {
			std::cout << "ERROR:" << std::endl << ex << std::endl;
		}
		catch (const char* ex) {
			std::cout << "ERROR:" << std::endl << ex << std::endl;
		}

		// render a building
		render();
		QImage rendered_img = this->grabFrameBuffer();
		cv::Mat predicted_img = cv::Mat(rendered_img.height(), rendered_img.width(), CV_8UC4, rendered_img.bits(), rendered_img.bytesPerLine()).clone();

		// make the image grayscale
		cv::cvtColor(predicted_img, predicted_img, cv::COLOR_BGRA2GRAY);

		// resize the predicted image
		utils::resizeImage(predicted_img, img.size());

		// make the predicted image blue
		utils::blueImage(predicted_img);

		// blend the prediction and the input image
		utils::blendImages(predicted_img, img, cv::Scalar(255, 255, 255));
				
		//cv::imwrite((output_dir + QString("/%1_%2_input.png").arg(grammar_id, 2, 10, QChar('0')).arg(iter, 2, 10, QChar('0'))).toUtf8().constData(), img);
		cv::imwrite((output_dir + QString("/%1_%2_pred.png").arg(grammar_id, 2, 10, QChar('0')).arg(iter, 2, 10, QChar('0'))).toUtf8().constData(), predicted_img);

		iter++;
	}

	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Classification accuracy: " << (float)correct_classification / (correct_classification + incorrect_classification) << std::endl;
	std::cout << std::endl;

	// 誤差を計算
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Parameter estimation RMSE:" << std::endl;
	for (int i = 0; i < rmse.size(); ++i) {
		for (int j = 0; j < rmse[i].size(); ++j) {
			if (j > 0) std::cout << ", ";
			rmse[i][j] = sqrt(rmse[i][j] / rmse_count[i]);
			std::cout << rmse[i][j];
		}
		std::cout << std::endl;
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);

	for (int i = 0; i < regressions.size(); ++i) {
		delete regressions[i];
	}
}
#endif

void GLWidget3D::predictRoofImages(const QString& cga_dir, const QString& testdata_dir, const QString& classification_dir, const QString& regression_dir, const QString& output_dir) {
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	// setup grammars
	std::vector<cga::Grammar> grammars;
	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = QDir(cga_dir).entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		cga::Grammar grammar;
		cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
		grammars.push_back(grammar);
	}

	// load pretrained models
	Classifier classifier((classification_dir + "/model/deploy.prototxt").toUtf8().constData(), (classification_dir + "/model/train_iter_20000.caffemodel").toUtf8().constData(), (classification_dir + "/data/mean.binaryproto").toUtf8().constData());
	std::vector<Regression*> regressions(7);
	for (int i = 0; i < regressions.size(); ++i) {
		QString deploy_name = regression_dir + QString("/model/deploy_%1.prototxt").arg(i + 1, 2, 10, QChar('0'));
		QString model_name = regression_dir + QString("/model/train_%1_iter_80000.caffemodel").arg(i + 1, 2, 10, QChar('0'));
		regressions[i] = new Regression(deploy_name.toUtf8().constData(), model_name.toUtf8().constData());
	}

	// setup the camera
	camera.xrot = 30.0f;
	camera.yrot = -45.0f;
	camera.zrot = 0.0f;
	camera.fovy = 45;
	camera.pos = glm::vec3(0, CAMERA_DEFAULT_HEIGHT * cosf(camera.xrot / 180.0f * M_PI), CAMERA_DEFAULT_DEPTH + sinf(camera.xrot / 180.0f * M_PI));
	camera.updatePMatrix(width(), height());

	int correct_classification = 0;
	int incorrect_classification = 0;
	std::map<int, std::vector<float>> rmse;
	std::map<int, int> rmse_count;

	for (int grammar_id = 0; grammar_id < grammars.size(); ++grammar_id) {
		int iter = 0;

		for (float object_width = BUILDING_MASS_MIN_WIDTH; object_width <= BUILDING_MASS_MAX_WIDTH; object_width += 4.0f) {
			for (float object_depth = BUILDING_MASS_MIN_DEPTH; object_depth <= BUILDING_MASS_MAX_DEPTH; object_depth += 4.0f) {
				int numSamples = 1;
				for (int k = 0; k < numSamples; ++k) {
					std::vector<float> params_truth;

					// generate a roof base
					if (grammar_id != 5 && grammar_id != 6) { // rectangular base
						boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1)));
						system.stack.push_back(start);
					}
					else {	// circular base
						float radius = (object_width + object_depth) * 0.5f;
						boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Circle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-radius*0.5f, -radius*0.5f, 0)), glm::mat4(), radius, radius, glm::vec3(1, 1, 1)));
						system.stack.push_back(start);
					}

					// generate a roof
					try {
						params_truth = system.randomParamValues(grammars[grammar_id]);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// render a roof
					render();

					// replace the edges by sketchy polylines
					QImage rendered_img = this->grabFrameBuffer();
					cv::Mat source(rendered_img.height(), rendered_img.width(), CV_8UC4, rendered_img.bits(), rendered_img.bytesPerLine());
					cv::Mat img;
					EDLine(source, img, 4, true, false);

					// resize the image
					utils::resizeImage(img, cv::Size(227, 227));

					// convert the image to grayscale with 128x128 size
					cv::Mat grayImg;
					cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
					utils::resizeImage(grayImg, cv::Size(128, 128));

					// classification
					std::vector<Prediction> predictions = classifier.Classify(img, grammars.size());
					if (predictions[0].first == grammar_id) correct_classification++;
					else incorrect_classification++;

					// parameter estimation
					std::vector<float> predicted_params = regressions[grammar_id]->Predict(grayImg);

					// 誤差を計算
					if (rmse[grammar_id].size() == 0) {
						rmse[grammar_id].resize(predicted_params.size(), 0);
						rmse_count[grammar_id] = 0;
					}

					for (int i = 0; i < predicted_params.size(); ++i) {
						rmse[grammar_id][i] += SQR(params_truth[i] - predicted_params[i]);
					}
					rmse_count[grammar_id]++;

					// generate a roof base
					if (grammar_id != 5 && grammar_id != 6) { // rectangular base
						boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1)));
						system.stack.push_back(start);
					}
					else {	// circular base
						float radius = (object_width + object_depth) * 0.5f;
						boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Circle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-radius*0.5f, -radius*0.5f, 0)), glm::mat4(), radius, radius, glm::vec3(1, 1, 1)));
						system.stack.push_back(start);
					}

					try {
						system.setParamValues(grammars[grammar_id], predicted_params);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// render a building
					render();
					rendered_img = this->grabFrameBuffer();
					cv::Mat predicted_img = cv::Mat(rendered_img.height(), rendered_img.width(), CV_8UC4, rendered_img.bits(), rendered_img.bytesPerLine()).clone();
					cv::cvtColor(predicted_img, predicted_img, cv::COLOR_BGRA2GRAY);
					cv::cvtColor(predicted_img, predicted_img, cv::COLOR_GRAY2BGR);

					// resize the predicted image
					utils::resizeImage(predicted_img, img.size());

					// make the predicted image blue
					utils::blueImage(predicted_img);

					cv::imwrite((output_dir + QString("/%1_%2_input.png").arg(grammar_id, 2, 10, QChar('0')).arg(iter, 6, 10, QChar('0'))).toUtf8().constData(), img);
					cv::imwrite((output_dir + QString("/%1_%2_pred.png").arg(grammar_id, 2, 10, QChar('0')).arg(iter, 6, 10, QChar('0'))).toUtf8().constData(), predicted_img);

					iter++;
				}
			}
		}
	}

	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Classification accuracy: " << (float)correct_classification / (correct_classification + incorrect_classification) << std::endl;
	std::cout << std::endl;

	// 誤差を計算
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Parameter estimation RMSE:" << std::endl;
	for (int i = 0; i < rmse.size(); ++i) {
		for (int j = 0; j < rmse[i].size(); ++j) {
			if (j > 0) std::cout << ", ";
			rmse[i][j] = sqrt(rmse[i][j] / rmse_count[i]);
			std::cout << rmse[i][j];
		}
		std::cout << std::endl;
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);

	for (int i = 0; i < regressions.size(); ++i) {
		delete regressions[i];
	}
}

void GLWidget3D::predictWindowImages(const QString& cga_dir, const QString& testdata_dir, const QString& classification_dir, const QString& regression_dir, const QString& output_dir) {
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	// setup grammars
	std::vector<cga::Grammar> grammars;
	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = QDir(cga_dir).entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		cga::Grammar grammar;
		cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
		grammars.push_back(grammar);
	}

	// load pretrained models
	Classifier classifier((classification_dir + "/model/deploy.prototxt").toUtf8().constData(), (classification_dir + "/model/train_iter_20000.caffemodel").toUtf8().constData(), (classification_dir + "/data/mean.binaryproto").toUtf8().constData());
	std::vector<Regression*> regressions(9);
	for (int i = 0; i < regressions.size(); ++i) {
		QString deploy_name = regression_dir + QString("/model/deploy_%1.prototxt").arg(i + 1, 2, 10, QChar('0'));
		QString model_name = regression_dir + QString("/model/train_%1_iter_80000.caffemodel").arg(i + 1, 2, 10, QChar('0'));
		regressions[i] = new Regression(deploy_name.toUtf8().constData(), model_name.toUtf8().constData());
	}

	// setup camera
	camera.xrot = 90.0f;
	camera.yrot = 0.0f;
	camera.zrot = 0.0f;
	camera.pos = glm::vec3(0, 0, 5.0f);
	camera.updateMVPMatrix();

	int correct_classification = 0;
	int incorrect_classification = 0;
	std::map<int, std::vector<float>> rmse;
	std::map<int, int> rmse_count;

	for (int grammar_id = 0; grammar_id < grammars.size(); ++grammar_id) {
		printf("Grammar #%d:", grammar_id + 1);

		int iter = 0;

		for (float object_width = 0.3f; object_width <= 3.0f; object_width += 0.1f) {
			for (float object_height = 0.9f; object_height <= 2.0f; object_height += 0.1f) {
				int numSamples = 1;
				for (int k = 0; k < numSamples; ++k) {
					printf("\rGrammar #%d: %d", grammar_id + 1, iter + 1);

					std::vector<float> params_truth;

					// generate a window
					boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1)));
					system.stack.push_back(start);

					// generate a roof
					try {
						params_truth = system.randomParamValues(grammars[grammar_id]);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// render a roof
					render();

					// replace the edges by sketchy polylines
					QImage rendered_img = this->grabFrameBuffer();
					cv::Mat source(rendered_img.height(), rendered_img.width(), CV_8UC4, rendered_img.bits(), rendered_img.bytesPerLine());
					cv::Mat img;
					EDLine(source, img, 4, true, false);

					// 画像を縮小
					utils::resizeImage(img, cv::Size(227, 227));

					// convert the image to grayscale of size 128x128
					cv::Mat grayImg;
					cv::cvtColor(img, grayImg, cv::COLOR_BGRA2GRAY);
					utils::resizeImage(grayImg, cv::Size(128, 128));

					// classification
					std::vector<Prediction> predictions = classifier.Classify(img, grammars.size());
					if (predictions[0].first == grammar_id) correct_classification++;
					else incorrect_classification++;

					// parameter estimation
					std::vector<float> predicted_params = regressions[grammar_id]->Predict(grayImg);

					// 誤差を計算
					if (rmse[grammar_id].size() == 0) {
						rmse[grammar_id].resize(predicted_params.size(), 0);
						rmse_count[grammar_id] = 0;
					}

					for (int i = 0; i < predicted_params.size(); ++i) {
						rmse[grammar_id][i] += SQR(params_truth[i] - predicted_params[i]);
					}
					rmse_count[grammar_id]++;

					// generate a window
					boost::shared_ptr<cga::Shape> start2 = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1)));
					system.stack.push_back(start2);

					try {
						system.setParamValues(grammars[grammar_id], predicted_params);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// render a window
					render();
					rendered_img = this->grabFrameBuffer();
					cv::Mat predicted_img = cv::Mat(rendered_img.height(), rendered_img.width(), CV_8UC4, rendered_img.bits(), rendered_img.bytesPerLine()).clone();
					cv::cvtColor(predicted_img, predicted_img, cv::COLOR_BGRA2GRAY);
					cv::cvtColor(predicted_img, predicted_img, cv::COLOR_GRAY2BGR);

					// resize the predicted image
					utils::resizeImage(predicted_img, img.size());

					// make the predicted image blue
					utils::blueImage(predicted_img);

					cv::imwrite((output_dir + QString("/%1_%2_input.png").arg(grammar_id, 2, 10, QChar('0')).arg(iter, 6, 10, QChar('0'))).toUtf8().constData(), img);
					cv::imwrite((output_dir + QString("/%1_%2_pred.png").arg(grammar_id, 2, 10, QChar('0')).arg(iter, 6, 10, QChar('0'))).toUtf8().constData(), predicted_img);

					iter++;
				}
			}
		}

		printf("\n");
	}

	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Classification accuracy: " << (float)correct_classification / (correct_classification + incorrect_classification) << std::endl;
	std::cout << std::endl;

	// 誤差を計算
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Parameter estimation RMSE:" << std::endl;
	for (int i = 0; i < rmse.size(); ++i) {
		for (int j = 0; j < rmse[i].size(); ++j) {
			if (j > 0) std::cout << ", ";
			rmse[i][j] = sqrt(rmse[i][j] / rmse_count[i]);
			std::cout << rmse[i][j];
		}
		std::cout << std::endl;
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);

	for (int i = 0; i < regressions.size(); ++i) {
		delete regressions[i];
	}
}

void GLWidget3D::predictLedgeImages(const QString& cga_dir, const QString& testdata_dir, const QString& classification_dir, const QString& regression_dir, const QString& output_dir) {
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	// setup grammars
	std::vector<cga::Grammar> grammars;
	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = QDir(cga_dir).entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		cga::Grammar grammar;
		cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
		grammars.push_back(grammar);
	}

	// load pretrained models
	Classifier classifier((classification_dir + "/model/deploy.prototxt").toUtf8().constData(), (classification_dir + "/model/train_iter_20000.caffemodel").toUtf8().constData(), (classification_dir + "/data/mean.binaryproto").toUtf8().constData());
	std::vector<Regression*> regressions(4);
	for (int i = 0; i < regressions.size(); ++i) {
		QString deploy_name = regression_dir + QString("/model/deploy_%1.prototxt").arg(i + 1, 2, 10, QChar('0'));
		QString model_name = regression_dir + QString("/model/train_%1_iter_80000.caffemodel").arg(i + 1, 2, 10, QChar('0'));
		regressions[i] = new Regression(deploy_name.toUtf8().constData(), model_name.toUtf8().constData());
	}

	// setup camera
	camera.xrot = 0.0f;
	camera.yrot = 0.0f;
	camera.zrot = 0.0f;
	camera.pos = glm::vec3(0, 0, 3);
	camera.updateMVPMatrix();

	int correct_classification = 0;
	int incorrect_classification = 0;
	std::map<int, std::vector<float>> rmse;
	std::map<int, int> rmse_count;

	for (int grammar_id = 0; grammar_id < grammars.size(); ++grammar_id) {
		int iter = 0;

		for (float object_width = 3.0f; object_width <= 3.0f; object_width += 0.1f) {
			for (float object_height = 0.3f; object_height <= 1.0f; object_height += 0.1f) {
				int numSamples = 1;
				for (int k = 0; k < numSamples; ++k) {
					std::vector<float> params_truth;

					// generate a window
					boost::shared_ptr<cga::Shape> start = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1)));
					system.stack.push_back(start);

					// generate a ledge
					try {
						params_truth = system.randomParamValues(grammars[grammar_id]);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// render a ledge
					render();

					// replace the edges by sketchy polylines
					QImage rendered_img = this->grabFrameBuffer();
					cv::Mat source(rendered_img.height(), rendered_img.width(), CV_8UC4, rendered_img.bits(), rendered_img.bytesPerLine());
					cv::Mat img;
					EDLine(source, img, 4, true, false);

					// resize the image
					utils::resizeImage(img, cv::Size(227, 227));
					utils::resizeImage(img, cv::Size(227, 227));

					// convert the image to grayscale of size 128x128
					cv::Mat grayImg;
					cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
					utils::resizeImage(grayImg, cv::Size(128, 128));

					// classification
					std::vector<Prediction> predictions = classifier.Classify(img, grammars.size());
					if (predictions[0].first == grammar_id) correct_classification++;
					else incorrect_classification++;

					// parameter estimation
					std::vector<float> predicted_params = regressions[grammar_id]->Predict(grayImg);

					// 誤差を計算
					if (rmse[grammar_id].size() == 0) {
						rmse[grammar_id].resize(predicted_params.size(), 0);
						rmse_count[grammar_id] = 0;
					}

					for (int i = 0; i < predicted_params.size(); ++i) {
						rmse[grammar_id][i] += SQR(params_truth[i] - predicted_params[i]);
					}
					rmse_count[grammar_id]++;

					// generate a window
					boost::shared_ptr<cga::Shape> start2 = boost::shared_ptr<cga::Shape>(new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1)));
					system.stack.push_back(start2);

					try {
						system.setParamValues(grammars[grammar_id], predicted_params);
						system.derive(grammars[grammar_id], true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						renderManager.removeObjects();
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// render a building
					render();
					rendered_img = this->grabFrameBuffer();
					cv::Mat predicted_img = cv::Mat(rendered_img.height(), rendered_img.width(), CV_8UC4, rendered_img.bits(), rendered_img.bytesPerLine()).clone();
					cv::cvtColor(predicted_img, predicted_img, cv::COLOR_BGRA2GRAY);
					cv::cvtColor(predicted_img, predicted_img, cv::COLOR_GRAY2BGR);

					// resize the predicted image
					utils::resizeImage(predicted_img, img.size());

					// make the predicted image blue
					utils::blueImage(predicted_img);

					cv::imwrite((output_dir + QString("/%1_%2_input.png").arg(grammar_id, 2, 10, QChar('0')).arg(iter, 6, 10, QChar('0'))).toUtf8().constData(), img);
					cv::imwrite((output_dir + QString("/%1_%2_pred.png").arg(grammar_id, 2, 10, QChar('0')).arg(iter, 6, 10, QChar('0'))).toUtf8().constData(), predicted_img);

					iter++;
				}
			}
		}
	}

	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Classification accuracy: " << (float)correct_classification / (correct_classification + incorrect_classification) << std::endl;
	std::cout << std::endl;

	// 誤差を計算
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Parameter estimation RMSE:" << std::endl;
	for (int i = 0; i < rmse.size(); ++i) {
		for (int j = 0; j < rmse[i].size(); ++j) {
			if (j > 0) std::cout << ", ";
			rmse[i][j] = sqrt(rmse[i][j] / rmse_count[i]);
			std::cout << rmse[i][j];
		}
		std::cout << std::endl;
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);

	for (int i = 0; i < regressions.size(); ++i) {
		delete regressions[i];
	}
}

void GLWidget3D::keyPressEvent(QKeyEvent *e) {
	shiftPressed = false;

	switch (e->key()) {
	case Qt::Key_Shift:
		shiftPressed = true;
		break;
	default:
		break;
	}
}

void GLWidget3D::keyReleaseEvent(QKeyEvent* e) {
	shiftPressed = false;
}