//#include "directionallight.h"
//#include "shader.h"
//#include <glm/gtc/type_ptr.hpp>
//
//DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float ambient) : Light(), _direction(glm::normalize(direction)),
//    _color(color), _ambient(ambient) {
//
//}
//
//void DirectionalLight::setup(Shader* s) {
//    int lightDirLoc = glGetUniformLocation(s->getProgId(), "uLightDir");
//    int lightColorLoc = glGetUniformLocation(s->getProgId(), "uLightColor");
//    int ambientLoc = glGetUniformLocation(s->getProgId(), "uAmbient");
//    glUniform3fv(lightDirLoc, 1, &_direction[0]);
//    glUniform3fv(lightColorLoc, 1, &_color[0]);
//    glUniform1f(ambientLoc, _ambient);
//}