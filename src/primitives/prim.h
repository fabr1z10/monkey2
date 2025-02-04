#pragma once

#include "glm/glm.hpp"
#include "vertex.h"

namespace primitives {

	class Line {
	public:
		Line (glm::vec3 A, glm::vec3 B, glm::vec4 color) : A(A), B(B), color(color) {}

		Line(const float*);

		glm::vec3 A;
		glm::vec3 B;
		glm::vec4 color;

		static constexpr GLenum _glPrim = GL_LINES;

		static constexpr int _nVertices = 2;

		static constexpr int _nIndices = 2;

		static constexpr int _shaderType = 1;

		static constexpr int _floatsPerPrimitive = 10;       // 3 coordinates per point + 4 color

		static constexpr int _indices[2] = {0, 1};

		using Vertex = VertexColor;

		void transform(VertexColor*, const glm::mat4& t) const;
	};


	class Triangle {
	public:
	    Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec4 color) :
	        A(A), B(B), C(C), color(color) {}

        Triangle(const float*);

        glm::vec3 A;
	    glm::vec3 B;
	    glm::vec3 C;
	    glm::vec4 color;

	    static constexpr GLenum _glPrim = GL_TRIANGLES;

	    static constexpr int _nVertices = 3;

	    static constexpr int _nIndices = 3;

	    static constexpr int _shaderType = 1;

        static constexpr int _floatsPerPrimitive = 13;

        static constexpr int _indices[3] = {0, 1, 2};

	    using Vertex = VertexColor;

	    void transform(VertexColor*, const glm::mat4& t) const;
	};

    class TriangleNormal {
    public:
        TriangleNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec4 color);

        TriangleNormal(const float*);

        glm::vec3 A;
        glm::vec3 B;
        glm::vec3 C;
        glm::vec3 n;
        glm::vec4 color;

        static constexpr GLenum _glPrim = GL_TRIANGLES;

        static constexpr int _nVertices = 3;

        static constexpr int _nIndices = 3;

        static constexpr int _shaderType = 2;

        static constexpr int _floatsPerPrimitive = 13;

        static constexpr int _indices[3] = {0, 1, 2};

        using Vertex = VertexColorNormal;

        void transform(VertexColorNormal*, const glm::mat4& t) const;
    };




    class Quad {
    public:
        glm::vec4 texCoord;
        glm::vec2 anchor;
        glm::vec2 size;
        int index;

        Quad(glm::vec4 texCoords, glm::vec2 anchor, glm::vec2 size, int);

        Quad(const float*);

        static constexpr GLenum _glPrim = GL_TRIANGLES;

        static constexpr int _nVertices = 4;

        static constexpr int _nIndices = 6;

        static constexpr int _shaderType = 3;

        // 4 texture coordinates, 2 anchor point, and texture index
        static constexpr int _floatsPerPrimitive = 7;

        static constexpr int _indices[6] = {0, 1, 2, 0, 2, 3};

        using Vertex = VertexTexture;

        void transform(VertexTexture*, const glm::mat4& t) const;

        void clear(VertexTexture*);
    };

}