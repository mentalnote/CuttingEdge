# CuttingEdge
A simple OpenGL application which will implement a few algorithms for slicing meshes and handle visualisation of such.
NOTE: As the project is using SDL2-2.0.3 for Visual C++ it requires VS2015 to compile. Earlier versions of visual studio have differently defined symbols required by this version of SDL.

TODO:
	- Fix poor design and hacks
	- Add normals texture coords and texturing to meshes
	- Modularise shader usage
	- Implement basic lighting model (maybe)
	- Implement mesh slicing algorithms (GPGPU and direct)