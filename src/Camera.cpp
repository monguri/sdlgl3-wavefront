#include "Camera.h"

Camera::Camera()
{
    // get viewport
    GLint mViewport[4];
    glGetIntegerv( GL_VIEWPORT, mViewport );

    // Set up Projection and ModelView matrices
    projectionMatrix= glm::mat4(1.0f);
    projectionMatrix *= glm::perspective(45.0f, (float)mViewport[2] /(float) mViewport[3], 0.1f, 10000.0f);

    // setup modelview matrix (look down the negative z-axis)
    modelViewMatrix= glm::mat4(1.0f);
    modelViewMatrix*= glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::moveForward(float amount)
{
    //std::cout << "moving forward by " << amount << std::endl;
    //std::cout << modelViewMatrix[3][0] << " " << modelViewMatrix[3][1] << " " << modelViewMatrix[3][2] << std::endl;

    modelViewMatrix[3][2] += amount;
}

void Camera::moveLeft(float amount)
{
    //std::cout << "moving forward by " << amount << std::endl;
    //std::cout << modelViewMatrix[3][0] << " " << modelViewMatrix[3][1] << " " << modelViewMatrix[3][2] << std::endl;

    modelViewMatrix[3][0] += amount;
}

void Camera::moveRight(float amount)
{
    //std::cout << "moving forward by " << amount << std::endl;
    //std::cout << modelViewMatrix[3][0] << " " << modelViewMatrix[3][1] << " " << modelViewMatrix[3][2] << std::endl;

    modelViewMatrix[3][0] -= amount;
}
