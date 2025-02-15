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

Quad::Quad(glm::vec4 texCoords, glm::vec2 anchor, glm::vec2 size, int texIndex)
    : texCoord(texCoords), anchor(anchor), size(size), index(texIndex) {}

Quad::Quad(const float* data) {
    this->texCoord = glm::vec4(data[0], data[1], data[2], data[3]);
    this->anchor = glm::vec2(data[4], data[5]);
    this->size = glm::vec2(data[6], data[7]);
    this->index = (int)data[8];
}

Quad::Quad(const float * data, float invw, float invh) {
    this->index = static_cast<int>(data[6]);
    this->texCoord = glm::vec4(data[0] * invw, data[1] * invh, data[2] * invw, data[3] * invh);
    this->anchor = glm::vec2(data[4], data[5]);
    this->size = {data[2], data[3]};
}


Quad::Quad(const float * data, float invw, float invh, int texId) {
    this->index = texId;
    this->texCoord = glm::vec4(data[0] * invw, data[1] * invh, data[2] * invw, data[3] * invh);
    this->anchor = glm::vec2(data[4], data[5]);
    this->size = {data[2], data[3]};
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

void Quad::transform(VertexTexture * v, const glm::mat4 &t) const {
    auto bottomLeft = glm::vec3(t * glm::vec4(-anchor, 0.f, 1.f));
    auto xAxis = t[0];
    auto yAxis = t[1];


    v->position = bottomLeft;
    v->texCoord = glm::vec2(texCoord[0], texCoord[1] + texCoord[3]);
    v->texIndex = index;

    (v+1)->position = bottomLeft + glm::vec3(size.x * xAxis.x, 0.f, 0.f);
    (v+1)->texCoord = glm::vec2(texCoord[0]+ texCoord[2], texCoord[1] + texCoord[3]);
    (v+1)->texIndex = index;

    (v+2)->position = bottomLeft + glm::vec3(size.x * xAxis.x, size.y * yAxis.y, 0.f);
    (v+2)->texCoord = glm::vec2(texCoord[0] + texCoord[2], texCoord[1]);
    (v+2)->texIndex = index;

    (v+3)->position = bottomLeft + glm::vec3(0.f, size.y * yAxis.y, 0.f);
    (v+3)->texCoord = glm::vec2(texCoord[0], texCoord[1]);
    (v+3)->texIndex = index;

}

void Quad::clear(VertexTexture * v) {
    v->texIndex = -1;
    (v+1)->texIndex = -1;
    (v+2)->texIndex = -1;
    (v+3)->texIndex = -1;

}

