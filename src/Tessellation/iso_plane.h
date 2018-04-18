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
#ifndef ISO_PLANE_H
#define ISO_PLANE_H

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

#include "../ColorTable/colortable.h"


using namespace std;

class IsoPlane
{

private:

	//--- Here we store the triangulation
	int n_tetrahedra, n_triangles, n_vertex;

	//--- Isosurface value
	float isoval;

    //--- Unique set objects
	std::set<Triangle, Compare_Triangles> uni_triangles;


	//====================================
	//   PRIVATE METHODS
	//====================================
	//--- Find the place where it cuts a line in the tetrahedra
	bool isoden_cut_line(float _isoval, float den1, float den2, int vert1, int vert2, std::vector<Vertex> &data_vertices, Vertex &verte_temp);

public:

	//====================================
	//   CONSTRUCTOR/DESTRUCTOR/INIT
	//====================================
	//--- Constructor
	IsoPlane(void);
	//--- Destructor
	~IsoPlane(void);

	//====================================
	//   ACCESS FUNCTIONS
	//====================================
	float get_isoval(void);

    //====================================
	//   WORK WITH TESSELLATION
	//====================================
	//--- Get isosurface
	void get_isoPlane(int cut_axis, float _isoval, std::vector<Tetrahedron> &raw_tetrahedra, std::vector<Vertex> &data_vertices);
	//--- Delete cut plane
	void Clear();


	//--- Attenuation factor, use for glBlendFunc(GL_ONE, GL_ONE)
	float atten_factor;

	//--- Id of thr current index_list (for display list)
	GLuint index_list_Id;

	//====================================
	//       DRAW ROUTINES
	//====================================
	void draw_lines(Color_Table &c_table);
	void draw_triangles(Color_Table &c_table);

	//--- Display cut plane in orthogonal projecion
	void draw_lines_flat(Color_Table &c_table, int width, int height);
	void draw_triangles_flat(Color_Table &c_table, int width, int height);

	//====================================
	//    DATA STRUCTURES
	//====================================
	//--- Isosurfaces
	std::vector<Triangle>     iso_triangles;
	std::vector<Vertex>       iso_vertices;

    //--- Densities
	std::vector<float> densities;

	//====================================
	//       COLOR TABLE
	//====================================
	Color_Table c_table;

};


#endif
