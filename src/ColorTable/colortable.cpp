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

#include "colortable.h"


//==============================
//--- Pub: Constructor
//==============================
Color_Table::Color_Table(void){
	color_table_shape   = 1.0;
	color_table_norm    = 1.0;
}

//==============================
//--- Pub: Destructor
//==============================
Color_Table::~Color_Table(void){

}


//==============================
//--- Pub: Accesor
//==============================
float Color_Table::get_R(int _index){
	return R_fin[_index];
}

//==============================
//--- Pub: Accesor
//==============================
float Color_Table::get_G(int _index){
	return G_fin[_index];
}

//==============================
//--- Pub: Accesor
//==============================
float Color_Table::get_B(int _index){
	return B_fin[_index];
}

//==============================
//--- Pub: Accesor
//==============================
float Color_Table::get_A(int _index){
	return A_fin[_index];
}

//==============================
//--- Pub: Accesor
//==============================
float * Color_Table::get_R_pointer(void){
	return &R_fin[0];
}
//==============================
//--- Pub: Accesor
//==============================
float * Color_Table::get_G_pointer(void){
	return &G_fin[0];
}
//==============================
//--- Pub: Accesor
//==============================
float * Color_Table::get_B_pointer(void){
	return &B_fin[0];
}
//==============================
//--- Pub: Accesor
//==============================
float * Color_Table::get_A_pointer(void){
	return &A_fin[0];
}



//==============================
//--- Pub: Change shape of mapping density-color
//==============================
void Color_Table::add_contrast(float _shape){
	color_table_shape += _shape;
	update_table();
}


//==============================
//--- Pub: Change normalization factor or brightnes
//==============================
void Color_Table::add_brightnes(float _brightnes){
	color_table_norm -= _brightnes; //--- minus because it divides
	update_table();
}


//==============================
//--- Priv: Update changes in the color table
//==============================
void Color_Table::update_table(void){
	int   n_colores = 256; //--- BADPROG
	int   i;
	float SQRT2 = 1.41421;

	for (i=0;i<n_colores;i++)
    {
		A_fin[i] = pow(A_ori[i],color_table_shape)/pow(SQRT2,color_table_norm);
    }

}



//==============================
//--- Pub: Read color table from file
//==============================
void Color_Table::read_color_table(char *fname) {
	FILE *file_ct;
	char  buffer[200];
	int   n_colores = 256; //--- BADPROG
	int   i;
	int   dummy;

	//--- Attempt to open file
	sprintf(buffer,"%s",fname);
	if(!(file_ct=fopen(buffer,"r"))) {
		printf("    Can't open read file '%s'\n",buffer);
		exit(0);
	}

	printf(">>> reading color table \n");

	for (i=0;i<n_colores;i++)
    {
		dummy = fscanf(file_ct, "%f", &R_ori[i]);
		dummy = fscanf(file_ct, "%f", &G_ori[i]);
		dummy = fscanf(file_ct, "%f", &B_ori[i]);

		//--- Normalize to 0-1
		R_ori[i] = R_ori[i]/float(n_colores);
		G_ori[i] = G_ori[i]/float(n_colores);
		B_ori[i] = B_ori[i]/float(n_colores);
		R_fin[i] = R_ori[i];
		G_fin[i] = G_ori[i];
		B_fin[i] = B_ori[i];

		A_ori[i] = float(i)/float(n_colores-1.0);
		A_fin[i] = A_ori[i];

    }
	fclose(file_ct);



}
