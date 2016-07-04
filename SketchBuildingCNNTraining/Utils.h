#pragma once

#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace utils {

	double uniform_rand();
	double uniform_rand(double a, double b);
	float gause(float u, float sigma);

	void drawLine(cv::Mat& img, cv::Point p1, cv::Point p2, float edge_prob, int noise_amount, const cv::Scalar& color, int thickness);
	void drawRectangle(cv::Mat& img, cv::Point p1, cv::Point p2, float edge_prob, int noise_amount, const cv::Scalar& color, int thickness);

	void extractEdges(const cv::Mat& img, std::vector<std::pair<glm::vec2, glm::vec2>>& edges);

	void resizeImage(cv::Mat& img, const cv::Size& size);
	void blueImage(cv::Mat& img);
	void blendImages(cv::Mat& img1, const cv::Mat& img2, const cv::Scalar& transparent_color);
}