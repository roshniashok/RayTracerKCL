//include guards
#ifndef _COLOR_H
#define _COLOR_H

class Color {
	double red, green, blue, special; //Color needs 4 values (special is reflection)
                                      // special values corresponds to reflectivity and shine

	public: //class needs to be public so all methods can be called from main

	Color (); //can have instants with no parameters (just to prevent errors)

	Color (double, double, double, double);

	// functions to return color values
	double getColorRed() { return red; }
	double getColorGreen() { return green; }
	double getColorBlue() { return blue; }
	double getColorSpecial() { return special; }

	// functions to set color values
	double setColorRed(double redValue) { red = redValue; }
	double setColorGreen(double greenValue) { green = greenValue; }
	double setColorBlue(double blueValue) { blue = blueValue; }
	double setColorSpecial(double specialValue) { special = specialValue; }

	double brightness() { // returns pixel brightness
		return(red + green + blue)/3;
	}

	Color colorScalar(double scalar) { // scales color by scalar value
		return Color (red*scalar, green*scalar, blue*scalar, special);
	}

	Color colorAdd(Color color) { // takes in a color (need to be able to add colors)
		return Color (red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), special);
	}

	Color colorMultiply(Color color) { // takes in a color (need to be able to multiply colors)
		return Color (red*color.getColorRed(), green*color.getColorGreen(), blue*color.getColorBlue(), special);
	}

	Color colorAverage(Color color) { // takes in a color (need to be able to average between two colors)
		return Color ((red + color.getColorRed())/2, (green + color.getColorGreen())/2, (blue + color.getColorBlue())/2, special);
	}

	Color clip() { // clip colors beyond a value so the surfaces do not get an inverted color of the shape when being shaded
		double alllight = red + green + blue;
		double excesslight = alllight - 3;
		if (excesslight > 0) {
			red = red + excesslight*(red/alllight);
			green = green + excesslight*(green/alllight);
			blue = blue + excesslight*(blue/alllight);
		}

		// prevent users from having given values more or less than the maximum or minimum of a color
		if (red > 1) {red = 1;}
		if (green > 1) {green = 1;}
		if (blue > 1) {blue = 1;}
		if (red < 0) {red = 0;}
		if (green < 0) {green = 0;}
		if (blue < 0) {blue = 0;}

		return Color (red, green, blue, special);
	}
};

Color::Color () { // parameters color will have if they had not been provided
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color (double r, double g, double b, double s) { // parameters color will have if they are provided
	red = r;
	green = g;
	blue = b;
	special = s;
}

#endif
