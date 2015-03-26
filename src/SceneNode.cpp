#include "SceneNode.h"

/*
BoundingBox* getBoundingBox(SceneNode* mesh)
{
    BoundingBox *bbox = new BoundingBox;
    if(mesh  == 0) {
        std::cerr << "Unable to get bounding box of null mesh" << std::endl;
    }

    bbox->top = bbox->bottom = bbox->left = bbox->right = bbox->front = bbox->back = 0.f;
    for(int i=0; i<mesh->vertexData.size(); i++) {
        if      (mesh->vertexData[i].vertex.x < bbox->left)      { bbox->left     = mesh->vertexData[i].vertex.x; }
        else if (mesh->vertexData[i].vertex.x > bbox->right)     { bbox->right    = mesh->vertexData[i].vertex.x; }
        if      (mesh->vertexData[i].vertex.y < bbox->bottom)    { bbox->left     = mesh->vertexData[i].vertex.y; }
        else if (mesh->vertexData[i].vertex.y > bbox->top)       { bbox->right    = mesh->vertexData[i].vertex.y; }
        if      (mesh->vertexData[i].vertex.z < bbox->back)      { bbox->back     = mesh->vertexData[i].vertex.z; }
        else if (mesh->vertexData[i].vertex.z > bbox->front)     { bbox->front    = mesh->vertexData[i].vertex.z; }
    }
    return bbox;
}
*/
