#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
			for (int c = 0; c < secondary_intersections.size(); c++) {
				if (secondary_intersections.at(c) > accuracy) { // if second intersection is better than accuracy then we get that color
					if (secondary_intersections.at(c) <= distance_to_light_magnitude) { // closest pixel is shaded
						shadowed = true;
					}
				}
				break;
			}

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
    ///Back Side of cube      ### this side will not be seen ###
    //scene_objects.push_back(new Triangle(D, A, corner1, color));
    //scene_objects.push_back(new Triangle(corner1, E, D, color));
    ///Left Side of cube      ### this side will not be seen ###
    //scene_objects.push_back(new Triangle(corner2, B, A, color));
    //scene_objects.push_back(new Triangle(A, D, corner2, color));
    ///Front Side of cube
    scene_objects.push_back(new Triangle(F, C, B, color));
    scene_objects.push_back(new Triangle(B, corner2, F, color));
    ///Right Side of cube
    scene_objects.push_back(new Triangle(E, corner1, C, color));
    scene_objects.push_back(new Triangle(C, F, E, color));
    ///Bottom Side of cube    ### this side will not be seen ###
    //scene_objects.push_back(new Triangle(D, E, F, color));
    //scene_objects.push_back(new Triangle(F, corner2, D, color));
    ///Top Side of cube       ### this side will not be seen ###
    //scene_objects.push_back(new Triangle(corner1, A, B, color));
    //scene_objects.push_back(new Triangle(B, C, corner1, color));

}

int thisone;

int main (int argc, char *argv[]) {
	cout << "rendering ..." << endl; //tells human when program starts rendering

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

	Vect new_sphere_location (1.75, -0.25, 0);
	Vect third_sphere_locatin (-2,1,0);

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
	Color pretty_green (0.5, 1.0, 0.75, 0);
	Color glass (0, 0, 0, 1);
	Color blue (0.55, 0.4, 1.0, 0);
	Color tile_floor (1, 1, 1, 2);
	Color purple (0.95, 0.0, 1.0, 0.4);
	Color black (0.0, 0.0, 0.0, 0);

	// scene objects
	Sphere scene_sphere (O, 0.8, pretty_green); // green sphere has radius 1 and is located at origin
	Sphere scene_sphere2 (new_sphere_location, 0.4, glass);
	Sphere scene_sphere3 (third_sphere_locatin, 0.4, purple);
	Plane scene_plane (Y, -1, tile_floor); // plane will be directly under the pretty green sphere
	//Triangle scene_triangle (Vect(3,0,0), Vect(0,3,0), Vect(0,0,3), blue); // to create a triangle in the scene, triangles can be used to make a cube

	// adding scene objects to the scene through scene_objects array
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere)); // push objects to the scene
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere3));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
	//scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle));

	makeCube(Vect(1.7,1.7,1.7), Vect(0.7,0.7,0.7), blue);

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

