# sdlgl3-wavefront

sdlgl3-wavefront is a starting point template for a shader-based OpenGL 3.3 application which can load multiple wavefront/.obj files and textures. The renderer will load a list of objects into a single vertex buffer and glDrawRangeElementsBaseVertex used for rendering. Other versions of OpenGL could also be used as long as shaders, buffer objects and glDrawRangeElementsBaseVertex are available. Only OpenGL 3.3 shaders are used for this template so shaders/default.vert and shaders/default.frag will need to be modified to support other versions of GLSL accordingly.

Frustum extraction and testing algorithms from http://www.crownandcutlass.com/features/technicaldetails/frustum.html have been incorporated into Frustum.cpp. After 3D model/models are loaded each disconnected mesh is put into a SceneNode structure and a bounding radius is calculated. The Renderer.renderer() is passed a camera object so the frustum matrix can be extracted every frame and every scene node in a renderer instance will be tested against the camera's frustum. This is one part of the code where many further optimizations could be made depending on the scene. Only the most basic frustum culling technique of testing a sphere is used in this project. The following unused methods are included in the Frustum class:
 
bool pointInFrustum( float x, float y, float z );
bool sphereInFrustum( float x, float y, float z, float radius );
float sphereInFrustumDistance( float x, float y, float z, float radius );
int spherePartiallyInFrustum( float x, float y, float z, float radius );
bool cubeInFrustum( float x, float y, float z, float size );
int cubePartiallyInFrustum( float x, float y, float z, float size );
bool polygonInFrustum(int numpoints, Point* pointlist);
 
The models used in this demo were derived from the Google 3D warehouse models and ariel imagery/elevation data was downloaded from http://oregonexplorer.info/

Screenshots:
![alt tag](https://raw.githubusercontent.com/chrisliebert/sdlgl3-wavefront/master/sdlgl3-wavefront_screenshot.png)
![alt tag](https://raw.githubusercontent.com/chrisliebert/sdlgl3-wavefront/master/sdlgl3-wavefront_screenshot2.png)
