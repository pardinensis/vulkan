#include "geometry_utils.hpp"

Sphere constructBoundingSphere(const std::vector<glm::vec3>& points, unsigned int startIndex) {
	if (points.size() == 0) {
		throw std::runtime_error("the bounding sphere of an empty set of points is not defined");
	}

	if (points.size() == 1) {
		return Sphere(points[0], 0.0f);
	}

	// p2 := the furthest point from p1
	glm::vec3 p1 = points[startIndex];
	glm::vec3 p2;
	float maxDist2 = -1.0f;
	for (const glm::vec3& p : points) {
		float dist2 = glm::distance2(p, p1);
		if (dist2 > maxDist2) {
			maxDist2 = dist2;
			p2 = p;
		}
	}

	// p1 := the furthest point from p2
	maxDist2 = -1.0f;
	for (const glm::vec3& p : points) {
		float dist2 = glm::distance2(p, p2);
		if (dist2 > maxDist2) {
			maxDist2 = dist2;
			p1 = p;
		}
	}

	// increase the radius to cover all points
	glm::vec3 center = (p1 + p2) * 0.5f;
	float radius2 = maxDist2 * 0.5f;
	for (const glm::vec3& p : points) {
		float dist2 = glm::distance2(p, center);
		if (dist2 > radius2) {
			radius2 = dist2;
		}
	}

	return Sphere(center, radius2);
}

unsigned int countNumberOfPointsInsideSphere(const std::vector<glm::vec3>& points, Sphere sphere) {
	unsigned int nPoints = 0;
	for (const glm::vec3& p : points) {
		if (glm::distance2(p, sphere.center) <= sphere.radius2 + EPSILON) {
			++nPoints;
		}
	}
	return nPoints;
}

float guessNeighborhoodRadius(const std::vector<glm::vec3>& points, unsigned int averagePointsInNeighborhood, float initialRadius) {
	const unsigned int nSamples = 10;
	const int nIterations = 10;

	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distribution(0, points.size() - 1);

	std::vector<glm::vec3> samplePoints;
	for (unsigned sample = 0; sample < nSamples; ++sample) {
		unsigned int index = distribution(generator);
		samplePoints.push_back(points.at(index));
	}

	float radius2 = initialRadius * initialRadius;
	for (unsigned iteration = 0; iteration < nIterations; ++iteration) {
		float neighborhood = 0;
		for (const glm::vec3& point : samplePoints) {
			neighborhood += countNumberOfPointsInsideSphere(points, Sphere(point, radius2));
		}
		if (neighborhood < 1.0f) {
			neighborhood = 1.0f; // avoid division by 0
		}
		neighborhood /= nSamples;

		float radiusFactor = averagePointsInNeighborhood / neighborhood;
		radius2 *= radiusFactor;
		if (radiusFactor < 1.1f && radiusFactor > 0.9f) {
			break;
		}
	}

	return std::sqrt(radius2);
}

bool limitNumberOfPoints(std::vector<glm::vec3>& positions, unsigned int maxNumberOfPoints) {
	if (positions.size() > maxNumberOfPoints) {
		std::random_shuffle(positions.begin(), positions.end());
		positions.resize(maxNumberOfPoints);
		return true;
	}
	return false;
}

bool limitNumberOfPoints(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, unsigned int maxNumberOfPoints) {
	if (positions.size() != normals.size()) {
		throw std::runtime_error("positions and normals differ in size");
	}
	if (positions.size() <= maxNumberOfPoints) {
		return false;
	}

	std::vector<std::pair<glm::vec3, glm::vec3>> vertices;
	for (int i = 0; i < positions.size(); ++i) {
		vertices.push_back(std::make_pair(positions[i], normals[i]));
	}

	std::random_shuffle(vertices.begin(), vertices.end());

	positions.resize(maxNumberOfPoints);
	normals.resize(maxNumberOfPoints);
	for (int i = 0; i < maxNumberOfPoints; ++i) {
		positions[i] = vertices[i].first;
		normals[i] = vertices[i].second;
	}
	return true;
}

void centerPoints(std::vector<glm::vec3>& points) {
	glm::vec3 center;
	for (const glm::vec3& p : points) {
		center += p;
	}
	center *= 1.0f / points.size();

	for (glm::vec3& p : points) {
		p -= center;
	}
}

glm::vec3 transform(glm::vec3 v, glm::mat4 m) {
	glm::vec4 v4(v.x, v.y, v.z, 1.0f);
	glm::vec4 vt = m * v4;
	float inv_w = 1.0f / vt.w;
	return glm::vec3(vt.x * inv_w, vt.y * inv_w, vt.z * inv_w);
}

glm::vec3 transformNormal(glm::vec3 v, glm::mat4 m) {
	glm::vec4 v4(v.x, v.y, v.z, 0.0f);
	glm::vec4 vt = m * v4;
	return glm::vec3(vt.x, vt.y, vt.z);	
}

void exportPointCloud(const std::vector<glm::vec3>& positions, const std::string& filename, glm::mat4 trafo) {
	std::ofstream os(filename);
	for (const glm::vec3& v : positions) {
		glm::vec3 transformed = transform(v, trafo);
		os << "v " << transformed.x << " " << transformed.y << " " << transformed.z << std::endl;
	}
	os.close();
}


void exportPointCloud(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::string& filename, glm::mat4 trafo) {
	std::ofstream os(filename);
	for (const glm::vec3& v : positions) {
		glm::vec3 transformed = transform(v, trafo);
		os << "v " << transformed.x << " " << transformed.y << " " << transformed.z << std::endl;
	}
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(trafo));
	for (const glm::vec3& n : normals) {
		glm::vec3 transformed = transformNormal(n, normalMatrix);
		os << "vn " << transformed.x << " " << transformed.y << " " << transformed.z << std::endl;
	}
	os.close();
}