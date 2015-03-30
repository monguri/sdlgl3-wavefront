#include "BinaryData.h"

void writeObjToBinaryFile(std::vector<tinyobj::shape_t>& shapes, std::vector<tinyobj::material_t>& materials)
{
    //TODO
}

void writeMaterialToBinaryFile(Material& material)
{
	if(strlen(material.name) < 1) {
		std::cerr << "Unable to write material with no name" << std::endl;
		return;
	}
	std::string fileName(material.name);
	fileName += ".mat";
	std::ofstream outFile(fileName.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!outFile.good()) {
        std::cerr << "Unable to open " << fileName << " for writing" << std::endl;
        return;
    }
    outFile.write((char*)& material, sizeof(Material));
    outFile.close();
}

void writeNodeToBinaryFile(std::vector<Vertex>& _vertexData, const char* filename, const char* name, const char* material, glm::mat4& modelViewMatrix, GLuint _startPosition, GLuint _endPosition, GLenum _drawMode)
{
    size_t vertexDataSize = _vertexData.size();
    Vertex* vertexData = new Vertex[vertexDataSize];
    // Copy vertices
    for(int i=0; i<vertexDataSize; i++) {
        for(int j=0; j<3; j++) {
            vertexData[i].vertex[j] = _vertexData[i].vertex[j];
            vertexData[i].normal[j] = _vertexData[j].normal[j];
        }
        for(int k=0; k<2; k++) {
            vertexData[i].textureCoordinate[k] = _vertexData[i].textureCoordinate[k];
        }
    }

    BinarySceneNodeFileHeader header;
    strcpy(header.name, name);
    strcpy(header.material, material);
    header.vertexDataSize = vertexDataSize;
    header.startPosition = _startPosition;
    header.endPosition = _endPosition;
    header.primativeMode = _drawMode;
    float *f = glm::value_ptr(modelViewMatrix);

    memcpy((void*)& header.modelViewMatrix, (void*) f, sizeof(float)*16);

    std::ofstream outFile(filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if(!outFile.good()) {
        std::cerr << "Unable to open " << filename << " for writing" << std::endl;
        return;
    }

    outFile.write((char*) &header, sizeof(BinarySceneNodeFileHeader));
    outFile.write((char*) &vertexData, sizeof(Vertex) * vertexDataSize);
    outFile.close();
}
