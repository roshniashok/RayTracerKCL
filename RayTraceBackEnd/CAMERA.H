//include guards
#ifndef _Camera_H
#define _Camera_H

#include "Vect.h"

class Camera {
    // camera needs 4 vectors
	Vect campos, camdir, camright, camdown; //camright and camdown define the coordinate system for our scene
                                            //and will be used our functions in vector compute our perspectives
                                            // ( to compute the direction of our shapes )

	public: //class needs to be public so all methods can be called from main

	Camera (); //can have instants with no parameters (just to prevent errors)

	Camera (Vect, Vect, Vect, Vect);

	// functions to return values of a camera's parameters
	Vect getCameraPosition () { return campos; }
	Vect getCameraDirection () { return camdir; }
	Vect getCameraRight () { return camright; }
	Vect getCameraDown () { return camdown; }

};

Camera::Camera () // parameters camera will have if they had not been provided
{
	campos = Vect(0,0,0);
	camdir = Vect(0,0,1);
	camright = Vect(0,0,0);
	camdown = Vect(0,0,0);
}

Camera::Camera (Vect pos, Vect dir, Vect right, Vect down) { // parameters ray will have if they are provided
	campos = pos;
	camdir = dir;
	camright = right;
	camdown = down;
}

#endif
