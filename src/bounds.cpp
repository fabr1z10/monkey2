#include "bounds.h"
#include <algorithm>
Bounds Bounds::transform(const glm::mat4 &t) {

	glm::vec4 corners[4] = {
			t * glm::vec4(xm, ym, 0.0f, 1.0f),
			t * glm::vec4(xM, ym, 0.0f, 1.0f),
			t * glm::vec4(xm, yM, 0.0f, 1.0f),
			t * glm::vec4(xM, yM, 0.0f, 1.0f)
	};

	Bounds transformed;
	transformed.xm = std::min({corners[0].x, corners[1].x, corners[2].x, corners[3].x});
	transformed.xM = std::max({corners[0].x, corners[1].x, corners[2].x, corners[3].x});
	transformed.ym = std::min({corners[0].y, corners[1].y, corners[2].y, corners[3].y});
	transformed.yM = std::max({corners[0].y, corners[1].y, corners[2].y, corners[3].y});

	return transformed;

}