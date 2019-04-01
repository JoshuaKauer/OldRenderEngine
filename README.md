# OldRenderEngine
This was a Render Engine made during school and was used for school projects. It is made with OpenGL and is capable of raycasting, transfering a .obj file to a binary file for faster reading, and A-Star Pathing.

### Binary Alpha
Alpha transparency, self explanatory
![Cube with alpha texture](Images/Alpha.PNG)

### Explosion
Using a geometry shader and normals (or calculated normals in my example) I was able to translate the triangles for a model across the normal, creating an explosion like effect.
![Almost formed clone trooper](Images/Explosion.PNG)
![Exploded clone trooper](Images/Explosion2.PNG)

### Water
Using a a plane with multiple triangles and sine/cosine waves, I create a plane that is very rudimentary water.
![Example with exagerated waves](Images/Water.PNG)

### CubeMap
Implemented skyboxes
![Skybox](Images/CubeMap2.PNG)
![Cubemap on a model](Images/CubeMap1.PNG)

### Shadows
Created a depth map to make shadows that can even cast onto the same model.
![Shadow being projected from 1 model to another](Images/ShadowMapping.PNG)
![Shadow being projected onto same model](Images/ShadowMapping2.PNG)
![Same as above just different area](Images/ShadowMapping3.PNG)

### Dynamic Environment Refraction
Applied cubemaps knowledge and framebuffers to make a cloaking effect
![Model refracting surrounding models and skybox](Images/DynamicEnvironmentRefraction.PNG)
![Different angle](Images/DynamicEnvironmentRefraction.PNG)

### Normal Mapping
Applied Normal Mapping to give better detail
![Normal mapping applied with correct texture](Images/NormalMapping.PNG)
![Normal mapping with incorrect texture](Images/NormalMapping2.png)

### Parallax Mapping
Turn a plane to look 3-D, by giving it depth via just an image file
![Parallax Mapping on a simple quad](Images/ParallaxMapping.PNG)
![Showing that it is a flat plane](Images/ParallaxMapping2.PNG)
![Different angle of Parallax Mapping](Images/ParallaxMapping3.PNG)

### Deferred Shading
Used the Deferred Shading lighting technique, I had to find and use new models that had the specular data(texture/map) I needed
![Top front view of Deferred Shading](Images/DeferredShading.png)
![Top back view](Images/DeferredShading2.png)
![Bottom back view](Images/DeferredShading3.png)
![Bottom side view](Images/DeferredShading4.png)

### Decals
Failed attempt to add sprays/decals to the engine. I never got it working in this engine.
