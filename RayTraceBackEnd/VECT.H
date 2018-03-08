//include guards
#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {
	double x, y, z; //Vect needs 3 values

	public: //class needs to be public so all methods can be called from main

	Vect (); //can have instants with no parameters (just to prevent errors)

	Vect (double, double, double);

	// functions to return values of a coordinate
	double getVectX() { return x; }
	double getVectY() { return y; }
	double getVectZ() { return z; }

	double magnitude () { // need to find magnitude of vector to normalise it
		return sqrt((x*x) + (y*y) + (z*z)); // return scalar value
	}

	Vect normalize () {
		double magnitude = sqrt((x*x) + (y*y) + (z*z)); // magnitude is not a call able function outside the main method so recreate its return value
		return Vect (x/magnitude, y/magnitude, z/magnitude); // return normalised vector
	}

	Vect negative () { // need to be able to invert a vector
		return Vect (-x, -y, -z);
	}

	double dotProduct(Vect v) { // takes in a vector to do dot product with the instance vector to help draw the shape accordingly to which way its facing
		return x*v.getVectX() + y*v.getVectY() + z*v.getVectZ(); // return scalar value
	}

	Vect crossProduct(Vect v) { // takes in a vector to do cross product with the instance vector to help draw the shape's perpendicular surfaces
		return Vect (y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX());
	}

	Vect vectAdd (Vect v) { // takes in a vector (need to be able to add vectors)
		return Vect (x + v.getVectX(), y + v.getVectY(), z + v.getVectZ());
	}

	Vect vectMult (double scalar) { // takes in a scalar (need to be able to multiply vectors)
		return Vect (x*scalar, y*scalar, z*scalar);
	}
};

Vect::Vect () { // parameters vector will have if they had not been provided
	x = 0;
	y = 0;
	z = 0;
}

Vect::Vect (double i, double j, double k) { // parameters vector will have if they are provided
	x = i;
	y = j;
	z = k;
}

#endif
