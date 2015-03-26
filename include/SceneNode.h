#ifndef _SCENE_NODE_H_
#define _SCENE_NODE_H_

#include "Common.h"
#include "Material.h"
#include "GpuProgram.h"

/*
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/vec3.hpp"
*/
/*
template<typename T>
struct Mesh {
	std::vector<T> vertices;
    glm::mat4 modelViewProjectionMatrix;
    Material material;
};

typedef struct Mesh<VertexNormalUv> TexturedMesh;
*/

typedef struct {
    float top, bottom, left, right, front, back;
} BoundingBox;

typedef struct {
    std::string name;
    std::string material;
	Vertex* vertexData;
	size_t vertexDataSize;
    glm::mat4 modelViewMatrix;
    GLuint startPosition;
    GLuint endPosition;
    GLenum primativeMode;

    GLuint ambientTextureId;
    GLuint diffuseTextureId;
    GLuint normalTextureId;
    GLuint specularTextureId;

} SceneNode;

//BoundingBox* getBoundingBox(SceneNode*);

#endif
