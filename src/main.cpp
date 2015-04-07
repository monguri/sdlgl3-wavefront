#include "Common.h"
#include "BinaryData.h"
#include "SceneNode.h"
#include "Renderer.h"
#include "Terrain.h"
#include "Texture.h"

#include "tiny_obj_loader.h"

#include "SDL.h"
#include "SDL_opengl.h"

void infoMsg(const char* msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info", msg, NULL);
}

void errorMsg(const char* title)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, SDL_GetError(), NULL);
}

class MyGLApp
{
public:
    SDL_Window* window;
    Renderer renderer;
    Camera* camera;
    SDL_GLContext glContext;
    SDL_Event event;

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;
    float horizontalAngle;
    float verticalAngle;
    float speed;
    float mouseSpeed;
    float deltaTime;
    int runLevel;
    double lastTime;


    MyGLApp()
    {
        position = glm::vec3(0.f, 1.f, 5.f);
        horizontalAngle = 3.14159f;
        verticalAngle = 0.f;
        speed = .001f;
        mouseSpeed = 0.001f;
        runLevel = 1;
        glm::vec3 direction;
        glm::vec3 position;
        lastTime = SDL_GetTicks();
        deltaTime = 0;
        window = 0;
        camera = 0;


        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
            runLevel = 0;
        }
        else
        {

            //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

            SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


            //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

            Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
            window = SDL_CreateWindow("", 300, 100, 1200, 800, flags);
            if (window == NULL)
            {
                fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
                errorMsg("Unable to create window");
                runLevel = 0;
                return;
            }
            else
            {
                glContext = SDL_GL_CreateContext(window);
                if (glContext == NULL)
                {
                    errorMsg("Unable to create OpenGL context");
                    errorMsg(SDL_GetError());
                    runLevel = 0;
                    return;
                }
                /* This makes our buffer swap syncronized with the monitor's vertical refresh */
                SDL_GL_SetSwapInterval(0);

                //GLEW
                glewExperimental = GL_TRUE;
                GLenum err = glewInit();
                checkForGLError();
                if (GLEW_OK != err)
                {
                    // Problem: glewInit failed, something is seriously wrong.
                    //fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
                    const char* errorStr = (char*)glewGetErrorString(err);
                    errorMsg(errorStr);

                }
                std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;


                std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

                int flags=IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF;
                int initted=IMG_Init(flags);
                if(initted&flags != flags)
                {
                    printf("IMG_Init: Failed to init required jpg and png support!\n");
                    printf("IMG_Init: %s\n", IMG_GetError());
                    // handle error
                }
            }


            SDL_SetWindowGrab(window,SDL_TRUE);
            //if(SDL_ShowCursor(SDL_DISABLE) < 0) {
            //    std::cerr << "Unable to hide the cursor" << std::endl;
            //}

            /*
            if(SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
                errorMsg(SDL_GetError());
            } */



            checkForGLError();

            // Get largest anisotropic filtering level
            GLfloat fLargest;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

            checkForGLError();
            // Enable depth test
            glEnable(GL_DEPTH_TEST);
            checkForGLError();
            // Accept fragment if it closer to the camera than the former one
            glDepthFunc(GL_LESS);
            // Cull triangles which normal is not towards the camera
            //glEnable(GL_CULL_FACE);

            checkForGLError();
            glClearColor(0.8, 0.8, 0.8, 1.0);
            checkForGLError();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            checkForGLError();


            checkForGLError();
            camera = new Camera();



           /* for(int i=0; i<2; i++) {
               renderer.addWavefront("human.obj", glm::translate(glm::mat4(1.f), glm::vec3(i * 0.4f, 0.0, 0.0)));
            }

            renderer.addWavefront("hires.obj", glm::translate(glm::mat4(1.f), glm::vec3(-5.0, -0.1, 0.0)));
            //renderer.addWavefront("hires.obj", glm::translate(glm::mat4(1.f), glm::vec3(-1.0, -0.6, 0.0)));

            renderer.addWavefront("nexuiz2.obj", glm::translate(glm::mat4(1.f), glm::vec3(-6.0, 0.0, 0.0)));*/
            renderer.addWavefront("portland.obj", glm::translate(glm::mat4(1.f), glm::vec3(0.0,0.0, 0.0)));

            renderer.buildScene();


            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            glViewport(0, 0, viewport[2], viewport[3]);

            SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
            SDL_WarpMouseInWindow(window, viewport[2]/2, viewport[3]/2);
            SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);

        }
    }

    ~MyGLApp()
    {
        delete camera;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void keyDown(SDL_Keycode& key)
    {
        switch(key)
        {
        case SDLK_ESCAPE:
            runLevel = 0;
            break;
        }
    }

    void keyUp(SDL_Keycode& key)
    {

    }

    void start()
    {
        while (runLevel > 0)
        {
            SDL_PollEvent(&event);

            if(event.type == SDL_QUIT)
            {
                runLevel = 0;
                break;
            }
            else if(event.type == SDL_KEYDOWN)
            {
                keyDown(event.key.keysym.sym);
                if(runLevel < 1) {
                    break;
                }
            }
            else if(event.type == SDL_KEYUP)
            {
                keyUp(event.key.keysym.sym);
            }

            const Uint8 *keys = SDL_GetKeyboardState(NULL);

            if ( keys[SDL_SCANCODE_W] )
            {
                position += direction * deltaTime * speed;
            }

            if(keys[SDL_SCANCODE_S])
            {
                position -= direction * deltaTime * speed;
            }

            if(keys[SDL_SCANCODE_D])
            {
                position += right * deltaTime * speed;
            }

            if(keys[SDL_SCANCODE_A])
            {
                position -= right * deltaTime * speed;
            }

            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);

            int width = viewport[2];
            int height = viewport[3];

            // Get mouse position
            double xpos, ypos;
            int x, y;
            SDL_GetMouseState(&x, &y);

            xpos = (double)x;
            ypos = (double)y;


            SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
            SDL_WarpMouseInWindow(window, width/2, height/2);
            SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);

            // Compute time difference between current and last frame
            double currentTime = SDL_GetTicks();
            deltaTime = float(currentTime - lastTime);


            // Compute new orientation
            horizontalAngle += mouseSpeed * deltaTime * float(width/2 - xpos );
            verticalAngle   += mouseSpeed * deltaTime * float(height/2 - ypos );



            direction = glm::vec3(
                            cos(verticalAngle) * sin(horizontalAngle),
                            sin(verticalAngle),
                            cos(verticalAngle) * cos(horizontalAngle)
                        );

            // Right vector
            right = glm::vec3(
                        sin(horizontalAngle - 3.14f/2.0f),
                        0,
                        cos(horizontalAngle - 3.14f/2.0f)
                    );

            // Up vector
            glm::vec3 up = glm::cross( right, direction );


            camera->modelViewMatrix = glm::lookAt(
                                          position,           // Camera is here
                                          position+direction, // and looks here : at the same position, plus "direction"
                                          up                  // Head is up (set to 0,-1,0 to look upside-down)
                                      );
            lastTime = currentTime;

            // Render frame
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            renderer.render(camera);



            SDL_GL_SwapWindow(window);
        }
    }

};

int main(int argc, char** argv)
{
    MyGLApp app;
    app.start();
    return 0;
}
