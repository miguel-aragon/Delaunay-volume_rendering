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

#include "volume_tetra.h"


/*/////////////////////////////////////////

 PUBLIC FUNCTIONS

 */////////////////////////////////////////

//==============================
//--- Pub: Constructor
//==============================
Volume_tetra::Volume_tetra(void) {
	atten_factor = 2.0;
	densi_threshold = 0.2;
}

//==============================
//--- Pub: Destructor
//==============================
Volume_tetra::~Volume_tetra(void) {

}


void Volume_tetra::draw_lines(std::vector<Vertex> &data_vertices, std::vector<Triangle> &data_triangles, Color_Table &c_table){

	glBegin(GL_LINES);

	//--- Go though all the combinations and check if the tetrahedra cuts the isosurface
	vector <Triangle> :: iterator TriIter;
	vector <float>    :: iterator DenIter;

	float x,y,z;
	//--- Loop over all triangles
	for (TriIter=data_triangles.begin(); TriIter != data_triangles.end(); TriIter++){

		//--- bad fix for boundaries
		if (data_vertices[TriIter->vert[0]].xyz[0] < 0.05 || data_vertices[TriIter->vert[0]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[0]].xyz[1] < 0.05 || data_vertices[TriIter->vert[0]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[0]].xyz[2] < 0.05 || data_vertices[TriIter->vert[0]].xyz[2] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[0] < 0.05 || data_vertices[TriIter->vert[1]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[1] < 0.05 || data_vertices[TriIter->vert[1]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[2] < 0.05 || data_vertices[TriIter->vert[1]].xyz[2] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[0] < 0.05 || data_vertices[TriIter->vert[2]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[1] < 0.05 || data_vertices[TriIter->vert[2]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[2] < 0.05 || data_vertices[TriIter->vert[2]].xyz[2] > 0.95 ) continue;

		//--- LINE 1
		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[0]].alpha[0]*255)),
				  c_table.get_G(int(data_vertices[TriIter->vert[0]].alpha[0]*255)),
				  c_table.get_B(int(data_vertices[TriIter->vert[0]].alpha[0]*255)),
				  pow(data_vertices[TriIter->vert[0]].alpha[0],3)/atten_factor);
				  //0.01);
		x = data_vertices[TriIter->vert[0]].xyz[0];
		y = data_vertices[TriIter->vert[0]].xyz[1];
		z = data_vertices[TriIter->vert[0]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[1]].alpha[0]*255)),
				  c_table.get_G(int(data_vertices[TriIter->vert[1]].alpha[0]*255)),
				  c_table.get_B(int(data_vertices[TriIter->vert[1]].alpha[0]*255)),
				  pow(data_vertices[TriIter->vert[1]].alpha[0],3)/atten_factor);
				  //0.01);
		x = data_vertices[TriIter->vert[1]].xyz[0];
		y = data_vertices[TriIter->vert[1]].xyz[1];
		z = data_vertices[TriIter->vert[1]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);
		//--- LINE 2
		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[0]].alpha[0]*255)),
				  c_table.get_G(int(data_vertices[TriIter->vert[0]].alpha[0]*255)),
				  c_table.get_B(int(data_vertices[TriIter->vert[0]].alpha[0]*255)),
				  pow(data_vertices[TriIter->vert[0]].alpha[0],3)/atten_factor);
				  //0.01);
		x = data_vertices[TriIter->vert[0]].xyz[0];
		y = data_vertices[TriIter->vert[0]].xyz[1];
		z = data_vertices[TriIter->vert[0]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[2]].alpha[0]*255)),
				  c_table.get_G(int(data_vertices[TriIter->vert[2]].alpha[0]*255)),
				  c_table.get_B(int(data_vertices[TriIter->vert[2]].alpha[0]*255)),
				  pow(data_vertices[TriIter->vert[2]].alpha[0],3)/atten_factor);
				  //0.01);
		x = data_vertices[TriIter->vert[2]].xyz[0];
		y = data_vertices[TriIter->vert[2]].xyz[1];
		z = data_vertices[TriIter->vert[2]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);
		//--- LINE 3
		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[1]].alpha[0]*255)),
				  c_table.get_G(int(data_vertices[TriIter->vert[1]].alpha[0]*255)),
				  c_table.get_B(int(data_vertices[TriIter->vert[1]].alpha[0]*255)),
				  pow(data_vertices[TriIter->vert[1]].alpha[0],3)/atten_factor);
				  //0.01);
		x = data_vertices[TriIter->vert[1]].xyz[0];
		y = data_vertices[TriIter->vert[1]].xyz[1];
		z = data_vertices[TriIter->vert[1]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[2]].alpha[0]*255)),
				  c_table.get_G(int(data_vertices[TriIter->vert[2]].alpha[0]*255)),
				  c_table.get_B(int(data_vertices[TriIter->vert[2]].alpha[0]*255)),
				  pow(data_vertices[TriIter->vert[2]].alpha[0],3)/atten_factor);
				  //0.01);
		x = data_vertices[TriIter->vert[2]].xyz[0];
		y = data_vertices[TriIter->vert[2]].xyz[1];
		z = data_vertices[TriIter->vert[2]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

	}

	glEnd();

}

void Volume_tetra::draw_volume(std::vector<Vertex> &data_vertices, std::vector<Triangle> &data_triangles, Color_Table &c_table){

	glBegin(GL_TRIANGLES);

	//--- Go though all the combinations and check if the tetrahedra cuts the isosurface
	vector <Triangle> :: iterator TriIter;
	vector <float>    :: iterator DenIter;

	printf("Density threshold: %f \n", densi_threshold);

	float x,y,z;
	//--- Loop over all triangles
	for (TriIter=data_triangles.begin(); TriIter != data_triangles.end(); TriIter++){

		if (data_vertices[TriIter->vert[0]].alpha[0] < densi_threshold) continue;

		//--- bad fix for boundaries
		if (data_vertices[TriIter->vert[0]].xyz[0] < 0.05 || data_vertices[TriIter->vert[0]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[0]].xyz[1] < 0.05 || data_vertices[TriIter->vert[0]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[0]].xyz[2] < 0.05 || data_vertices[TriIter->vert[0]].xyz[2] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[0] < 0.05 || data_vertices[TriIter->vert[1]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[1] < 0.05 || data_vertices[TriIter->vert[1]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[2] < 0.05 || data_vertices[TriIter->vert[1]].xyz[2] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[0] < 0.05 || data_vertices[TriIter->vert[2]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[1] < 0.05 || data_vertices[TriIter->vert[2]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[2] < 0.05 || data_vertices[TriIter->vert[2]].xyz[2] > 0.95 ) continue;

		if (data_vertices[TriIter->vert[0]].alpha[0] < 0.22) continue;

		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor,
				  c_table.get_G(int(data_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor,
				  c_table.get_B(int(data_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor,
				  pow(data_vertices[TriIter->vert[0]].alpha[0],3));
		x = data_vertices[TriIter->vert[0]].xyz[0];
		y = data_vertices[TriIter->vert[0]].xyz[1];
		z = data_vertices[TriIter->vert[0]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor,
				  c_table.get_G(int(data_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor,
				  c_table.get_B(int(data_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor,
				  pow(data_vertices[TriIter->vert[1]].alpha[0],3));
		x = data_vertices[TriIter->vert[1]].xyz[0];
		y = data_vertices[TriIter->vert[1]].xyz[1];
		z = data_vertices[TriIter->vert[1]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(data_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor,
				  c_table.get_G(int(data_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor,
				  c_table.get_B(int(data_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor,
				  pow(data_vertices[TriIter->vert[2]].alpha[0],3));
		x = data_vertices[TriIter->vert[2]].xyz[0];
		y = data_vertices[TriIter->vert[2]].xyz[1];
		z = data_vertices[TriIter->vert[2]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);
	}

	glEnd();

}


void Volume_tetra::draw_volume_shader(std::vector<Vertex> &data_vertices, std::vector<Triangle> &data_triangles, Color_Table &c_table){

	glBegin(GL_TRIANGLES);

	//--- Go though all the combinations and check if the tetrahedra cuts the isosurface
	vector <Triangle> :: iterator TriIter;
	vector <float>    :: iterator DenIter;

	printf("Density threshold: %f \n", densi_threshold);

	float x,y,z;
	//--- Loop over all triangles
	for (TriIter=data_triangles.begin(); TriIter != data_triangles.end(); TriIter++){

		if (data_vertices[TriIter->vert[0]].alpha[0] < densi_threshold) continue;

		//--- bad fix for boundaries
		if (data_vertices[TriIter->vert[0]].xyz[0] < 0.05 || data_vertices[TriIter->vert[0]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[0]].xyz[1] < 0.05 || data_vertices[TriIter->vert[0]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[0]].xyz[2] < 0.05 || data_vertices[TriIter->vert[0]].xyz[2] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[0] < 0.05 || data_vertices[TriIter->vert[1]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[1] < 0.05 || data_vertices[TriIter->vert[1]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[1]].xyz[2] < 0.05 || data_vertices[TriIter->vert[1]].xyz[2] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[0] < 0.05 || data_vertices[TriIter->vert[2]].xyz[0] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[1] < 0.05 || data_vertices[TriIter->vert[2]].xyz[1] > 0.95 ||
			data_vertices[TriIter->vert[2]].xyz[2] < 0.05 || data_vertices[TriIter->vert[2]].xyz[2] > 0.95 ) continue;

		if (data_vertices[TriIter->vert[0]].alpha[0] < 0.22) continue;

		glColor4f(data_vertices[TriIter->vert[0]].alpha[0],
				  data_vertices[TriIter->vert[0]].alpha[0],
				  data_vertices[TriIter->vert[0]].alpha[0],
				  data_vertices[TriIter->vert[0]].alpha[0]);
		x = data_vertices[TriIter->vert[0]].xyz[0];
		y = data_vertices[TriIter->vert[0]].xyz[1];
		z = data_vertices[TriIter->vert[0]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(data_vertices[TriIter->vert[1]].alpha[0],
				  data_vertices[TriIter->vert[1]].alpha[0],
				  data_vertices[TriIter->vert[1]].alpha[0],
				  data_vertices[TriIter->vert[1]].alpha[0]);
		x = data_vertices[TriIter->vert[1]].xyz[0];
		y = data_vertices[TriIter->vert[1]].xyz[1];
		z = data_vertices[TriIter->vert[1]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(data_vertices[TriIter->vert[2]].alpha[0],
				  data_vertices[TriIter->vert[2]].alpha[0],
				  data_vertices[TriIter->vert[2]].alpha[0],
				  data_vertices[TriIter->vert[2]].alpha[0]);
		x = data_vertices[TriIter->vert[2]].xyz[0];
		y = data_vertices[TriIter->vert[2]].xyz[1];
		z = data_vertices[TriIter->vert[2]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

	}

	glEnd();

}



void Volume_tetra::draw_points(std::vector<Vertex> &data_vertices, Color_Table &c_table){

	glBegin(GL_POINTS);

	//--- Go though all the combinations and check if the tetrahedra cuts the isosurface
	vector <Vertex> :: iterator VertexIter;

	//--- Loop over all vertices
	for (VertexIter=data_vertices.begin(); VertexIter != data_vertices.end(); VertexIter++) {
		//--- Vertex color
		glColor4f(c_table.get_R(int(VertexIter->alpha[0]*255)),
				  c_table.get_G(int(VertexIter->alpha[0]*255)),
				  c_table.get_B(int(VertexIter->alpha[0]*255)),
				  pow(VertexIter->alpha[0],0.5f));
		glVertex3f((VertexIter->xyz[0]-0.5)*10,(VertexIter->xyz[1]-0.5)*10,(VertexIter->xyz[2]-0.5)*10);

	}

	glEnd();


}

int Volume_tetra::check_boundary_tetra(Tetrahedron tetra_temp, std::vector<Vertex> &data_vertices){

    float tetra_posX[4] = {data_vertices[tetra_temp.vert[0]].xyz[0],data_vertices[tetra_temp.vert[1]].xyz[0], data_vertices[tetra_temp.vert[2]].xyz[0], data_vertices[tetra_temp.vert[3]].xyz[0] };
    sort( &tetra_posX[0], &tetra_posX[4]);
    if ( abs(tetra_posX[3] - tetra_posX[0]) > BoxSize/2.0 ) {
        return 1;
    }

    float tetra_posY[4] = {data_vertices[tetra_temp.vert[0]].xyz[1],data_vertices[tetra_temp.vert[1]].xyz[1], data_vertices[tetra_temp.vert[2]].xyz[1], data_vertices[tetra_temp.vert[3]].xyz[1] };
    sort( &tetra_posY[0], &tetra_posY[4]);
    if ( abs(tetra_posY[3] - tetra_posY[0]) > BoxSize/2.0 ) {
        return 1;
    }

    float tetra_posZ[4] = {data_vertices[tetra_temp.vert[0]].xyz[2],data_vertices[tetra_temp.vert[1]].xyz[2], data_vertices[tetra_temp.vert[2]].xyz[2], data_vertices[tetra_temp.vert[3]].xyz[2] };
    sort( &tetra_posZ[0], &tetra_posZ[4]);
    if ( abs(tetra_posZ[3] - tetra_posZ[0]) > BoxSize/2.0 ) {
        return 1;
    }

	return 0;
}
