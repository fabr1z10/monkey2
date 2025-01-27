#include "prim.h"

using namespace primitives;

Line::Line(const float* data) {
	this->A = glm::vec3(data[0], data[1], data[2]);
	this->B = glm::vec3(data[3], data[4], data[5]);
	this->color = glm::vec4(data[6], data[7], data[8], data[9]);

}

Triangle::Triangle(const float * data) {
	this->A = glm::vec3(data[0], data[1], data[2]);
	this->B = glm::vec3(data[3], data[4], data[5]);
	this->C = glm::vec3(data[6], data[7], data[8]);
    this->color = glm::vec4(data[9], data[10], data[11], data[12]);

}

TriangleNormal::TriangleNormal(const float * data) {
    this->A = glm::vec3(data[0], data[1], data[2]);
    this->B = glm::vec3(data[3], data[4], data[5]);
    this->C = glm::vec3(data[6], data[7], data[8]);
    this->color = glm::vec4(data[9], data[10], data[11], data[12]);
    this->n = glm::normalize(glm::cross(B - A, C - A));

}

void Line::transform(VertexColor * v, const glm::mat4 &t) const {
	v->A = glm::vec3(t * glm::vec4(A, 1.f));
	v->color = color;

	(v+1)->A = glm::vec3(t * glm::vec4(B, 1.f));
	(v+1)->color = color;


}

void Triangle::transform(VertexColor * v, const glm::mat4 &t) const {
	v->A = glm::vec3(t * glm::vec4(A, 1.f));
	v->color = color;

	(v+1)->A = glm::vec3(t * glm::vec4(B, 1.f));
	(v+1)->color = color;

	(v+2)->A = glm::vec3(t * glm::vec4(C, 1.f));
	(v+2)->color = color;

}

void TriangleNormal::transform(VertexColorNormal * v, const glm::mat4 &t) const {
    auto normalMat = glm::transpose(glm::inverse(glm::mat3(t)));
    glm::vec3 normal = normalMat * n;

    v->A = glm::vec3(t * glm::vec4(A, 1.f));
    v->color = color;
    v->normal = normal;

    (v+1)->A = glm::vec3(t * glm::vec4(B, 1.f));
    (v+1)->color = color;
    (v+1)->normal = normal;

    (v+2)->A = glm::vec3(t * glm::vec4(C, 1.f));
    (v+2)->color = color;
    (v+2)->normal = normal;

}

