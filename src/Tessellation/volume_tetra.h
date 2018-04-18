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
#ifndef VOLUME_TETRA_H
#define VOLUME_TETRA_H

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

class Volume_tetra
{

private:

	//--- Here we store the triangulation
	int n_triangles, n_vertex;

public:

	//====================================
	//   CONSTRUCTOR/DESTRUCTOR/INIT
	//====================================
	//--- Constructor
	Volume_tetra(void);
	//--- Destructor
	~Volume_tetra(void);

	//====================================
	//       DRAW ROUTINES
	//====================================
	void draw_lines(std::vector<Vertex> &data_vertices, std::vector<Triangle> &data_triangles, Color_Table &c_table);  //--- color table better as shader
	void draw_volume(std::vector<Vertex> &data_vertices, std::vector<Triangle> &data_triangles, Color_Table &c_table);  //--- color table better as shader
	void draw_points(std::vector<Vertex> &data_vertices, Color_Table &c_table);  //--- color table better as shader

	void draw_volume_shader(std::vector<Vertex> &data_vertices, std::vector<Triangle> &data_triangles, Color_Table &c_table);  //--- color table better as shader

	//--- Bad programming !!! IMPLEMENT AS SHADER
	float densi_threshold;
	float atten_factor;
	float BoxSize;

	int check_boundary_tetra(Tetrahedron tetra_temp, std::vector<Vertex> &data_vertices);

};


#endif
