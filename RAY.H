//include guards
#ifndef _RAY_H
#define _RAY_H

#include "Vect.h"

class Ray {
	Vect origin, direction; //Ray needs 2 values origin and direction as vectors

	public: //class needs to be public so all methods can be called from main

	Ray (); //can have instants with no parameters (just to prevent errors)

	Ray (Vect, Vect);

	// function to return origin
	Vect getRayOrigin () { return origin; }
	// function to return direction
	Vect getRayDirection () { return direction; }

};

Ray::Ray () { // parameters ray will have if they had not been provided
	origin = Vect(0,0,0);
	direction = Vect(1,0,0);
}

Ray::Ray (Vect o, Vect d) { // parameters ray will have if they are provided
	origin = o;
	direction = d;
}

#endif
