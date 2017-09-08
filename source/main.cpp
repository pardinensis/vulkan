#include "app.hpp"

#include "common_includes.hpp"
#include "geometry_utils.hpp"
#include "registrator.hpp"

#include <pcl/common/transforms.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/fpfh_omp.h>

typedef pcl::PointXYZ point_t;
typedef pcl::Normal normal_t;
typedef pcl::FPFHSignature33 feature_t;
typedef pcl::PointCloud<point_t>::Ptr points_t;
typedef pcl::PointCloud<normal_t>::Ptr normals_t;
typedef pcl::PointCloud<feature_t>::Ptr features_t;

std::vector<glm::vec3> take3DScreenshot(const App& app) {
	// download depth image and unproject
	std::vector<float> depthData = app.depthResource->downloadDepthImage();
	glm::mat4 projectionMatrix = app.projection->getProjectionMatrix();
	glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);
	VkExtent2D extent = app.swapchain->getVkExtent();

	std::vector<glm::vec3> positions;
	for (unsigned int yPixel = 0; yPixel < extent.height; ++yPixel) {
		const float y = yPixel / (float) extent.height;
		for (unsigned int xPixel = 0; xPixel < extent.width; ++xPixel) {
			const float x = xPixel / (float) extent.width;
			const float z = depthData[yPixel * extent.width + xPixel];
			if (z < 1.0f) {
				glm::vec3 viewSpacePoint = glm::unProject(glm::vec3(xPixel, yPixel, z), glm::mat4(),
					projectionMatrix, glm::vec4(0, 0, extent.width, extent.height));
				positions.push_back(viewSpacePoint);
			}
		}
	}

	return positions;

	// save depth image as obj
	// std::ofstream os("../screenshots/bunny.obj");
	// for (const glm::vec3& v : positions) {
	// 	os << "v " << v.x << " " << v.y << " " << v.z << std::endl;
	// }
	// os.close();

	// convert to pcl
	// points_t screenshotVertices(new pcl::PointCloud<point_t>());
	// for (const glm::vec3 v : positions) {
	// 	screenshotVertices->push_back(point_t(v.x, v.y, v.z));
	// }
	// return screenshotVertices;
}

normals_t estimateNormals(points_t points, float normalEstimationRadius,const glm::vec3& viewPoint) {
	pcl::NormalEstimation<point_t, normal_t> ne;
	normals_t normals(new pcl::PointCloud<normal_t>());
	ne.setInputCloud(points);
	pcl::search::KdTree<point_t>::Ptr tree(new pcl::search::KdTree<point_t>());
	ne.setSearchMethod(tree);
	ne.setRadiusSearch(normalEstimationRadius);
	ne.setViewPoint(viewPoint.x, viewPoint.y, viewPoint.z);
	ne.compute(*normals);

	return normals;
}

features_t generateFeatures(points_t points, normals_t normals, float normalEstimationRadius) {
	pcl::FPFHEstimationOMP<point_t, normal_t, feature_t> fpfh;
	features_t features(new pcl::PointCloud<feature_t>());
	fpfh.setRadiusSearch(1.5 * normalEstimationRadius);
	fpfh.setInputCloud(points);
	fpfh.setInputNormals(normals);
	fpfh.compute(*features);

	return features;
}

void writeFeaturesToFile(points_t points, features_t features, const std::string& filename, int n_points = -1) {
	std::ofstream os(filename, std::ofstream::out | std::ofstream::binary);
	if (n_points < 0) n_points = points->size();

	int n_dim = 33;
	os.write(reinterpret_cast<const char*>(&n_points), sizeof(int));
	os.write(reinterpret_cast<const char*>(&n_dim), sizeof(int));
	for (int i = 0; i < n_points; ++i) {
		const point_t& p = points->at(i);
		float xyz[3] = {p.x, p.y, p.z};
		os.write(reinterpret_cast<const char*>(xyz), 3 * sizeof(float));

		const feature_t& f = features->at(i);
		os.write(reinterpret_cast<const char*>(f.histogram), 33 * sizeof(float));
	}
	os.close();
}

points_t convertPositionsToPCL(const std::vector<glm::vec3>& vertices) {
	points_t verticesPCL(new pcl::PointCloud<point_t>());
	for (const glm::vec3& v : vertices) {
		verticesPCL->push_back(point_t(v.x, v.y, v.z));
	}
	return verticesPCL;
}

normals_t convertNormalsToPCL(const std::vector<glm::vec3>& normals) {
	normals_t normalsPCL(new pcl::PointCloud<normal_t>());
	for (const glm::vec3& n : normals) {
		normalsPCL->push_back(normal_t(n.x, n.y, n.z));
	}
	return normalsPCL;
}

void matchScreenshot(const App& app) {
	const unsigned int MODEL_MAX_N_VERTICES = 25000;
	const unsigned int SCREENSHOT_MAX_N_VERTICES = 10000;

	// get screenshot
	std::vector<glm::vec3> screenshotVertices = take3DScreenshot(app);

	// get model
	std::vector<glm::vec3> modelVertices;
	std::vector<glm::vec3> modelNormals;
	const std::vector<Vertex>& vertices = app.model->getVertexData();
	for (const Vertex& v : vertices) {
		modelVertices.push_back(v.position);
		modelNormals.push_back(v.normal);
	}

	limitNumberOfPoints(modelVertices, modelNormals, MODEL_MAX_N_VERTICES);
	limitNumberOfPoints(screenshotVertices, SCREENSHOT_MAX_N_VERTICES);

	exportPointCloud(screenshotVertices, "../out/screenshot.obj");
	exportPointCloud(modelVertices, modelNormals, "../out/model.obj");

	glm::mat4 cameraTrafo = app.camera->getViewMatrix();
	glm::vec3 cameraPosition = app.camera->getPos();
	exportPointCloud(modelVertices, "../out/transformed_model.obj", cameraTrafo);

	const float normalEstimationRadius = 0.045;

	points_t modelVerticesPCL = convertPositionsToPCL(modelVertices);
	normals_t modelNormalsPCL = convertNormalsToPCL(modelNormals);
	features_t modelFeatures = generateFeatures(modelVerticesPCL, modelNormalsPCL, normalEstimationRadius);

	points_t screenshotVerticesPCL = convertPositionsToPCL(screenshotVertices);
	normals_t screenshotNormalsPCL = estimateNormals(screenshotVerticesPCL, normalEstimationRadius, cameraPosition);
	features_t screenshotFeatures = generateFeatures(screenshotVerticesPCL, screenshotNormalsPCL, normalEstimationRadius);

	writeFeaturesToFile(modelVerticesPCL, modelFeatures, "model.bin");
	writeFeaturesToFile(screenshotVerticesPCL, screenshotFeatures, "screenshot.bin");

	std::cout << (*screenshotFeatures)[0] << std::endl;


	CApp capp;
	capp.ReadFeature("screenshot.bin");
	capp.ReadFeature("model.bin");
	capp.NormalizePoints();
	capp.AdvancedMatching();
	capp.OptimizePairwise(true, 64);
	glm::mat4 estimatedTrafo = glm::make_mat4(capp.GetTrans().data());

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(estimatedTrafo, scale, rotation, translation, skew, perspective);
	std::cout << " ----- estimation ----- " << std::endl;
	std::cout << "translation = " << glm::to_string(translation) << std::endl;
	std::cout << "axis / angle = " << glm::to_string(glm::axis(rotation)) << " / " << glm::degrees(glm::angle(rotation)) << std::endl;

	glm::decompose(cameraTrafo, scale, rotation, translation, skew, perspective);
	std::cout << " ----- truth ----- " << std::endl;
	std::cout << "translation = " << glm::to_string(translation) << std::endl;
	std::cout << "axis / angle = " << glm::to_string(glm::axis(rotation)) << " / " << glm::degrees(glm::angle(rotation)) << std::endl;

	std::cout << std::endl;
}

int main() {
	App app;

	try {
		app.init("");

		app.mainWindow->registerMousePressedHandler([app](MouseButton button, int x, int y) -> bool {
			if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				matchScreenshot(app);
			}
		});

		app.run();
		app.cleanup();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}