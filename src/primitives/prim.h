#pragma once

#include "glm/glm.hpp"
#include "vertex.h"

namespace primitives {

	class Line {
	public:
		Line (glm::vec3 A, glm::vec3 B, glm::vec4 color) : A(A), B(B), color(color) {}
		glm::vec3 A;
		glm::vec3 B;
		glm::vec4 color;

		static constexpr GLenum _glPrim = GL_LINES;

		static constexpr int _nVertices = 2;

		static constexpr int _shaderType = 1;

		using Vertex = VertexColor;

		void transform(VertexColor*, const glm::mat4& t) const;
	};
}