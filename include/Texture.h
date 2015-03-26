#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Common.h"

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

GLuint loadTGA(const char * imagepath);

#endif // _TEXTURE_H_
