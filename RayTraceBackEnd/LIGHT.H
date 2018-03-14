//include guards
#ifndef _Light_H
#define _Light_H

#include "Source.h"
#include "Vect.h"
#include "Color.h"

// subclass of the Source class
class Light : public Source {
    //Color needs 2 values
	Vect position;
	Color color;

	public: //class needs to be public so all methods can be called from main

	Light (); //can have instants with no parameters (just to prevent errors)

	Light (Vect, Color);

	// functions to return values of light coordinate and position
	virtual Vect getLightPosition () { return position; }
	virtual Color getLightColor () { return color; }

};

Light::Light () { // parameters light will have if they had not been provided
	position = Vect(0,0,0);
	color = Color(1,1,1,0);
}

Light::Light (Vect p, Color c) { // parameters light will have if they are provided
	position = p;
	color = c;
}

#endif
