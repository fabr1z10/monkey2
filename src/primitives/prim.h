#pragma once

#include "glm/glm.hpp"
#include "vertex.h"

namespace primitives {

	class Line {
	public:
        Line (glm::vec3 A, glm::vec3 B) : A(A), B(B) {}

		Line(const float*);

		glm::vec3 A;
		glm::vec3 B;		

		static constexpr GLenum _glPrim = GL_LINES;

		static constexpr int _nVertices = 2;

		static constexpr int _nIndices = 2;

		static constexpr int _shaderType = 1;

        static constexpr int _floatsPerPrimitive = 6;       // 2 points, 3 coordinates per point

		static constexpr int _indices[2] = {0, 1};

		using Vertex = VertexColor;

        void transform(VertexColor*, const glm::mat4& t, glm::vec4 color) const;
	};


	class Triangle {
	public:
        Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C) :
            A(A), B(B), C(C) {}

        Triangle(const float*);

        glm::vec3 A;
	    glm::vec3 B;
	    glm::vec3 C;
        //glm::vec4 color;

	    static constexpr GLenum _glPrim = GL_TRIANGLES;

	    static constexpr int _nVertices = 3;

	    static constexpr int _nIndices = 3;

	    static constexpr int _shaderType = 1;

        static constexpr int _floatsPerPrimitive = 9;

        static constexpr int _indices[3] = {0, 1, 2};

	    using Vertex = VertexColor;

        void transform(VertexColor*, const glm::mat4& t, glm::vec4 color) const;
	};

    class TriangleNormal {
    public:
        TriangleNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C);

        TriangleNormal(const float*);

        glm::vec3 A;
        glm::vec3 B;
        glm::vec3 C;
        glm::vec3 n;
        //glm::vec4 color;

        static constexpr GLenum _glPrim = GL_TRIANGLES;

        static constexpr int _nVertices = 3;

        static constexpr int _nIndices = 3;

        static constexpr int _shaderType = 2;

        static constexpr int _floatsPerPrimitive = 9;

        static constexpr int _indices[3] = {0, 1, 2};

        using Vertex = VertexColorNormal;

        void transform(VertexColorNormal*, const glm::mat4& t, glm::vec4 color) const;
    };




    class Quad {
    public:
        glm::vec4 texCoord;
        glm::vec2 anchor;
        glm::vec2 size;
        int index;
        float z;
        // color is provided by the renderer
        //glm::vec4 color;

        Quad(glm::vec4 texCoords, glm::vec2 anchor, glm::vec2 size, int, float z=0.f);

        /* Constructs a quad from raw data - requires the inverse
         * of the size of the texture
         */
        Quad(const float*);
        Quad(const float*, float invw, float invh);
        Quad(const float*, float invw, float invh, int texId);

        static constexpr GLenum _glPrim = GL_TRIANGLES;

        static constexpr int _nVertices = 4;

        static constexpr int _nIndices = 6;

        static constexpr int _shaderType = 3;

        // 4 texture coordinates, 2 anchor point, and texture index
        static constexpr int _floatsPerPrimitive = 9;

        static constexpr int _indices[6] = {0, 1, 2, 0, 2, 3};

        using Vertex = VertexTexture;

        void transform(VertexTexture*, const glm::mat4& t, glm::vec4 color,
            bool fliph = false, bool flipv = false) const;

        void clear(VertexTexture*);
    };

}
