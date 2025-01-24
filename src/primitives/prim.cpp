#include "prim.h"

using namespace primitives;

void Line::transform(VertexColor * v, const glm::mat4 &t) const {
	v->A = glm::vec3(t * glm::vec4(A, 1.f));
	v->color = color;

	(v+1)->A = glm::vec3(t * glm::vec4(B, 1.f));
	(v+1)->color = color;


}

