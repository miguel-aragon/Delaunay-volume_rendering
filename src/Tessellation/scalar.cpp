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
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "scalar.h"

//==============================
//--- Pub: Constructor
//==============================
Scalar::Scalar(void) {

}

//==============================
//--- Pub: Destructor
//==============================
Scalar::~Scalar(void) {
	data.clear();
}

//====================================
float * Scalar::get_scalar(void){
	return &data[0];
}

//--- Read scalar from file
void Scalar::read_file(char *fname){
	FILE *file_in;
	char  buffer[200];
	int   dummy;
	int   tempi;
	float tempf;

	//--- Attempt to open file
	sprintf(buffer,"%s",fname);
	if(!(file_in=fopen(buffer,"r"))) {
		printf("    Can't open read densities file '%s'\n",buffer);
		exit(0);
	}

	//--- Read number of densities
	dummy = fread(&tempi, sizeof(int), 1, file_in);

	//--- Read densities from file
	for (int i=0;i<tempi;i++){
		vector<float> densi_temp;
		dummy = fread(&tempf, sizeof(float), 1, file_in);
		data.push_back(tempf);
	}

	normalize(0.0f, 1.0f);


	fclose(file_in);

}




//--- Rescale scalar array
void Scalar::contrast(float _exponent){

}

void Scalar::normalize(float _min, float _max){

	float minDen,maxDen;

	vector<float>::iterator minDenIter;
	vector<float>::iterator maxDenIter;

	//--- Normalize densities
	minDenIter = min_element(data.begin(), data.end());
	minDen     = *minDenIter;
	maxDenIter = max_element(data.begin(), data.end());
	maxDen     = *maxDenIter;
	vector<float>::iterator dataIter;
	int i=0;
	for (dataIter=data.begin(); dataIter!=data.end(); dataIter++) {
		*dataIter = pow( double((*dataIter-minDen)/(maxDen-minDen)), 1.0);
	}

}
