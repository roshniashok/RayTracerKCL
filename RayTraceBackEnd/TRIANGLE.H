//include guards
#ifndef _Triangle_H
#define _Triangle_H

#include <iostream>

using namespace std;
#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Triangle : public Object { // subclass of the Object class (all classes that make a shape in the scene are subclasses of the Object class)
    // Triangle takes in 3 vectors (normal and distance will be calculated from A, B and C) and a color
    Vect A, B, C;
	Vect normal;
	double distance;
	Color color;

	public: //class needs to be public so all methods can be called from main

	Triangle (); //can have instants with no parameters (just to prevent errors)

	Triangle (Vect, Vect, Vect, Color);

	// functions to return values of triangle's normal, distance and color
	Vect getTriangleNormal ()
	{ // get normal by doing cross product of 2 sides
	    Vect CA (C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
	    Vect BA (B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
	    normal = CA.crossProduct(BA).normalize();
	    return normal;
    }
    virtual Vect getNormalAt(Vect point)
	{
	    normal = getTriangleNormal();
		return normal;
	}
	double getTriangleDistance ()
	{ // find distance by taking a normal of any point in the triangle
	    normal = getTriangleNormal();
	    distance = normal.dotProduct(A);
	    return distance;
    }
	virtual Color getColor () { return color; }

	virtual double findIntersection(Ray ray) { // take in ray
		Vect ray_direction = ray.getRayDirection(); //get ray
		Vect ray_origin = ray.getRayOrigin();

		normal = getTriangleNormal();
		distance = getTriangleDistance();

		double a = ray_direction.dotProduct(normal); // determine if ray hit the triangle by getting dot prduct with the ray and the normal

		if (a == 0) {
			// ray is parallel to the Triangle
			return -1;
		}
		else {
			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			double distance2plane = -1*b/a;

			// These are with the 3 conditions bellow to determine if ray intersects triangle's normal
			double Qx = ray_direction.vectMult(distance2plane).getVectX() + ray_origin.getVectX();
            double Qy = ray_direction.vectMult(distance2plane).getVectY() + ray_origin.getVectY();
            double Qz = ray_direction.vectMult(distance2plane).getVectZ() + ray_origin.getVectZ();

            Vect Q(Qx,Qy,Qz);

            // These are the 3 conditions to see if ray intersects normal ray that's supposed to bounce of the triangle's surface
            //[CAxQA]*n >= 0
            Vect CA (C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
            Vect QA (Q.getVectX() - A.getVectX(), Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
            double test1 = CA.crossProduct(QA).dotProduct(normal);
            //[BCxQC]*n >= 0
            Vect BC (B.getVectX() - C.getVectX(), B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
            Vect QC (Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
            double test2 = BC.crossProduct(QC).dotProduct(normal);
            //[ABxQB]*n >= 0
            Vect AB (A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
            Vect QB (Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
            double test3 = AB.crossProduct(QB).dotProduct(normal);

			if((test1 >= 0) && (test2 >= 0) && (test3 >= 0))
            {
                //ray hit inside triangle
                return -1*b/a;
            }
            else
            {
                //ray hit outside triangle
                return -1;
            }

		}
	}

};

Triangle::Triangle () { // parameters triangle will have if they had not been provided
	A = Vect(1,0,0);
	B = Vect(0,1,0);
	C = Vect(0,0,1);
	color = Color(0.5,0.5,0.5, 0);
}

Triangle::Triangle (Vect pointA, Vect pointB, Vect pointC, Color colorValue) { // parameters triangle will have if they are provided
	A = pointA;
	B = pointB;
	C = pointC;
	color = colorValue;
}

#endif
