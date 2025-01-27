#pragma once

class Shader;


class Light {

public:
    virtual void setup(Shader*) = 0;
};