/*

Fast pseudo volume rendering with Delaunay Tessellations.
Copyright (C) 2009-2018 Miguel Angel Aragon-Calvo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If you wish to contact the author write to:
		miguel.angel.aragon.calvo@gmail.com

*/
#ifndef SCALAR_H
#define SCALAR_H

#include "ofMain.h"

//--- Standard headers
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

class Scalar
{

private:

	//--- Here we store the triangulation
	int n_data;

public:

	//====================================
	//   CONSTRUCTOR/DESTRUCTOR/INIT
	//====================================
	//--- Constructor
	Scalar(void);
	//--- Destructor
	~Scalar(void);

	//====================================
	//   ACCESORS
	//====================================
	float * get_scalar(void);

	//--- Read scalar from file
	void read_file(char *fname);

	//--- Rescale scalar array
	void contrast(float _exponent);
	void normalize(float _min, float _max);


	//--- Data
	std::vector<float> data;

};


#endif
