#ifndef _BINARY_DATA_H_
#define _BINARY_DATA_H_

#include "Common.h"
#include "Material.h"

#include "tiny_obj_loader.h"

typedef struct {
    char name[64];
    char material[64];
    size_t vertexDataSize;
    float modelViewMatrix[16];
    GLuint startPosition;
    GLuint endPosition;
    GLenum primativeMode;
} BinarySceneNodeFileHeader;

//void writeObjToBinaryFile(std::vector<tinyobj::shape_t>& shapes, std::vector<tinyobj::material_t>& materials);


//void writeMaterialToBinaryFile(Material&);
//void writeNodeToBinaryFile(std::vector<Vertex>& vertexData, const char*, const char*, const char*, glm::mat4&, GLuint, GLuint, GLenum);

#endif // _BINARY_DATA_H_
