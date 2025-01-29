#pragma once

#include <GL/glew.h>

struct VertexColor {
	glm::vec3 A;
	glm::vec4 color;

	static void setupVertices() {

		auto stride = sizeof(VertexColor);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (const void *) offsetof(VertexColor, color));
	}
};

struct VertexColorNormal {
    glm::vec3 A;
    glm::vec4 color;
    glm::vec3 normal;

    static void setupVertices() {

        auto stride = sizeof(VertexColorNormal);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (const void *) offsetof(VertexColorNormal, color));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (const void *) offsetof(VertexColorNormal, normal));
    }
};

struct VertexTexture {
public:
    glm::vec3 position;
    glm::vec2 texCoord;
    // Each quad (sprite) needs to know which texture to use, so we add an integer texIndex to the vertex attributes:
    int texIndex;

    static void setupVertices() {

        auto stride = sizeof(VertexTexture);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void *) offsetof(VertexTexture, texCoord));

        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(2, 1, GL_INT, stride, (const void *) offsetof(VertexTexture, texIndex));



    }
};