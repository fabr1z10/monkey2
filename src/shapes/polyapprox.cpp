#include "polyapprox.h"
#include "../assetmanager.h"
#include "stb_image.h"


std::vector<glm::vec2> PolyApproximation::approximatePoly(const std::string& filename,
														  glm::ivec4 coords, float epsilon) {
	auto path = Game::instance().getWorkingDirectory() + "/assets/" + filename;
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

	if (!data) {
		throw std::runtime_error("Failed to load image");
	}

	// Extract alpha channel

	auto quadWidth = coords[2];
	auto quadHeight = coords[3];
	std::vector<unsigned char> alpha(quadWidth * quadHeight);
	int k = 0;
	int x0 = coords[0];
	int y0 = coords[1] + quadHeight - 1;
	for (int i = 0; i < quadHeight; ++i) {
		for (int j = 0; j < quadWidth; ++j) {
			alpha[k++] = data[((y0 - i) * width + x0 + j) * 4 + 3];
		}
	}

	stbi_image_free(data);

	// Get contour points
	std::vector<glm::vec2> contour = findContourPoints(alpha.data(), quadWidth, quadHeight);

	auto p = douglasPeucker(contour, epsilon);
	std::cout << " -- epsilon, points: " << epsilon << ", " << p.size() << "\n";

	return p;
	// Binary search for epsilon that gives desired number of vertices
//	float epsilon_min = 0.1f;
//	float epsilon_max = 100.0f;
//	std::vector<glm::vec2> best_result;
//	int best_diff = std::numeric_limits<int>::max();
//
//	float epsilon = 0.1f;
//	for (int i = 0; i < 100; ++i) {
//		//std::vector<bool> keep(contour.size(), false);
//		auto p = douglasPeucker(contour, epsilon); //contour.size()-1, epsilon, keep);
//		//int vertices = std::count(keep.begin(), keep.end(), true);
//
//		std::cout << epsilon << ": " << p.size() << "\n";
//		epsilon *= 1.1f;
//
//	}
//	return {};

//	for (int iteration = 0; iteration < 20; iteration++) {
//		float epsilon = (epsilon_min + epsilon_max) / 2;
//		std::vector<bool> keep(contour.size(), false);
//		keep[0] = keep[contour.size() - 1] = true;
//
//		// Apply Douglas-Peucker algorithm
//		douglasPeucker(contour, 0, contour.size() - 1, epsilon, keep);
//
//		// Count vertices
//		int vertices = std::count(keep.begin(), keep.end(), true);
//
//		// Update binary search
//		if (abs(vertices - target_vertices) < best_diff) {
//			best_diff = abs(vertices - target_vertices);
//			best_result.clear();
//			for (size_t i = 0; i < contour.size(); i++) {
//				if (keep[i]) {
//					best_result.push_back(contour[i]);
//				}
//			}
//		}
//
//		if (vertices > target_vertices) {
//			epsilon_min = epsilon;
//		} else {
//			epsilon_max = epsilon;
//		}
//
//		if (vertices == target_vertices) break;
//	}
//
//	return best_result;
}

std::vector<glm::vec2> PolyApproximation::findContourPoints(unsigned char* alpha, int width, int height) {
	// Create binary image for contour extraction
	std::vector<bool> binary(width * height, false);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			binary[y * width + x] = (alpha[y * width + x] > 127);
		}
	}

	// Use marching squares to trace the contour
	std::vector<glm::vec2> contour;
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

	// Find first edge pixel
	bool found = false;
	int startX = 0, startY = 0;

	for (int y = 1; y < height - 1 && !found; y++) {
		for (int x = 1; x < width - 1 && !found; x++) {
			if (binary[y * width + x] && !binary[y * width + (x-1)]) {
				startX = x;
				startY = y;
				found = true;
			}
		}
	}

	if (!found) {
		// Fallback to simple boundary
		contour.push_back(glm::vec2(0, 0));
		contour.push_back(glm::vec2(width-1, 0));
		contour.push_back(glm::vec2(width-1, height-1));
		contour.push_back(glm::vec2(0, height-1));
		return contour;
	}

	// Directions for tracing: right, down, left, up
	const int dx[] = {1, 0, -1, 0};
	const int dy[] = {0, 1, 0, -1};

	int x = startX, y = startY;
	int dir = 0;  // Start by going right

	do {
		contour.push_back(glm::vec2(x, y));
		visited[y][x] = true;

		// Try to turn left first (counter-clockwise tracing)
		int newDir = (dir + 3) % 4;
		int nx = x + dx[newDir];
		int ny = y + dy[newDir];

		if (nx >= 0 && nx < width && ny >= 0 && ny < height && binary[ny * width + nx]) {
			x = nx;
			y = ny;
			dir = newDir;
			continue;
		}

		// Try to go straight
		newDir = dir;
		nx = x + dx[newDir];
		ny = y + dy[newDir];

		if (nx >= 0 && nx < width && ny >= 0 && ny < height && binary[ny * width + nx]) {
			x = nx;
			y = ny;
			continue;
		}

		// Try to turn right
		newDir = (dir + 1) % 4;
		nx = x + dx[newDir];
		ny = y + dy[newDir];

		if (nx >= 0 && nx < width && ny >= 0 && ny < height && binary[ny * width + nx]) {
			x = nx;
			y = ny;
			dir = newDir;
			continue;
		}

		// Turn around
		dir = (dir + 2) % 4;

	} while (!(x == startX && y == startY) && contour.size() < width * height);

	// Remove duplicates while preserving order
	if (!contour.empty()) {
		std::vector<glm::vec2> uniqueContour;
		uniqueContour.push_back(contour[0]);

		for (size_t i = 1; i < contour.size(); i++) {
			if (!(contour[i] == contour[i-1])) {
				uniqueContour.push_back(contour[i]);
			}
		}

		contour = uniqueContour;
	}

	// Ensure the contour is closed
	if (!contour.empty() && !(contour.front() == contour.back())) {
		contour.push_back(contour.front());
	}

	return contour;
}

// Douglas-Peucker algorithm for polygon approximation
std::vector<glm::vec2> PolyApproximation::douglasPeucker(const std::vector<glm::vec2>& points, float epsilon) {
	if (points.size() < 3) return points;

	// Find the point with the maximum distance
	float dmax = 0;
	size_t index = 0;

	for (size_t i = 1; i < points.size() - 1; i++) {
		float d = pointToLineDistance(points[i], points.front(), points.back());
		if (d > dmax) {
			index = i;
			dmax = d;
		}
	}

	std::vector<glm::vec2> result;

	// If max distance is greater than epsilon, recursively simplify
	if (dmax > epsilon) {
		// Recursive call
		std::vector<glm::vec2> first_half(points.begin(), points.begin() + index + 1);
		std::vector<glm::vec2> second_half(points.begin() + index, points.end());

		std::vector<glm::vec2> r1 = douglasPeucker(first_half, epsilon);
		std::vector<glm::vec2> r2 = douglasPeucker(second_half, epsilon);

		// Combine results
		result.insert(result.end(), r1.begin(), r1.end() - 1);
		result.insert(result.end(), r2.begin(), r2.end());
	} else {
		result.push_back(points.front());
		result.push_back(points.back());
	}

	return result;
}

// Helper function to compute distance from point to line segment
float PolyApproximation::pointToLineDistance(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b) {
	if (a.x == b.x && a.y == b.y) return std::sqrt((p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y));

	float t = ((p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y)) /
			  ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));

	if (t < 0) t = 0;
	if (t > 1) t = 1;

	float dx = p.x - (a.x + t * (b.x - a.x));
	float dy = p.y - (a.y + t * (b.y - a.y));

	return std::sqrt(dx * dx + dy * dy);
}