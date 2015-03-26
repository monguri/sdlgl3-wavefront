#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Common.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/vec3.hpp"

class Camera
{
public:
    Camera();
    glm::mat4 modelViewMatrix;
    glm::mat4 projectionMatrix;

    void moveForward(float amount);
    void moveLeft(float amount);
    void moveRight(float amount);
};

#endif
