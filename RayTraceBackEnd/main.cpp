#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <cctype>
#include <locale>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <Lmcons.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Source.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"

//Tutorial Source: https://www.youtube.com/watch?v=k_aRiYSXcyo

using namespace std;

struct RGBType { //enables red green and blue array
	double r;
	double g;
	double b;
};

void savepng (const char *filename, int w, int h, int dpi, RGBType *data) {
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;

	unsigned char pngfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0}; //spaces between them just to help know how many I done
	unsigned char pnginfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	//saves file size (how many kilo bites)
	pngfileheader[ 2] = (unsigned char)(filesize);
	pngfileheader[ 3] = (unsigned char)(filesize>>8);
	pngfileheader[ 4] = (unsigned char)(filesize>>16);
	pngfileheader[ 5] = (unsigned char)(filesize>>24);

	//saves file width
	pnginfoheader[ 4] = (unsigned char)(w);
	pnginfoheader[ 5] = (unsigned char)(w>>8);
	pnginfoheader[ 6] = (unsigned char)(w>>16);
	pnginfoheader[ 7] = (unsigned char)(w>>24);

	//saves file height
	pnginfoheader[ 8] = (unsigned char)(h);
	pnginfoheader[ 9] = (unsigned char)(h>>8);
	pnginfoheader[10] = (unsigned char)(h>>16);
	pnginfoheader[11] = (unsigned char)(h>>24);

	//saves file area
	pnginfoheader[21] = (unsigned char)(s);
	pnginfoheader[22] = (unsigned char)(s>>8);
	pnginfoheader[23] = (unsigned char)(s>>16);
	pnginfoheader[24] = (unsigned char)(s>>24);

	//saves file as ppm image
	pnginfoheader[25] = (unsigned char)(ppm);
	pnginfoheader[26] = (unsigned char)(ppm>>8);
	pnginfoheader[27] = (unsigned char)(ppm>>16);
	pnginfoheader[28] = (unsigned char)(ppm>>24);

	//saves file ppm image
	pnginfoheader[29] = (unsigned char)(ppm);
	pnginfoheader[30] = (unsigned char)(ppm>>8);
	pnginfoheader[31] = (unsigned char)(ppm>>16);
	pnginfoheader[32] = (unsigned char)(ppm>>24);

	f = fopen(filename,"wb"); //open ppm file and make it writeable

	//converts ppm file type to png
	fwrite(pngfileheader,1,14,f);
	fwrite(pnginfoheader,1,40,f);

	for (int i = 0; i < k; i++) { //we can save colour to the file
		RGBType rgb = data[i];

		double red = (data[i].r)*255;
		double green = (data[i].g)*255;
		double blue = (data[i].b)*255;

		unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

		fwrite(color,1,3,f);
	}

	fclose(f);
}

int winningObjectIndex(vector<double> object_intersections) {
	// return the index of the winning intersection
	int index_of_minimum_value;

	// prevent unnecessary calculations as we don't care about rays that do not intersect object
	if (object_intersections.size() == 0) { // if there are no intersections
		return -1;
	}
	else if (object_intersections.size() == 1) { // else if there is 1 intersection
		if (object_intersections.at(0) > 0) {
			// if that intersection is greater than zero then its our index of minimum value
			return 0; // put closest intersection in the first value of the array
		}
		else {
			// else the only intersection value is negative
			return -1; // the ray has missed every pixel in the array
		}
	}
	else { // multiple intersections ( find closest intersection to camera )
		// else there is more than one intersection
		// first find the maximum value

		double max = 0;
		for (int i = 0; i < object_intersections.size(); i++) {
			if (max < object_intersections.at(i)) {
				max = object_intersections.at(i);
			}
		}

		// then starting from the maximum value find the minimum positive value
		if (max > 0) {
			// we only want positive intersections
			for (int index = 0; index < object_intersections.size(); index++) {
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}

			return index_of_minimum_value;
		}
		else {
			// all the intersections were negative
			return -1;
		}
	}
}

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientlight) {
// this method will help us determine color of a pixel in terms of shadows
	Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

	if (winning_object_color.getColorSpecial() == 2) { // this if statement is for the tiled floor
                                                       // we're using the special value but here we are using it to specify how many distinct colors the object
                                                       // has ( distinct as in two completely different colors)

		int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());

		if ((square % 2) == 0) {
			// black tile
			winning_object_color.setColorRed(0);
			winning_object_color.setColorGreen(0);
			winning_object_color.setColorBlue(0);
		}
		else {
			// white tile
			winning_object_color.setColorRed(1);
			winning_object_color.setColorGreen(1);
			winning_object_color.setColorBlue(1);
		}
	}

	Color final_color = winning_object_color.colorScalar(ambientlight);

	// this if statement uses special value but for its correct purpose of reflection so the color of each pixel is determined by what color the pixels are
	// of the other scenes in the object (which is notes by the ray bouncing form the reflective object to the other objects, back to the light source and
    // camera)
	if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
		// reflection from objects with specular intensity
		double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative()); // determine ray direction, reverse it to reflect off object
		Vect scalar1 = winning_object_normal.vectMult(dot1);
		Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
		Vect scalar2 = add1.vectMult(2);
		Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
		Vect reflection_direction = add2.normalize();

		// create ray for reflection
		Ray reflection_ray (intersection_position, reflection_direction);

		// determine what the ray intersects with first
		vector<double> reflection_intersections; // array of reflection intersections

		for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) {
			reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray)); // push the reflection intersections
                                                                                                                      // into the scene of the object
		}

		int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections); // reflection intersection will be the objects closest to
                                                                                                    // it

		if (index_of_winning_object_with_reflection != -1) {
			// reflection ray missed everything else
			if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) { // do not want intersections less than our accuracy value
				// determine the position and direction at the point of intersection with the reflection ray
				// the ray only affects the color if it reflected off something

                // we have to make recursive calls to do our reflection as with each reflection a sub-ray is formed, so our ray tracing slows by a 3 seconds
				Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_object_with_reflection)));
				Vect reflection_intersection_ray_direction = reflection_direction;

				// modify color of each pixel accordingly to what it's reflecting and its own color
				Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);
				final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));
			}
		}
	}

	for (int light_index = 0; light_index < light_sources.size(); light_index++) {
        // look through light sources to figure where shadows should be on object by getting color form the normal to the camera at the point of intersection
		Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();

		float cosine_angle = winning_object_normal.dotProduct(light_direction); // need cosine_angle to determine if there is a shadow
                                                                                // if dot product between light direction and ray normal to object pixel is
                                                                                // positive then it might have shadow if negative then there is no shadow

		if (cosine_angle > 0) {
			// test for shadows
			bool shadowed = false;

			// distance from light source to intersection
			Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
			float distance_to_light_magnitude = distance_to_light.magnitude();

			// new ray from intersection point to light source and if it intersects with anything before getting to the light source, then point is shadowed
			Ray shadow_ray (intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());

			vector<double> secondary_intersections;

			for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
				secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
			}

			// loop through array of secondary intersections
			/*for (int c = 0; c < secondary_intersections.size(); c++) {
				if (secondary_intersections.at(c) > accuracy) { // if second intersection is better than accuracy then we get that color
					if (secondary_intersections.at(c) <= distance_to_light_magnitude) { // closest pixel is shaded
						shadowed = true;
					}
				}
				break;
			}*/

			if (shadowed == false) {
                // linear algebra behind the shading
				final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

				if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) { // draws out the reflections by changing
                                                                                                                 // color value in terms of specular value
					// special [0-1]
					double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
					Vect scalar1 = winning_object_normal.vectMult(dot1);
					Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
					Vect scalar2 = add1.vectMult(2);
					Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
					Vect reflection_direction = add2.normalize();

					double specular = reflection_direction.dotProduct(light_direction);
					if (specular > 0) {
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
					}
				}

			}

		}
	}

	return final_color.clip();
}

vector<Object*> scene_objects; // array of objects in the scene

void makeCube (Vect corner1, Vect corner2, Color color) // uses two points to create the other points and make the cube, uses color to colour in the cube
                                                        // the two vectors are opposite points to each other
{
    // point1's coordinates
    double c1x = corner1.getVectX();
    double c1y = corner1.getVectY();
    double c1z = corner1.getVectZ();
    // point2's coordinates
    double c2x = corner2.getVectX();
    double c2y = corner2.getVectY();
    double c2z = corner2.getVectZ();

    // the other 6 points of the cube
    Vect A (c2x, c1y, c1z);
    Vect B (c2x, c1y, c2z);
    Vect C (c1x, c1y, c2z);
    Vect D (c2x, c2y, c1z);
    Vect E (c1x, c2y, c1z);
    Vect F (c1x, c2y, c2z);

    // drawing the lines and filling in the shape using a 2 triangles to fill in 1 side
    ///Front Side of cube
    scene_objects.push_back(new Triangle(D, A, corner1, color));
    scene_objects.push_back(new Triangle(corner1, E, D, color));
    ///Right Side of cube
    scene_objects.push_back(new Triangle(corner2, B, A, color));
    scene_objects.push_back(new Triangle(A, D, corner2, color));
    ///Back Side of cube      ### this side will not be seen ###
    //scene_objects.push_back(new Triangle(F, C, B, color));
    //scene_objects.push_back(new Triangle(B, corner2, F, color));
    ///Left Side of cube      ### this side will not be seen ###
    //scene_objects.push_back(new Triangle(E, corner1, C, color));
    //scene_objects.push_back(new Triangle(C, F, E, color));
    ///Top Side of cube
    scene_objects.push_back(new Triangle(D, E, F, color));
    scene_objects.push_back(new Triangle(F, corner2, D, color));
    ///Top Side of cube       ### this side will not be seen ###
    //scene_objects.push_back(new Triangle(corner1, A, B, color));
    //scene_objects.push_back(new Triangle(B, C, corner1, color));

}

void makePyramid (Vect corner1, Vect corner2, Color color) // uses two points to create the other points and make the cube, uses color to colour in the cube
                                                           // the two vectors are opposite points to each other
{
    // point1's coordinates
    double c1x = corner1.getVectX();
    double c1y = corner1.getVectY();
    double c1z = corner1.getVectZ();
    // point2's coordinates
    double c2x = corner2.getVectX();
    double c2y = corner2.getVectY();
    double c2z = corner2.getVectZ();

    // the other 2 points of the pyramid that is viewable
    double x = 0;
    double y = 0;
    double z = 0;

    double x2 = 0;
    double y2 = 0;
    double z2 = 0;

    // generating other points
    x = fabs(c1x - c2x);
    if (c1y >= c2y)
    {
        if(c1x >= c2x)
        {
            x = c1x + x;
            x2 = c1x;
        }
        else
        {
            x = c2x + x;
            x2 = c2x;
        }
        y = c2y;
        z = c2z;

        z2 = z + z;
        y2 = fabs(c1y - c2y)/2;
    }
    else
    {
        if(c1x >= c2x)
        {
            x = c1x + x;
            x2 = c1x;
        }
        else
        {
            x = c2x + x;
            x2 = c2x;
        }
        y = c1y;
        z = c1z;

        z2 = z - z;
        y2 = fabs(c1y - c2y)/2;
    }

    Vect A (x, y, z);
    Vect B (x2, y2, z2);

    // drawing the lines and filling in the shape
    ///Front Side of pyramid
    scene_objects.push_back(new Triangle(corner1, corner2, A, color));
    ///Other Side of pyramid
    scene_objects.push_back(new Triangle(A, corner2, B, color));

    double red = color.getColorRed() - 0.3;
    double green = color.getColorGreen() - 0.3;
    double blue = color.getColorBlue() - 0.3;

    Color darker (red,green,blue,0);
    ///Bottom Side of pyramid
    scene_objects.push_back(new Triangle(A, corner1, B, darker));
}
double getShapeSize(string scale,double shapeSize)
{
    ///determines size based string scale
    if(scale == "Scale to 25%")
    {
        shapeSize = shapeSize + 0.25;
    }
    if(scale == "Scale to 50%")
    {
        shapeSize = shapeSize + 0.5;
    }
    else if (scale == "Scale to 75%")
    {
        shapeSize = shapeSize + 0.75;
    }
    else
    {
        shapeSize = shapeSize + 1.0;
    }
    return shapeSize;
}

Color ifTranslucentOrReflective(string translucent, string reflective, Color color)
{
    /// determines if object is translucent or reflective based on their string values
    if(translucent == "Yes")
    {
        color.setColorRed(0.0);
        color.setColorGreen(0.0);
        color.setColorBlue(0.0);
        color.setColorSpecial(1.0);
    }
    else if(reflective == "Yes")
    {
        color.setColorSpecial(0.4);
    }
    return color;
}

Color getColour(string color)
{
    /// determines colour of shape
    if(color == "pink")
    {
        Color colour (1.0,0.6,0.9,0.0);
        return colour;
    }
    else if(color=="black")
  {
    Color Black (0.0,0.0,0.0,0.0);
    return Black;
  }
  else if (color=="blue"){
    Color Blue (0.0,0.0,0.5,0.0);
    return Blue;
  }
  else if (color=="cyan"){
    Color Cyan (0.0,0.9,0.9,0.0);
    return Cyan;
  }
  else if (color=="darkblue"){
    Color darkBlue (0.1,0,0.4,0.0);
    return darkBlue;
  }
  else if (color=="darkcyan"){
    Color darkCyan (0.1,0.6,0.8,0.0);
    return darkCyan;
  }
  else if (color=="darkgray"){
    Color darkGray (0.5,0.5,0.5,0.0);
    return darkGray;
  }
  else if (color=="darkgreen"){
    Color darkGreen (0.2,0.3,0.1,0.0);
    return darkGreen;
  }
  else if (color=="darkred"){
    Color darkRed (0.5,0.0,0.0,0.0);
    return darkRed;
  }
  else if (color=="darkmagenta"){
    Color darkMagenta (0.6,0.0,0.1,0.0);
    return darkMagenta;
  }
   else if (color=="darkred"){
    Color darkRed (0.7,0.0,0.0,0.0);
    return darkRed;
  }
   else if (color=="darkyellow"){
    Color darkYellow (1.0,0.9,0.1,0.0);
    return darkYellow;
  }
  else if (color=="gray"){
    Color gray (0.5,0.5,0.5,0.0);
    return gray;
  }
  else if (color=="green"){
    Color green (0.2,0.9,0.0,0.0);
    return green;
  }
  else if (color=="lightgray"){
    Color lightGray (0.7,0.7,0.7,0.0);
    return lightGray;
  }
  else if (color=="magenta"){
    Color magenta (1.0,0.0,1.0,0.0);
    return magenta;
  }
  else if (color=="red"){
    Color red (1.0,0.0,0.0,0.0);
    return red;
  }
  else if (color=="white"){
    Color white (1.0,1.0,1.0,0.0);
    return white;
  }
  else if (color=="yellow"){
    Color yellow (1.0,1.0,0.1,0.0);
    return yellow;
  }

  //second section
     else if (color=="fuchsia"){

    Color fuchsia (07,0.0,0.7,0.0);
    return fuchsia;
}


   else if (color=="forestgreen"){

    Color forestgreen (0.3,0.4,0.1,0.0);
    return forestgreen;

}
 else if (color=="ghostwhite"){

    Color ghostwhite (0.9,0.9,0.9,0.0);
    return ghostwhite;

}
 else if (color=="gainsboro"){

    Color gainsboro(0.7,0.7,0.7,0.0);
    return gainsboro;

}

 else if (color=="gold"){

    Color gold (0.9,0.9,0.3,0.0);
    return gold;

}

 else if (color=="goldenrod"){

    Color goldenrod (0.9,0.7,0.0,0.0);
    return goldenrod;

}
else if (color=="gray"){

    Color gray (0.5,0.5,0.5,0.0);
    return gray;

}
else if (color=="green"){

    Color green (0.2,0.9,0.0,0.0);
    return green;

}
else if (color=="greenyellow"){

    Color greenyellow (0.7,1.0,0.0,0.0);
    return greenyellow;

}

else if (color=="grey"){

    Color grey (0.7,0.7,0.6,0.0);
    return grey;
}

else if (color=="honeydew"){

    Color honeydew (1.0,0.8,0.2,0.0);
    return honeydew;
}
else if (color=="hotpink"){

    Color hotpink (1.0,0.4,0.8,0.0);
    return hotpink;
}
else if (color=="indianred"){

    Color indianred (0.6,0.0,0.1,0.0);
    return indianred;
}
else if (color=="indigo"){

    Color indigo (0.4,0.2,1.0,0.0);
    return indigo;
}
else if (color=="ivory"){

    Color ivory (0.9,0.9,0.8,0.0);
    return ivory;
}
else if (color=="khaki"){

    Color khaki (0.8,0.8,0.6,0.0);
    return khaki;
}
else if (color=="lavender"){

    Color lavender (0.6,0.2,0.8,0.0);
    return lavender;
}
else if (color=="lavenderblush"){

    Color lavenderblush (0.8,0.6,0.7,0.0);
    return lavenderblush;
}
 else if (color=="darkolivegreen"){
    Color darkolivegreen (0.4,0.5,0.2,0.0);
    return darkolivegreen;
  }
else if (color=="darkorange"){
    Color darkorange (0.9,0.5,0.1,0.0);
    return darkorange;
  }
else if (color=="darkorchid"){
    Color darkorchid (0.6,0.2,0.8,0.0);
    return darkorchid;
  }
else if (color=="darksalmon"){
    Color darksalmon (1.0,0.6,0.5,0.0);
    return darksalmon;
  }
else if (color=="darkseagreen"){
    Color darkseagreen (0.5,0.7,0.5,0.0);
    return darkseagreen;
  }
else if (color=="darkslateblue"){
    Color darkslateblue (0.2,0.2,0.5,0.0);
    return darkslateblue;
  }
else if (color=="darkslategray"){
    Color darkslategray (0.1,0.3,0.3,0.0);
    return darkslategray;
  }
else if (color=="darkturquoise"){
    Color darkturquoise (0.2,0.8,0.8,0.0);
    return darkturquoise;
  }
else if (color=="darkviolet"){
    Color darkviolet (0.6,0.1,0.8,0.0);
    return darkviolet;
  }
else if (color=="deeppink"){
    Color deeppink (1.0,0.1,0.6,0.0);
    return deeppink;
  }
else if (color=="deepskyblue"){
    Color deepskyblue (0.0,0.8,1.0,0.0);
    return deepskyblue;
  }
else if (color=="dimgray"){
    Color dimgray (0.8,0.8,0.8,0.0);
    return dimgray;
  }
else if (color=="dimgrey"){
    Color dimgrey (0.8,0.8,0.8,0.0);
    return dimgrey;
  }
else if (color=="dodgerblue"){
    Color dodgerblue (0.0,0.6,1.0,0.0);
    return dodgerblue;
  }
else if (color=="firebrick") {
    Color firebrick (0.7,0.1,0.0,0.0);
    return firebrick;
  }
else if (color=="floralwhite"){
    Color floralwhite (1.0,1.0,0.9,0.0);
    return floralwhite;
  }

  //third section
  else if(color=="lawngreen")
  {
      Color lawngreen (0.1,0.7,0,0);
      return lawngreen;
  }
  else if(color=="lemonchiffon")
  {
      Color lemonchiffon (1,1,0.8,0);
      return lemonchiffon;
  }
  else if(color=="lightblue")
  {
      Color lightblue(0.1,0.7,1,0);
      return lightblue;
  }
  else if(color=="lightcoral")
  {
      Color lightcoral(0.9,0.3,0.6,0);
      return lightcoral;
  }
  else if(color=="lightcyan")
  {
      Color lightcyan(0.4,1,1,0);
      return lightcyan;
  }
  else if(color=="lightgoldenrodyellow")
  {
      Color lightgoldenrodyellow(1,1,0.7,0);
      return lightgoldenrodyellow;
  }
  else if(color=="lightgrey")
  {
      Color lightgrey(0.7,0.7,0.7,0);
      return lightgrey;
  }
  else if(color=="lightgreen")
  {
      Color lightgreen(0.2,1,0.4,0);
      return lightgreen;
  }
  else if(color=="lightpink")
  {
      Color lightpink(1.0,0.6,0.9,0);
      return lightpink;
  }
  else if(color=="lightsalmon")
  {
      Color lightsalmon(1,0.6,0.7,0);
      return lightsalmon;
  }
  else if(color=="lightseagreen")
  {
      Color lightseagreen(0.3,1,0.6,0);
      return lightseagreen;
  }
  else if(color=="lightskyblue")
  {
      Color lightskyblue(0.6,0.6,1,0);
      return lightskyblue;
  }
  else if(color=="lightslategray")
  {
      Color lightslategray(0.8,0.8,0.8,0);
      return lightslategray;
  }
  else if(color=="lightslategrey")
  {
      Color lightslategrey(0.9,0.8,0.8,0);
      return lightslategrey;
  }
  else if(color=="lightsteelblue")
  {
      Color lightsteelblue(0.7,0.7,1,0);
      return lightsteelblue;
  }
  else if(color=="lightyellow")
  {
      Color lightyellow(1,1,0.8,0);
      return lightyellow;
  }
  else if(color=="lime")
  {
      Color lime(0.4,0.7,0.2,0);
      return lime;
  }
  else if(color=="limegreen")
  {
      Color limegreen(0.4,1,0.2,0);
      return limegreen;
  }
  else if(color=="aliceblue")
  {
      Color aliceblue(0.0,0.7,0.1,0);
      return aliceblue;
  }
  else if(color=="antiquewhite")
  {
    Color antiquewhite(1.0,0.9,0.8,0);
    return antiquewhite;
  }
  else if(color=="aqua")
  {
    Color aqua(0.0,1.0,0.9,0);
    return aqua;
  }
  else if(color=="aquamarine")
  {
    Color aquamarine(0.1,1.0,0.8,0);
    return aquamarine;
  }
  else if(color=="beige")
  {
    Color beige(0.8,0.8,0.6,0);
    return beige;
  }
  else if(color=="azure")
  {
    Color azure(0.8,0.9,0.9,0);
    return azure;
  }
  else if(color=="bisque")
  {
    Color bisque(1,0.8,0.5,0);
    return bisque;
  }
 else if(color=="blanchedalmond")
  {
    Color blanchedalmond(1,0.9,0.8,0);
    return blanchedalmond;
  }
  else if(color=="blueviolet")
  {
    Color blueviolet(0.5,0.3,0.8,0);
    return blueviolet;
  }
  else if(color=="burlywood")
  {
    Color burlywood(0.8,0.5,0.4,0);
    return burlywood;
  }
  else if(color=="brown")
  {
    Color brown(0.8,0.4,0.0,0);
    return brown;
  }
  else if(color=="cadetblue")
  {
    Color cadetblue(0.2,0.6,0.5,0);
    return cadetblue;
  }
  else if(color=="chartreuse")
  {
    Color chartreuse(0.8,0.9,0.0,0);
    return chartreuse;
  }
  else if(color=="chocolate")
  {
    Color chocolate(0.8,0.4,0.0,0);
    return chocolate;
  }
  else if(color=="coral")
  {
    Color coral(0.9,0.5,0.5,0);
    return coral;
  }
  else if(color=="cornflowerblue")
  {
    Color cornflowerblue(0.4,0.5,0.1,0);
    return cornflowerblue;
  }
  else if(color=="cornfsilk")
  {
    Color cornsilk(0.9,0.9,0.5,0);
    return cornsilk;
  }
  else if(color=="crimson")
  {
    Color crimson(0.9,0.9,0.5,0);
    return crimson;
  }
  else if(color=="crimson")
  {
    Color crimson(0.6,0.2,0.2,0);
    return crimson;
  }
  else if(color=="darkgoldenrod")
  {
    Color darkgoldenrod(0.7,0.5,0.1,0);
    return darkgoldenrod;
  }
  else if (color=="darkgrey")
  {
    Color darkgrey(0.6,0.6,0.6,0);
    return darkgrey;
  }
  else if (color=="darkkhaki")
  {
    Color darkkhaki(0.7,0.7,0.4,0);
    return darkkhaki;
  }

 else if (color=="linen"){
    Color linen (1.0,0.9,0.8,0.0);
    return linen;
  }

 else if (color=="magenta"){
    Color magenta (1.0,0.0,1.0,0.0);
    return magenta;
  }

 else if (color=="maroon"){
    Color maroon (0.5,0.0,0.0,0.0);
    return maroon;
  }

 else if (color=="mediumaquamarine"){
    Color mediumaquamarine (0.4,0.8,0.7,0.0);
    return mediumaquamarine;
  }

 else if (color=="mediumblue"){
    Color mediumblue (0.0,0.0,0.8,0.0);
    return mediumblue;
  }

 else if (color=="mediumorchid"){
    Color mediumorchid (0.7,0.3,0.8,0.0);
    return mediumorchid;
  }

 else if (color=="mediumpurple"){
    Color mediumpurple (0.5,0.4,0.8,0.0);
    return mediumpurple;
  }

 else if (color=="mediumseagreen"){
    Color mediumseagreen (0.2,0.7,0.4,0.0);
    return mediumseagreen;
  }

 else if (color=="mediumslateblue"){
    Color mediumslateblue (0.4,0.4,0.9,0.0);
    return mediumslateblue;
  }

 else if (color=="mediumspringgreen"){
    Color mediumspringgreen (0.0,0.9,0.6,0.0);
    return mediumspringgreen;
  }

 else if (color=="mediumturquoise"){
    Color mediumturquoise (0.2,0.8,0.8,0.0);
    return mediumturquoise;
  }

 else if (color=="mediumvioletred"){
    Color mediumvioletred (0.7,0.0,0.5,0.0);
    return mediumvioletred;
  }

 else if (color=="midnightblue"){
    Color midnightblue (0.0,0.0,0.4,0.0);
    return midnightblue;
  }

 else if (color=="mintcream"){
    Color mintcream (0.9,1.0,0.9,0.0);
    return mintcream;
  }

 else if (color=="mistyrose"){
    Color mistyrose (1.0,0.9,0.9,0.0);
    return mistyrose;
  }

 else if (color=="moccasin"){
    Color moccasin (1.0,0.9,0.7,0.0);
    return moccasin;
  }

 else if (color=="navajowhite"){
    Color navajowhite (1.0,0.8,0.6,0.0);
    return navajowhite;
  }

 else if (color=="navy"){
    Color navy (0.0,0.0,0.5,0.0);
    return navy;
  }
  else if (color=="red")
  {
      Color red (1,0,0,0);
      return red;
  }
  else if(color=="rosybrown")
  {
      Color rosybrown(0.7,0.2,0,0);
      return rosybrown;
  }
  else if(color=="royalblue")
  {
      Color royalblue(0.3,0,0.9,0);
      return royalblue;
  }
  else if(color=="saddlebrown")
  {
      Color saddlebrown(0.4,0.2,0.1,0);
      return saddlebrown;
  }
  else if(color=="salmon")
  {
      Color salmon(0.8,0.6,0.6,0);
      return salmon;
  }
  else if(color=="sandybrown")
  {
      Color sandybrown(0.8,0.8,0.5,0);
      return sandybrown;
  }
  else if(color=="seagreen")
  {
      Color seagreen(0.2,1,0.8,0);
      return seagreen;
  }
  else if(color=="seashell")
  {
      Color seashell(1.0,0.8,0.9,0);
      return seashell;
  }
  else if(color=="silver")
  {
      Color silver(0.9,0.9,1,0);
      return silver;
  }
  else if (color=="oldlace"){
    Color oldlace (1,0.9,1,0.0);
    return oldlace;
  }
   else if (color=="olive"){
    Color olive (0.7,0.7,0.3,0.0);
    return olive;
  }
  else if (color=="olivedrab"){
    Color olivedrab (0.5,0.5,0.3,0.0);
    return olivedrab;
  }
   else if (color=="orange"){
    Color orange (1,0.6,0.0,0.0);
    return orange;
  }
   else if (color=="orangered"){
    Color orangered (1,0.3,0.2,0.0);
    return orangered;
  }
   else if (color=="orchid"){
    Color Orchid (0.9,0.4,0.9,0.0);
    return Orchid;
  }
   else if (color=="palegoldenrod"){
    Color palegoldenrod (1,0.9,0.6,0.0);
    return palegoldenrod;
  }
   else if (color=="palegreen"){
    Color palegreen (0.6,1,0.6,0.0);
    return palegreen;
  }
    else if (color=="paleturquoise"){
    Color paleturquoise (0.7,1,1,0.0);
    return paleturquoise;
  }

   else if (color=="palevioletred"){
    Color palevioletred (0.9,0.4,0.8,0.0);
    return palevioletred;
  }
   else if (color=="papayawhip"){
    Color papayawhip (1,0.9,0.8,0.0);
    return papayawhip;
  }
  else if (color=="peachpuff"){
    Color peachpuff (1,0.7,0.6,0.0);
    return peachpuff;
  }
   else if (color=="peru"){
    Color Peru (0.9,0.5,0.2,0.0);
    return Peru;
  }
   else if (color=="pink"){
    Color pink (1,0,0.7,0.0);
    return pink;
  }
   else if (color=="plum"){
    Color plum (0.4,0.1,0.6,0.0);
    return plum;
  }
   else if (color=="powderblue"){
    Color powderblue (0.8,1,1,0.0);
    return powderblue;
  }
  else
  {
      Color purple (0.95, 0.0, 1.0, 0.4);
      return purple;
  }
}

int thisone;

int main (int argc, char *argv[]) {
	cout << "rendering ..." << endl; //tells human when program starts rendering

        string output;
        string shape = "";
        string scale = "";
        double shapeSize = 0.5;
        string color = "";
        string translucent = "";
        string reflection = "";

        char uname[UNLEN+1];
        DWORD username_len = UNLEN+1;
        GetUserName(uname, &username_len);
        string username(uname);

        std::stringstream ss;
        //ss << "C:\\Users\\" << username << "\\Documents\\GitHub\\RayTracerKCL\\RayTraceFrontEnd\\Scene.txt";
        ss << "C:\\Users\\" << username << "\\Desktop\\RayTracerKCL\\RayTraceFrontEnd\\Scene.txt";
        ///for the ray tracer to work with user input please save the project in the desktop.
        std::string fileLocation = ss.str();

        ifstream myReadFile;
        myReadFile.open(fileLocation);

        int i = 0;

        if (myReadFile.is_open())
        {
            while (!myReadFile.eof())
            {
                for(i; i < 5; i++)
                {
                    getline(myReadFile,output);
                    if(i == 0)
                    {
                        shape = output;
                        break;
                    }
                    else if (i == 1)
                    {
                        scale = output;
                        break;
                    }
                    else if(i == 2)
                    {
                        color = output;
                        break;
                    }
                    else if(i == 3)
                    {
                        translucent = output;
                        break;
                    }
                    else
                    {
                        reflection = output;
                        break;
                    }
                }
            i++;
            }
        }

	clock_t t1, t2;
	t1 = clock(); // start time

	int dpi = 72;
	int width = 500; //image width
	int height = 500; //image height
	int n = width*height;
	RGBType *pixels = new RGBType[n];

	int aadepth = 1; // signifies 4 pixels at 1 pixel so we can take the average for anti-aliasing
                     // we will not do too much anti-aliasing as it will take a long time which is bad for demonstration
	double aathreshold = 0.1;
	double aspectratio = (double)width/(double)height;
	double ambientlight = 0.2;
	double accuracy = 0.00000001;

	Vect O (0,0,0); // scene origin
	Vect X (1,0,0);
	Vect Y (0,1,0);
	Vect Z (0,0,1);

	Vect campos (3, 1.5, -4); // camera position (position is above plane)

	Vect look_at (0, 0, 0); // look_at starts at origin
	// diff_btw is the difference between the coordinates of our camera position and the coordinates of look_at vector
	Vect diff_btw (campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ());

	Vect camdir = diff_btw.negative().normalize(); // define point for camera to look at and we get the difference between our camera position and the
                                                   // point we want to look at so our camera direction can be a normalised vector in the opposite
                                                   // direction of the difference between the vector of the camera and the look at point
	Vect camright = Y.crossProduct(camdir).normalize(); // camera direction right
	Vect camdown = camright.crossProduct(camdir); // camera direction down
	Camera scene_cam (campos, camdir, camright, camdown); // camera instance (make camera)

	Color white_light (1.0, 1.0, 1.0, 0);
	Color glass (0, 0, 0, 1);
	Color tile_floor (1, 1, 1, 2);
	Color shapeColor = getColour(color);
	Color black (0.0, 0.0, 0.0, 0);

	// scene objects responding to user created text file

    shapeColor = ifTranslucentOrReflective(translucent,reflection,shapeColor);

	Sphere scene_sphere (O, getShapeSize(scale,shapeSize), shapeColor);
	if(shape == "Circle")
    {
        scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    }
    else if (shape == "Square")
    {
        shapeSize = 1.0;
        shapeSize = getShapeSize(scale,shapeSize);
        makeCube(Vect(0,0,0), Vect(shapeSize,shapeSize,shapeSize), shapeColor);
    }
    else
    {
        shapeSize = 1.0;
        shapeSize = getShapeSize(scale,shapeSize);
        makePyramid(Vect(0,0,0), Vect(shapeSize,shapeSize,shapeSize), shapeColor);
    }

	Plane scene_plane (Y, -1, tile_floor); // plane will be directly under the pretty green sphere

	// adding scene objects to the scene through scene_objects array
	scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));

	//creating light source at (-7,10,-10) so it's a little to the side
	Vect light_position (-7,10,-10);
	Light scene_light (light_position, white_light);
	vector<Source*> light_sources; // array enables multiple light sources (we won't be using multiple light sources but it makes reflections easier to do)
	light_sources.push_back(dynamic_cast<Source*>(&scene_light)); // push lights to the scene

	int thisone, aa_index;
	double xamnt, yamnt; // slightly left and right of our camera direction
	double tempRed, tempGreen, tempBlue;

	// this loop is to begin anti-aliasing (remove jagged lines) by only returning average color of pixel thus making the scene smoother
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			thisone = y*width + x; //specify individual x y pixels

			// start with a blank pixel
			double tempRed[aadepth*aadepth];
			double tempGreen[aadepth*aadepth];
			double tempBlue[aadepth*aadepth];

			for (int aax = 0; aax < aadepth; aax++) {
				for (int aay = 0; aay < aadepth; aay++) {

					aa_index = aay*aadepth + aax;

					srand(time(0));

					// create the ray from the camera to this pixel
					if (aadepth == 1) {
                        // if aadepth is 1 then there is anti-aliasing
                        // rays to go to left, right and above of the camera
						if (width > height) {
							// the image is wider than it is tall
							xamnt = ((x+0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
							yamnt = ((height - y) + 0.5)/height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamnt = (x + 0.5)/ width;
							yamnt = (((height - y) + 0.5)/height)/aspectratio - (((height - width)/(double)width)/2);
						}
						else {
							// the image is square
							xamnt = (x + 0.5)/width;
							yamnt = ((height - y) + 0.5)/height;
						}
					}
					else {
						// there is no anti-aliasing so we make there be anti-aliasing here more
						if (width > height) {
							// the image is wider than it is tall
							xamnt = ((x + (double)aax/((double)aadepth - 1))/width)*aspectratio - (((width-height)/(double)height)/2);
							yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamnt = (x + (double)aax/((double)aadepth - 1))/ width;
							yamnt = (((height - y) + (double)aax/((double)aadepth - 1))/height)/aspectratio - (((height - width)/(double)width)/2);
						}
						else {
							// the image is square
							xamnt = (x + (double)aax/((double)aadepth - 1))/width;
							yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
						}
					}

					Vect cam_ray_origin = scene_cam.getCameraPosition(); // ray origin is from camera's position
					// ray direction is camera direction including horizontal and vertical
					Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();

					Ray cam_ray (cam_ray_origin, cam_ray_direction); // creates ray

					vector<double> intersections; //array of intersections

					for (int index = 0; index < scene_objects.size(); index++) { // check for each scene object for ray intersection
						intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray)); // push intersected ray objects to scene
					}

					int index_of_winning_object = winningObjectIndex(intersections); // sorts ray of intersections and the one at the front is the
                                                                                     // intersected pixel on that surface point

					if (index_of_winning_object == -1) {
						// set the background black when ray miss object ( consider plane an object)
						tempRed[aa_index] = 0;
						tempGreen[aa_index] = 0;
						tempBlue[aa_index] = 0;
					}
					else{
						// index corresponds to an object in our scene
						if (intersections.at(index_of_winning_object) > accuracy) { // if intersection is better than accuracy then we get that color

							// determines the position and direction vectors at the point of intersection
							Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
							Vect intersecting_ray_direction = cam_ray_direction;

							// color of object seen with ambience and shading
							Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientlight);

							// find color at the intersection points with the ray ( as pixels may be different color due to the shade )
							tempRed[aa_index] = intersection_color.getColorRed();
							tempGreen[aa_index] = intersection_color.getColorGreen();
							tempBlue[aa_index] = intersection_color.getColorBlue();
						}
					}
				}
			}

			// average the pixel color
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;
			for (int iRed = 0; iRed < aadepth*aadepth; iRed++) {
				totalRed = totalRed + tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++) {
				totalGreen = totalGreen + tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++) {
				totalBlue = totalBlue + tempBlue[iBlue];
			}
			double avgRed = totalRed/(aadepth*aadepth);
			double avgGreen = totalGreen/(aadepth*aadepth);
			double avgBlue = totalBlue/(aadepth*aadepth);
			pixels[thisone].r = avgRed;
			pixels[thisone].g = avgGreen;
			pixels[thisone].b = avgBlue;
		}
	}

	savepng("scene.png",width,height,dpi,pixels); //calls method to save png file

	delete pixels, tempRed, tempGreen, tempBlue;

	t2 = clock(); // end time
	float diff = ((float)t2 - (float)t1)/1000; // time between start time and end is the length of time our program took to ray trace the image

	cout << diff << " seconds" << endl;

	return 0;
}

