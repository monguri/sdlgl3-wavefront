#include "Terrain.h"
#include "Texture.h"
/*
Terrain::Terrain()
{

}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &vbo);
    delete gpuProgram;
}

void Terrain::init()
{
    textureId = loadTGA("assets/wellington-dem.tga");

    for(float i = -100.f; i<= 100.f; i+= 0.20f)
    {
        for(float j = -100.f; j<= 100.f; j+= 0.20f)
        {
            VertexNormalUv v;
            v.vertex.x = i;
            v.vertex.y = 0;
            v.vertex.z = j;
            v.normal.x = i;
            v.normal.y = 1;
            v.normal.z = j;
            v.uv.x = 0.3;
            v.uv.y = 0.5;
            triangleVertexData.push_back(v);
            v.vertex.x = i + 1;
            v.vertex.y = 0;
            v.vertex.z = j;
            v.normal.x = i;
            v.normal.y = 1;
            v.normal.z = j;
            v.uv.x = 0.3;
            v.uv.y = 0.5;
            triangleVertexData.push_back(v);
            v.vertex.x = i + 1;
            v.vertex.y = 0;
            v.vertex.z = j + 1;
            v.normal.x = i;
            v.normal.y = 1;
            v.normal.z = j;
            v.uv.x = 0.3;
            v.uv.y = 0.5;
            triangleVertexData.push_back(v);
            v.vertex.x = i;
            v.vertex.y = 0;
            v.vertex.z = j + 1;
            v.normal.x = i;
            v.normal.y = 1;
            v.normal.z = j;
            v.uv.x = 0.3;
            v.uv.y = 0.5;
            triangleVertexData.push_back(v);
        }
    }

        std::cout << "Drawing " << triangleVertexData.size() << " verts" << std::endl;



    //Triangle Vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormalUv) * triangleVertexData.size(), &triangleVertexData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexNormalUv),(void*)0);                       //send positions on pipe 0
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexNormalUv),(void*)(sizeof(float)*3));       //send normals on pipe 1
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(VertexNormalUv),(void*)(sizeof(float)*6));     //send colors on pipe 2
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    gpuProgram = new GpuProgram();

    try
    {

        VertexShader vertShader("assets/terrain.vert");
        FragmentShader fragShader("assets/terrain.frag");

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
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
            log = (char*) malloc(length);
            glGetProgramInfoLog(programId, length, &result, log);
            std::cout << "Unable to link shader: " << log << std::endl;
            free(log);
        }

    }
    catch (std::exception e)
    {
        std::cerr << "Unable to load shader: " << e.what() << std::endl;
    }

    GLuint textureIdUniformLoc  = glGetUniformLocation(gpuProgram->getId(), "myTextureSampler");


}

void Terrain::render(Camera* camera)
{

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // create and upload modelviewprojection matrix
    glm::mat4 trans1;
    GLuint programID = gpuProgram->getId();
    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");



    //float i, j = 0.f;
    gpuProgram->use();
    checkForGLError();




    trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    modelViewProjectionMatrix= camera->projectionMatrix * (camera->modelViewMatrix * trans1);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &camera->projectionMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &camera->modelViewMatrix[0][0]);

    glm::vec3 lightPos = glm::vec3(0,3,0);
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
    //Texturing
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(glGetUniformLocation(gpuProgram->getId(), "myTextureSampler"), 0);



    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //glDrawArrays(GL_TRIANGLES, 0, triangleVertexData.size());

    //glDrawArrays(GL_TRIANGLE_STRIP, 0, triangleVertexData.size());
//
    glDrawArrays(GL_LINES, 0, triangleVertexData.size());
    glDrawArrays(GL_LINES_ADJACENCY, 0, triangleVertexData.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    checkForGLError();



    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);


}

*/
