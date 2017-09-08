#pragma once

#include "common_includes.hpp"

const float EPSILON = 0.00001f;

struct Sphere {
	glm::vec3 center;
	float radius2;

	Sphere() : center(NAN, NAN, NAN), radius2(NAN) {}
	Sphere(const glm::vec3 center, float radius2) : center(center), radius2(radius2) {}

	inline float radius() const { return std::sqrt(radius2); }
};

// Computes a close to the smallest sphere that contains all given points
// Algorithm of J. Ritter, 1990
// Runtime complexity: O(n)
Sphere constructBoundingSphere(const std::vector<glm::vec3>& points, unsigned int startIndex = 0);

// Returns the number of points that lie inside the given sphere
// Runtime complexity: O(n)
unsigned int countNumberOfPointsInsideSphere(const std::vector<glm::vec3>& points, Sphere sphere);

// Returns a radius such that a sphere of that radius around any point contains an average of x other points
// This function only gives a very rough estimation that may vary a lot from call to call (also because of the use of randomness)
// Runtime complexity: worst case O(n^2), average case O(n)
float guessNeighborhoodRadius(const std::vector<glm::vec3>& points, unsigned int averagePointsInNeighborhood, float initialRadius = 0.2f);

bool limitNumberOfPoints(std::vector<glm::vec3>& positions, unsigned int maxNumberOfPoints);
bool limitNumberOfPoints(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, unsigned int maxNumberOfPoints);

void centerPoints(std::vector<glm::vec3>& points);


void exportPointCloud(const std::vector<glm::vec3>& positions, const std::string& filename, glm::mat4 trafo = glm::mat4());
void exportPointCloud(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::string& filename, glm::mat4 trafo = glm::mat4());


