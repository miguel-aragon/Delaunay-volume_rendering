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
#ifndef COLOR_TABLE_H
#define COLOR_TABLE_H

//--- Standard headers
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

class Color_Table
{

private:

	//--- Store color table. OJO This should be a pointer to allow other table sizes
	float R_ori[256]; //--- BADPROG
	float G_ori[256]; //--- BADPROG
	float B_ori[256]; //--- BADPROG
	float A_ori[256];

	float R_fin[256]; //--- BADPROG
	float G_fin[256]; //--- BADPROG
	float B_fin[256]; //--- BADPROG
	float A_fin[256];

	//--- Control the mapping of density and color table.
	//    These variables act like contrast and brightness.
	float color_table_shape; //--- Contrast
	float color_table_norm;  //--- Brightness

public:

	//================================
	//   CONSTRUCTOR/DESTRUCTOR
	//================================
	//--- Constructor
	Color_Table(void);
	//--- Destructor
	~Color_Table(void);

	//================================
	//       IO FILES
	//================================
	//--- read color table
	void read_color_table(char *fname);


	//================================
	//       ACCESOR FUNCTIONS
	//================================
	float get_R(int _index);
	float get_G(int _index);
	float get_B(int _index);
	float get_A(int _index);
	float * get_R_pointer(void);
	float * get_G_pointer(void);
	float * get_B_pointer(void);
	float * get_A_pointer(void);

	//================================
	//       MODIFY TABLE
	//================================
	void add_contrast(float _shape);
	void add_brightnes(float _brightnes);
	void update_table(void);

};



#endif
