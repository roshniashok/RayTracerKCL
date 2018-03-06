//include guards
#ifndef _SOURCE_H
#define _SOURCE_H

class Source {
	public: //class needs to be public so all methods can be called from main

	Source(); //can have instants with no parameters (just to prevent errors)

	// functions to return values of light source color and position
	virtual Vect getLightPosition() {return Vect(0, 0, 0);}
	virtual Color getLightColor() {return Color(1, 1, 1, 0);}

};

Source::Source() {} // parameters light source will have if they had not been provided

#endif
