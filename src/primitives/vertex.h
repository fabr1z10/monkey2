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