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
#ifndef TESSEL_H
#define TESSEL_H

#include "ofMain.h"

//--- Standard headers
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

#include "tetrahedron.h"
#include "triangle.h"
#include "vertex.h"

#include "volume_tetra.h"
#include "isosurface.h"
#include "iso_plane.h"
#include "scalar.h"

#include "../ColorTable/colortable.h"


using namespace std;

class Tessel
{

private:

	int i,j,k;

	//--- Translation and scale of the complete tessellation
	float x_off,y_off,z_off;
	float scale_factor;

    //---Tessellation properties
    float BoxSize;

	//--- Here we store the triangulation
	int n_tetrahedra, n_triangles, n_vertex;

    //--- Unique set objects
	std::set<Tetrahedron, Compare_Tetrahedra> uni_tetrahedra;
	std::set<Triangle, Compare_Triangles>     uni_triangles;

public:

	//====================================
	//   CONSTRUCTOR/DESTRUCTOR/INIT
	//====================================
	//--- Constructor
	Tessel(void);
	//--- Destructor
	~Tessel(void);
    //--- Intialize object
	void Init(void);
    //====================================
	//   WORK WWITH TESSELLATION
	//====================================
	//--- Get isosurface
	void get_IsoSurface(float _isoval);
	void get_CutPlane(float _isoval);

	//--- Ordering
	void order_triangles_by_density(void);

	//--- Attenuation factor, use for glBlendFunc(GL_ONE, GL_ONE)
	float atten_factor;

	//====================================
	//       DRAW ROUTINES
	//====================================
	void draw_volume(int draw_option, int table_Id);
	void draw_IsoSurface(GLuint index_list, int table_Id);
	void draw_CutPlane(GLuint _index_list_Id, int table_Id);
	void draw_CutPlane_flat(int table_Id, int width, int height);

	//====================================
	//       IO FILES
	//====================================
	void read_tetrahedra(char *fname);
	void read_densities(char *fname);
	void read_positions(char *fname);


	//====================================
	//    DATA STRUCTURES
	//====================================
	//--- Hold work data
	std::vector<Tetrahedron>  data_tetrahedra;
	//--- Store the raw tessellation here.
	std::vector<Tetrahedron>  raw_tetrahedra;
	std::vector<Triangle>     data_triangles;
	std::vector<Vertex>       data_vertices;
    //--- Densities
	std::vector<float> densities;


	//====================================
	//    DATA OBJECTS
	//====================================
	//--- Volume object
	Volume_tetra DelauVolume;
	//--- Isosurface object
	Isosurface   IsoSurfObject;
	//--- Plane cut object
	IsoPlane     IsoPlaneObject;
	//--- Scalar array
	Scalar       scalar1, scalar2, scalar3;

	//====================================
	//       COLOR TABLE
	//====================================
	Color_Table c_table;


	//--- Bad programming
	float densi_threshold;

};



#endif
