#pragma once

#include <GL/glew.h>

struct VertexColor {
	glm::vec3 position;
	glm::vec4 color;
	int hide = 0;


	static void setupVertices() {

		auto stride = sizeof(VertexColor);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (const void *) offsetof(VertexColor, color));

		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 1, GL_INT, stride, (const void *) offsetof(VertexColor, hide));

	}

	void show(bool value) {
		hide = value ? 0 : 1;
	}};

struct VertexColorNormal {
    glm::vec3 A;
    glm::vec4 color;
    glm::vec3 normal;
	void show(bool) {}
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

struct VertexPalette {
public:
	glm::vec3 position;
	glm::vec2 texCoord;
	int paletteId;
	int hide = 0;

	//glm::vec4 color;
	void show(bool value) {
		hide = value ? 0 : 1;
	}

	static void setupVertices() {

		auto stride = sizeof(VertexPalette);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void *) offsetof(VertexPalette, texCoord));

		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 1, GL_INT, stride, (const void *) offsetof(VertexPalette, paletteId));

		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 1, GL_INT, stride, (const void *) offsetof(VertexPalette, hide));

	}
};

struct VertexTexture {
public:
    glm::vec3 position;
    glm::vec2 texCoord;
    // Each quad (sprite) needs to know which texture to use, so we add an integer texIndex to the vertex attributes:
    int texIndex;
	glm::vec4 color;
	void show(bool) {}
    static void setupVertices() {

        auto stride = sizeof(VertexTexture);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void *) offsetof(VertexTexture, texCoord));

        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(2, 1, GL_INT, stride, (const void *) offsetof(VertexTexture, texIndex));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (const void *) offsetof(VertexTexture, color));


    }



};

struct VertexSkeletal {
public:
    glm::vec3 position;
    glm::vec2 texture;
    // Each quad (sprite) needs to know which texture to use, so we add an integer texIndex to the vertex attributes:
    glm::vec3 weights;
};