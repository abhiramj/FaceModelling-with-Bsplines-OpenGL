Computer Graphics(CAP 5705) assignment 3 at University of Florida

A face is modelled using a NURBS surface. This way the model (my face) is represented using very few points, here 13*13.Eyes are also drawn as textured spheres. The face can be shown by pressing 'n'which loads the texture and then 'l'which sets the points.'c' shows the control points. 'f'is for front view. Arrow keys can be used to rotate the face to required angle. Space animates a smile.




Tasks Implemented:

Trim Eyes and place textured spheres
Animate a smile

Sunglass strap:
================

The sunglass strap was done using a Bezier patch of degree 2 in along width and degree 4 along length. The patch can be pulled back using a purple point drawn on screen. It snaps back automatically by linaerly interpolating betwwn the point the mouse was released and original point.

Use key 'p' to trigger the sunglass strap. 

Tasks Incomplete

Realistic Hair 
=================
Source: http://www.xbdev.net/directx3dx/specialX/Fur/index.php
Implemented in Direct3d. 


The idea is to use textures on different layers of surface with varying transparency values. Implemented the varying transparency but on spheres to test the effect. I have an model of the scalp but was not able to proceed further because of texture mapping.  Both can be seen by pressing 'h'.





