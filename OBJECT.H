//include guards
#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

// Object class is parent class to sphere and triangle class

class Object {
	public: //class needs to be public so all methods can be called from main

	Object (); //has instants with no parameters (does not require values to be provided to it)

	// functions to return color value and Normal vector
	virtual Color getColor () { return Color (0.0, 0.0, 0.0, 0); } // returns color of objects in the scene
	virtual Vect getNormalAt(Vect intersection_position) {
		return Vect (0, 0, 0);
	}

    // function that returns ray intersection
	virtual double findIntersection(Ray ray) {
		return 0; // for no objects intersection is 0
	}

};

Object::Object () {} //Object does not need to set parameters as it has none

#endif
