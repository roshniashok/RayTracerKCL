//include guards
#ifndef _Plane_H
#define _Plane_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object { // subclass of the Object class (all classes that make a shape in the scene are subclasses of the Object class)
	//Plane needs 3 values
	Vect normal;
	double distance; // distance from centre of plane from centre of scene
	Color color;

	public:

	Plane ();

	Plane (Vect, double, Color);

	// functions to return plane normal, color value, Normal vector of the plane  and plane distance from scene
	Vect getPlaneNormal () { return normal; }
	double getPlaneDistance () { return distance; }
	virtual Color getColor () { return color; }
	virtual Vect getNormalAt(Vect point) {
		return normal;
	}

	// function that returns ray intersection
	virtual double findIntersection(Ray ray) { // ray reflects off a plane and is a normal to the plane as the plane is a flat surface so
                                               // it's easier to find ray-plane intersection then ray-sphere intersection
		Vect ray_direction = ray.getRayDirection();

		double a = ray_direction.dotProduct(normal); // determines if direction of ray is normal to the plane so they will intersect

		if (a == 0) { // If ray does not intersect plane
			// ray is parallel to the plane
			return -1;
		}
		else { //else ray does intersect plane
			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			return -1*b/a; // returns distance from point of intersection from ray origin
		}
	}

};

Plane::Plane () {// parameters plane will have if they had not been provided
	normal = Vect(1,0,0); //undefined normal vector with be in x direction
	distance = 0;
	color = Color(0.5,0.5,0.5, 0);
}

Plane::Plane (Vect normalValue, double distanceValue, Color colorValue) { // parameters plane will have if they are provided
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

#endif
