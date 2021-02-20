# OpenGLPAG
The OpenGL exercises I have done for the "Computer Graphics Programming" course. The course was about how a 3D computer graphics works and how to use the OpenGL and shaders to create simple 3D scenes. The repository is a fork of the startup project that contain necessary libraries. Branches "ex2", "ex3" and "ex4" contain consecutive exercises.

The last (ex4) exercise contains almost everything that was implemented in the previous ones.
Summary of implemented features:
- Rendering meshes using VAOs, VBOs and EBOs
- Vertex and fragment shaders includes:
  - Texture drawing
  - Cubemap drawing
  - Drawing objects that refract and reflect a cubemap
  - Lighting calculations using Blinn-Phong model: directional (global) light, spotlight, point light
- Instanced rendering
- Scene graph
- Camera
- Importing models using the Assimp library
- Changing the scene in runtime with GUI controls using ImGui library

### Exercise 2 (branch "ex2") - Solar system
![Solar system](https://i.imgur.com/pO5aws9.png)
Solar system with textured planets and moons. Objects are stored in a scene graph.  
Rotatable view.

### Exercise 3 ("ex3") - Housing estate
![Housing estate](https://i.imgur.com/IlwQtov.png)
Grid of houses using instanced rendering.  
3 types of lighting: global, spotlight and point light (diffuse and specular).

### Exercise 4 ("ex4") - Motorbike
![Motorbike](https://i.imgur.com/E064lL4.png)  
![Motorbike](https://i.imgur.com/rjtcuhY.png)  
Video: https://i.imgur.com/c2CNsay.mp4  
Controllable motorbike with moving wheels and handlebar.  
Mirrors are reflecting the skybox. Windshield is refracting the skybox.  
The camera (view) can be "attached" behind the motorbike.  
