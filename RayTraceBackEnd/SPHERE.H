//include guards
#ifndef _SPHERE_H
#define _SPHERE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Sphere : public Object { // subclass of the Object class (all classes that make a shape in the scene are subclasses of the Object class)
    //Sphere needs 2 values
	Vect center;
	double radius;
	Color color;

	public: //class needs to be public so all methods can be called from main

	Sphere (); //can have instants with no parameters (just to prevent errors)

	Sphere (Vect, double, Color);

	// functions to return values of sphere centre, radius and color
	Vect getSphereCenter () { return center; }
	double getSphereRadius () { return radius; }
	virtual Color getColor () { return color; }

	virtual Vect getNormalAt(Vect point) {
		// normal always points away from the sphere centre so we'll get the normal
		Vect normal_Vect = point.vectAdd(center.negative()).normalize();
		return normal_Vect;
	}

	virtual double findIntersection(Ray ray) {
	    // get ray origin and it's coordinates
		Vect ray_origin = ray.getRayOrigin();
		double ray_origin_x = ray_origin.getVectX();
		double ray_origin_y = ray_origin.getVectY();
		double ray_origin_z = ray_origin.getVectZ();

		// get ray direction and it's coordinates
		Vect ray_direction = ray.getRayDirection();
		double ray_direction_x = ray_direction.getVectX();
		double ray_direction_y = ray_direction.getVectY();
		double ray_direction_z = ray_direction.getVectZ();

		// make instance of a sphere to model sphere and ray intersection
		Vect sphere_center = center;
		double sphere_center_x = sphere_center.getVectX();
		double sphere_center_y = sphere_center.getVectY();
		double sphere_center_z = sphere_center.getVectZ();

		// helps find intersection point
		double a = 1; // normalized
		double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x) + (2*(ray_origin_y - sphere_center_y)*ray_direction_y) + (2*(ray_origin_z - sphere_center_z)*ray_direction_z);
		double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius*radius);

		double discriminant = b*b - 4*c;

		if (discriminant > 0) {
			// the ray intersects the sphere
			// the first root
			double root_1 = ((-1*b - sqrt(discriminant))/2) - 0.000001;

			if (root_1 > 0) {
				// the first root is the smallest positive root
				return root_1; // this root is the intersect point
			}
			else {
				// the second root is the smallest positive root
				double root_2 = ((sqrt(discriminant) - b)/2) - 0.000001;
				return root_2; // else this root is the intersect point
			}
		}
		else {
			// the ray missed the sphere
			return -1;
		}
	}

};

Sphere::Sphere () { // parameters sphere will have if they had not been provided
	center = Vect(0,0,0);
	radius = 1.0;
	color = Color(0.5,0.5,0.5, 0);
}

Sphere::Sphere (Vect centerValue, double radiusValue, Color colorValue) { // parameters sphere will have if they are provided
	center = centerValue;
	radius = radiusValue;
	color = colorValue;
}

#endif
