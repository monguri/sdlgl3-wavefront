#include "Common.h"
#include "Renderer.h"
#include "Texture.h"

void _checkForGLError(const char *file, int line)
{
    GLenum err (glGetError());
    int numErrors = 0;
    while(err!=GL_NO_ERROR)
    {
        std::string error;
        numErrors++;
        switch(err)
        {
        case GL_INVALID_OPERATION:
            error="INVALID_OPERATION";
            break;
        case GL_INVALID_ENUM:
            error="INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error="INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            error="OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error="INVALID_FRAMEBUFFER_OPERATION";
            break;
        }

        std::cerr << "GL_" << error.c_str() <<" - "<< file << ":" << line << std::endl;

        if(numErrors > 50)
        {
            exit(1);
        }

        err=glGetError();
    }
}

Renderer::Renderer()
{
    startPosition = 0;
    vao = vbo = ibo = 0;
    gpuProgram = 0;
    //vertexData = 0;
    //vertexDataSize = 0;
}

Renderer::~Renderer()
{
    if(sceneNodes.size() > 0)
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
    }
    //if(vertexData || vertexDataSize > 0)
    //{
    //    delete[] vertexData;
    //}


    //TODO:delete sceneNodes and gpuPrograms
}

void Renderer::addMaterial(Material* material)
{
    materials[material->name] = *material;
}

void Renderer::addSceneNode(SceneNode* sceneNode)
{
    if(!sceneNode)
    {
        std::cerr << "Unable to add null sceneNode" << std::endl;
    }
    else
    {
        sceneNodes.push_back(*sceneNode);
    }
}


// Used to check file extension
bool hasEnding (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

void Renderer::addTexture(const char* textureFileName, GLuint& textureId)
{
    std::string fileNameStr(TEXTURE_DIRECTORY);
    fileNameStr += DIRECTORY_SEPARATOR;
    fileNameStr += textureFileName;

    std::map<std::string,SDL_Surface*>::const_iterator it = textures.find(textureFileName);

    SDL_Surface* image = 0;
    if(it != textures.end())
    {
        //std::cout << textureFileName << " is already a loaded texture: " << textures[textureFileName]->w << std::endl;
        image = textures[textureFileName];
    }
    else
    {
        image = IMG_Load(fileNameStr.c_str());
        if(!image)
        {
            std::cerr << "Unable to load texture: " << textureFileName << std::endl;
            return;
        }
        textures[textureFileName] = image;

    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    int mode = GL_RGB;

    // This is still not working correctly (tga images still have wrong colors)
    std::string tga(".tga");
    if(hasEnding(fileNameStr, tga))
    {
        mode = GL_BGR;
        if(image->format->BytesPerPixel == 4)
        {
            mode = GL_BGRA;
        }
    }
    //todo: change to GL_BRG on .tga images


    if(image->format->BytesPerPixel == 4)
    {
        mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


void printVertex(Vertex& v)
{
    std::cerr << "v " << v.vertex[0] << ", " << v.vertex[1] << ", " << v.vertex[2]
              << '\t' << " n " << v.normal[0] << ", " << v.normal[1] << ", " << v.normal[2]
              << '\t' << " t " << v.textureCoordinate[0] << ", " << v.textureCoordinate[1] << std::endl;
}

void Renderer::addWavefront(const char* fileName, glm::mat4 matrix)
{

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string modelDirectory(MODEL_DIRECTORY);
    modelDirectory += DIRECTORY_SEPARATOR;
    std::string fileNameStr(modelDirectory);
    fileNameStr += fileName;
    std::string err = tinyobj::LoadObj(shapes, materials, fileNameStr.c_str(), modelDirectory.c_str());
    if(!err.empty())
    {
        std::cerr << err << std::endl;
        return;
    }


    //for(int i=0; i<shapes[0].mesh.normal.y)


    for(size_t i=0; i<materials.size(); i++)
    {
        Material m;
        memcpy((void*)& m.ambient, (void*)& materials[i].ambient[0], sizeof(float)*3);
        memcpy((void*)& m.diffuse, (void*)& materials[i].diffuse[0], sizeof(float)*3);
        memcpy((void*)& m.emission, (void*)& materials[i].emission[0], sizeof(float)*3);
        memcpy((void*)& m.specular, (void*)& materials[i].specular[0], sizeof(float)*3);
        memcpy((void*)& m.transmittance, (void*) &materials[i].transmittance[0], sizeof(float)*3);
        memcpy((void*)& m.illum, (void*)& materials[i].illum, sizeof(int));
        memcpy((void*)& m.ior, (void*)& materials[i].ior, sizeof(float));
        memcpy((void*)& m.shininess, (void*)& materials[i].shininess, sizeof(float));
        memcpy((void*)& m.dissolve, (void*)& materials[i].dissolve, sizeof(float));
        strcpy(m.name, materials[i].name.c_str());
        strcpy(m.ambientTexName, materials[i].ambient_texname.c_str());
        strcpy(m.diffuseTexName, materials[i].diffuse_texname.c_str());
        strcpy(m.normalTexName, materials[i].normal_texname.c_str());
        strcpy(m.specularTexName, materials[i].specular_texname.c_str());
        addMaterial(&m);
    }

    for (size_t i = 0; i < shapes.size(); i++)
    {
        std::vector<Vertex> mVertexData;

        unsigned int materialId, lastMaterialId = 0;
        if(shapes[i].mesh.material_ids.size() > 0)
        {
            materialId = lastMaterialId = shapes[i].mesh.material_ids[0];
        }

        for(int j=0; j <shapes[i].mesh.indices.size(); j++)
        {
            if((j%3) == 0)
            {

                //std::cout << "material " << materials[shapes[i].mesh.material_ids[j/3]].name << std::endl;
                lastMaterialId = materialId;
                materialId = shapes[i].mesh.material_ids[j/3];

                // std::cout << ""
                if(materialId != lastMaterialId )
                {
                    //new node
                    SceneNode sceneNode;
                    sceneNode.name = shapes[i].name.c_str();
                    sceneNode.material = materials[lastMaterialId].name.c_str();
                    sceneNode.vertexDataSize = mVertexData.size();
                    sceneNode.vertexData = new Vertex[sceneNode.vertexDataSize];
                    memcpy((void*) sceneNode.vertexData, (void*) mVertexData.data(), sizeof(Vertex) * sceneNode.vertexDataSize);
                    sceneNode.startPosition = startPosition;
                    startPosition += sceneNode.vertexDataSize;
                    sceneNode.endPosition = sceneNode.startPosition + sceneNode.vertexDataSize;
                    sceneNode.primativeMode = GL_TRIANGLES;
                    sceneNode.diffuseTextureId = 0;
                    sceneNode.modelViewMatrix = matrix;
                    addSceneNode(&sceneNode);
                    mVertexData.clear();
                }
            }

            Vertex v;
            memcpy((void*)& v.vertex, (void*)& shapes[i].mesh.positions[ shapes[i].mesh.indices[j] * 3 ], sizeof(float) * 3);

            if((shapes[i].mesh.indices[j] * 3) >= shapes[i].mesh.normals.size())
            {
                std::cerr << "Unable to put normal " << std::endl;
                return;
            }

            memcpy((void*)& v.normal, (void*)& shapes[i].mesh.normals[ (shapes[i].mesh.indices[j] * 3) ], sizeof(float) * 3);

            if((shapes[i].mesh.indices[j] * 2) >= shapes[i].mesh.texcoords.size())
            {
                std::cerr << "Unable to put texcoord in " << shapes[i].name << std::endl;
                return;
            }
            tinyobj::mesh_t* m = &shapes[i].mesh;
            v.textureCoordinate[0] = m->texcoords[(int)m->indices[j]*2];
            v.textureCoordinate[1] = 1 - m->texcoords[(int)m->indices[j]*2+1]; // Account for wavefront to opengl coordinate system conversion

            mVertexData.push_back(v);
            if(j == shapes[i].mesh.indices.size() - 1)
            {
                SceneNode sceneNode;
                sceneNode.name = shapes[i].name.c_str();
                sceneNode.material = materials[materialId].name.c_str();
                sceneNode.vertexDataSize = mVertexData.size();
                sceneNode.vertexData = new Vertex[sceneNode.vertexDataSize];
                memcpy((void*) sceneNode.vertexData, (void*) mVertexData.data(), sizeof(Vertex) * sceneNode.vertexDataSize);
                sceneNode.startPosition = startPosition;
                sceneNode.endPosition = sceneNode.startPosition + sceneNode.vertexDataSize;
                startPosition += sceneNode.vertexDataSize;
                sceneNode.primativeMode = GL_TRIANGLES;
                sceneNode.diffuseTextureId = 0;
                sceneNode.modelViewMatrix = matrix;
                addSceneNode(&sceneNode);
            }
        }
    }
}

void Renderer::buildScene()
{
    checkForGLError();
    if(sceneNodes.size() == 0)
    {
        std::cout << "building empty scene" << std::endl;
        return;
    }


    for(int i=0; i<sceneNodes.size(); i++)
    {
        for(int j=0; j<sceneNodes[i].vertexDataSize; j++)
        {
            Vertex v;
            memcpy((void*) &v, (void*) &sceneNodes[i].vertexData[j], sizeof(Vertex));
            vertexData.push_back(v);
            indices.push_back(indices.size());
        }
    }

    for(int i=0; i <sceneNodes.size(); i++)
    {
        if(materials.find(sceneNodes[i].material) == materials.end()  )
        {
            std::cerr << "Material " << sceneNodes[i].material << " was not loaded" << std::endl;
        }
        else
        {
            if(strlen(materials[sceneNodes[i].material.c_str()].diffuseTexName) > 0)
                addTexture(materials[sceneNodes[i].material.c_str()].diffuseTexName, sceneNodes[i].diffuseTextureId);
        }
    }

    //Calculate Bounding Sphere radius

    for(int i=0; i<sceneNodes.size(); i++)
    {
        float r = 0;
        for(int j=0; j<sceneNodes[i].vertexDataSize; j++)
        {

            int r2 = sqrt((sceneNodes[i].vertexData[j].vertex[0]*sceneNodes[i].vertexData[j].vertex[0])
                          +(sceneNodes[i].vertexData[j].vertex[1]*sceneNodes[i].vertexData[j].vertex[1])
                          +(sceneNodes[i].vertexData[j].vertex[2]*sceneNodes[i].vertexData[j].vertex[2]));
            if(r2 > r)
            {
                r = r2;
            }

        }
        if(r == 0)
        {
            std::cerr << "Warning, bounding sphere radius = 0 for " << sceneNodes[i].name << std::endl;
        }
        sceneNodes[i].boundingSphere = r;
    }



    //std::cout << "num scene nodes: " << sceneNodes.size() << std::endl;
    //std::cout << "num vertices: " << vertexData.size() << std::endl;
    //std::cout << "num indices: " << indices.size() << std::endl;


    checkForGLError();

    //Allocate and assign a Vertex Array Object to our handle
    glGenVertexArrays(1, &vao);
    checkForGLError();
    // Bind our Vertex Array Object as the current used object
    glBindVertexArray(vao);
    checkForGLError();

    //Triangle Vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);                       //send positions on pipe 0
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(sizeof(float)*3));       //send normals on pipe 1
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(sizeof(float)*6));     //send colors on pipe 2
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    checkForGLError();

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
    checkForGLError();
    gpuProgram = new GpuProgram();

    try
    {

        VertexShader vertShader("shaders/default.vert");
        FragmentShader fragShader("shaders/default.frag");

        gpuProgram->attachShader(vertShader);
        gpuProgram->attachShader(fragShader);

        glLinkProgram(gpuProgram->getId());

        // Check for link errors
        GLint result;
        GLuint programId = gpuProgram->getId();
        glGetProgramiv(programId, GL_LINK_STATUS, &result);
        if(result == GL_FALSE)
        {
            GLint length;
            char *log;
            // get the program info log
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
            log = (char*) malloc(length);
            glGetProgramInfoLog(programId, length, &result, log);
            std::cout << "Unable to link shader: " << log << std::endl;
            free(log);
        }
        checkForGLError();
    }

    catch (std::exception& e)
    {
        std::cerr << "Unable to load shader: " << e.what() << std::endl;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    checkForGLError();


}

void Renderer::render(Camera* camera)
{


    if(sceneNodes.size() == 0)
    {
        std::cout << "skipping render() on empty scene" << std::endl;
        return;
    }
    checkForGLError();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    checkForGLError();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


    for(int i=0; i<sceneNodes.size(); i++)
    {

        frustum.extractFrustum(camera->modelViewMatrix, camera->projectionMatrix);
        glm::vec4 position(0.f, 0.f, 0.f, 1.f);
        position = sceneNodes[i].modelViewMatrix * position;

        // Frustum culling test
        if(frustum.spherePartiallyInFrustum(position.x, position.y, position.z, sceneNodes[i].boundingSphere) > 0)
        {
            // create and upload modelviewprojection matrix
            modelViewProjectionMatrix= camera->projectionMatrix * (camera->modelViewMatrix * sceneNodes[i].modelViewMatrix) ;
            glm::vec3 lightPos = glm::vec3(10,35,0);
            checkForGLError();

            glActiveTexture(GL_TEXTURE0);
            checkForGLError();
            glBindTexture(GL_TEXTURE_2D,  sceneNodes[i].diffuseTextureId );
            checkForGLError();


            //TODO: move uniforms to buffer object
            GLuint programID = gpuProgram->getId();
            // Get a handle for our "MVP" uniform
            GLuint MatrixID = glGetUniformLocation(programID, "MVP");
            GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
            GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
            GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
            GLuint ambientLocation = glGetUniformLocation(programID, "MaterialAmbient");
            GLuint diffuseLocation = glGetUniformLocation(programID, "MaterialDiffuse");
            GLuint specularLocation = glGetUniformLocation(programID, "MaterialSpecular");

            checkForGLError();
            gpuProgram->use();
            checkForGLError();
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
            glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &camera->projectionMatrix[0][0]);
            glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &camera->modelViewMatrix[0][0]);
            checkForGLError();
            glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
            //std::cout << "setting diffuse color: " << materials[sceneNodes[i].material].diffuse[0] << ", " << materials[sceneNodes[i].material].diffuse[1] << ", " << materials[sceneNodes[i].material].diffuse[2] << std::endl;
            glUniform3fv(ambientLocation, 1, materials[sceneNodes[i].material].ambient);
            glUniform3fv(diffuseLocation, 1, materials[sceneNodes[i].material].diffuse);
            glUniform3fv    (specularLocation, 1, materials[sceneNodes[i].material].specular);

            glUniform1i(glGetUniformLocation(gpuProgram->getId(), "myTextureSampler"), 0);
            checkForGLError();







            glDrawRangeElementsBaseVertex(sceneNodes[i].primativeMode, sceneNodes[i].startPosition, sceneNodes[i].endPosition,
                                          (sceneNodes[i].endPosition - sceneNodes[i].startPosition)  , GL_UNSIGNED_INT, (void*)(0), sceneNodes[i].startPosition);
        }
        else
        {
            //std::cerr << "Clipping " << sceneNodes[i].name << std::endl;
        }



        checkForGLError();
    }


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);

    checkForGLError();
}
