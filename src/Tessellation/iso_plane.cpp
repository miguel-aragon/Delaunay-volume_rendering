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

#include "iso_plane.h"
#include "indexed_comparison.h"





/*/////////////////////////////////////////

 PUBLIC FUNCTIONS

 */////////////////////////////////////////

//==============================
//--- Pub: Constructor
//==============================
IsoPlane::IsoPlane(void) {
	atten_factor=20.0;
}

//==============================
//--- Pub: Destructor
//==============================
IsoPlane::~IsoPlane(void) {
	iso_triangles.clear();
	iso_vertices.clear();
}

//==============================
//--- Pub: Clear
//==============================
void IsoPlane::Clear(void) {
	iso_triangles.clear();
	iso_vertices.clear();
}

//==============================
//--- Pub: Access
//==============================
float IsoPlane::get_isoval(void){
	return isoval;
}



bool IsoPlane::isoden_cut_line(float _isoval, float den1, float den2, int vert1, int vert2, std::vector<Vertex> &data_vertices, Vertex &verte_temp){

	//--- Check if the isodensity lies between the two vertices
	if ((den2 > _isoval) && (_isoval > den1)) {
		//--- Parameter
		float t_par;

		//--- Get two point parametric curve of the line along the two vertices
		t_par = (_isoval - data_vertices[vert1].xyz[0]) / (data_vertices[vert2].xyz[0] - data_vertices[vert1].xyz[0]);
		verte_temp.xyz[0] = (1-t_par)*data_vertices[vert1].xyz[0] + t_par*data_vertices[vert2].xyz[0];
		verte_temp.xyz[1] = (1-t_par)*data_vertices[vert1].xyz[1] + t_par*data_vertices[vert2].xyz[1];
		verte_temp.xyz[2] = (1-t_par)*data_vertices[vert1].xyz[2] + t_par*data_vertices[vert2].xyz[2];
		verte_temp.alpha[0] = data_vertices[vert1].alpha[0];

		return true;
	}
	return false;
}


//==============================
//--- Pub: Compute isosurface. Use the raw tetrahedra
//==============================
void IsoPlane::get_isoPlane(int cut_axis, float  _isoval, std::vector<Tetrahedron> &raw_tetrahedra, std::vector<Vertex> &data_vertices){
	bool flag;

	//--- IMPORTANT! First check if there is already an iso plane. If so then delete it
	//    so we can compute it gain.
	if (iso_triangles.size() != 0) Clear();

	//--- Update the isodensityvalue
	isoval = _isoval;

	//--- Go though all the combinations and check if the tetrahedra cuts the isosurface
    Triangle    trian_temp;
	Vertex      verte_temp;

	//--- Important, triangle offset index must be initialized!!!
	int off_triangle=0;
	//--- Loop over all tetrahedra and find the intersections with the isosurface
	for (vector <Tetrahedron> :: iterator TetraIter=raw_tetrahedra.begin(); TetraIter != raw_tetrahedra.end(); TetraIter++) {


        //--- Get density at the vertices tetrahedra and sort them
        float tetra_den[4] = {data_vertices[TetraIter->vert[0]].xyz[0],data_vertices[TetraIter->vert[1]].xyz[0], data_vertices[TetraIter->vert[2]].xyz[0], data_vertices[TetraIter->vert[3]].xyz[0] };
		int   tetra_ind[4] = {0,1,2,3};
		vector<float> vec_den (tetra_den, tetra_den + sizeof(tetra_den) / sizeof(tetra_den[0]));
		vector<int>   vec_ind (tetra_ind, tetra_ind + sizeof(tetra_ind) / sizeof(tetra_ind[0]));
		sort(vec_ind.begin(),vec_ind.end(),compare(&vec_den));

		//--- Get rid of tetrahedra with repeated values
		if(tetra_den[vec_ind[0]]==tetra_den[vec_ind[1]] || tetra_den[vec_ind[1]]==tetra_den[vec_ind[2]] || tetra_den[vec_ind[2]]==tetra_den[vec_ind[3]]) continue;

		//----------------------------------------------
		//--- Is the isodensity inside the tetrahedra?
		//----------------------------------------------
        if ( (tetra_den[vec_ind[3]] < _isoval) || (_isoval < tetra_den[vec_ind[0]]) ) continue;


		//--- There are two posibilities for the intersection between the isodensity and the tetrahedra:
		//---   * a triangle
		//---   * a quad formed by two triangles
		if ((tetra_den[vec_ind[2]] > _isoval) && (_isoval > tetra_den[vec_ind[1]])) {

			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[1]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[1]], data_vertices, verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[2]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[2]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[1]], tetra_den[vec_ind[2]], TetraIter->vert[vec_ind[1]], TetraIter->vert[vec_ind[2]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[1]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[1]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[2]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[2]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);


			//--- Add the current triangle's indexes
			trian_temp.vert[0] = off_triangle*3+0;
			trian_temp.vert[1] = off_triangle*3+1;
			trian_temp.vert[2] = off_triangle*3+2;
			iso_triangles.push_back(trian_temp);
			off_triangle++;

			//=== ADD MISSING VERTICES FOR SECOND TRIANGLE [triangle repeated, fix !!!]
			//--- VERTEX 2
			//--- Add the two elements before the last
			verte_temp.xyz[0]   = iso_vertices[iso_vertices.size()-2].xyz[0];
			verte_temp.xyz[1]   = iso_vertices[iso_vertices.size()-2].xyz[1];
			verte_temp.xyz[2]   = iso_vertices[iso_vertices.size()-2].xyz[2];
			verte_temp.alpha[0] = iso_vertices[iso_vertices.size()-2].alpha[0];
			iso_vertices.push_back(verte_temp);

			//--- VERTEX 3
			//--- Add the two elements before the last
			verte_temp.xyz[0]   = iso_vertices[iso_vertices.size()-4].xyz[0];
			verte_temp.xyz[1]   = iso_vertices[iso_vertices.size()-4].xyz[1];
			verte_temp.xyz[2]   = iso_vertices[iso_vertices.size()-4].xyz[2];
			verte_temp.alpha[0] = iso_vertices[iso_vertices.size()-4].alpha[0];
			iso_vertices.push_back(verte_temp);

			//--- Add the current tringle's indexes
			trian_temp.vert[0] = off_triangle*3+0;
			trian_temp.vert[1] = off_triangle*3+1;
			trian_temp.vert[2] = off_triangle*3+2;
			iso_triangles.push_back(trian_temp);
			off_triangle++;

		}
		else {

			//--- Check where the isodensity cuts the line between vertex 1 and vertex 2
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[1]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[1]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[2]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[2]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[1]], tetra_den[vec_ind[2]], TetraIter->vert[vec_ind[1]], TetraIter->vert[vec_ind[2]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[1]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[1]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[2]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[2]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp);
			if (flag) iso_vertices.push_back(verte_temp);

			//--- Add the current triangle's indexes
			trian_temp.vert[0] = off_triangle*3+0;
			trian_temp.vert[1] = off_triangle*3+1;
			trian_temp.vert[2] = off_triangle*3+2;
			iso_triangles.push_back(trian_temp);
			off_triangle++;

		}


	}//--- end for


}


void IsoPlane::draw_lines(Color_Table &c_table){
	float x,y,z;

	glBegin(GL_LINES);

	float fac2 = 16.0;

	//--- Go through all isodensity triangles
	vector <Triangle> :: iterator TriIter;
	for (TriIter=iso_triangles.begin(); TriIter != iso_triangles.end(); TriIter++){

	    //--- Line 1
		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);

		x = iso_vertices[TriIter->vert[0]].xyz[0];
		y = iso_vertices[TriIter->vert[0]].xyz[1];
		z = iso_vertices[TriIter->vert[0]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[1]].xyz[0];
		y = iso_vertices[TriIter->vert[1]].xyz[1];
		z = iso_vertices[TriIter->vert[1]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		//--- Line 2
		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[1]].xyz[0];
		y = iso_vertices[TriIter->vert[1]].xyz[1];
		z = iso_vertices[TriIter->vert[1]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[2]].xyz[0];
		y = iso_vertices[TriIter->vert[2]].xyz[1];
		z = iso_vertices[TriIter->vert[2]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		//--- Line 3
		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[0]].xyz[0];
		y = iso_vertices[TriIter->vert[0]].xyz[1];
		z = iso_vertices[TriIter->vert[0]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  pow(iso_vertices[TriIter->vert[2]].alpha[0],1));
		x = iso_vertices[TriIter->vert[2]].xyz[0];
		y = iso_vertices[TriIter->vert[2]].xyz[1];
		z = iso_vertices[TriIter->vert[2]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);
	}

	glEnd();

}


void IsoPlane::draw_triangles(Color_Table &c_table){
	float x,y,z;

	glBegin(GL_TRIANGLES);

	//--- Go though all isodensity triangles
	for (vector <Triangle> :: iterator TriIter=iso_triangles.begin(); TriIter != iso_triangles.end(); TriIter++){

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*24,
				  c_table.get_G(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*24,
				  c_table.get_B(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*24,
				  pow(iso_vertices[TriIter->vert[0]].alpha[0],0.5f));
		x = iso_vertices[TriIter->vert[0]].xyz[0];
		y = iso_vertices[TriIter->vert[0]].xyz[1];
		z = iso_vertices[TriIter->vert[0]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*24,
				  c_table.get_G(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*24,
				  c_table.get_B(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*24,
				  pow(iso_vertices[TriIter->vert[1]].alpha[0],0.5f));
		x = iso_vertices[TriIter->vert[1]].xyz[0];
		y = iso_vertices[TriIter->vert[1]].xyz[1];
		z = iso_vertices[TriIter->vert[1]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*24,
				  c_table.get_G(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*24,
				  c_table.get_B(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*24,
				  pow(iso_vertices[TriIter->vert[2]].alpha[0],0.5f));
		x = iso_vertices[TriIter->vert[2]].xyz[0];
		y = iso_vertices[TriIter->vert[2]].xyz[1];
		z = iso_vertices[TriIter->vert[2]].xyz[2];
		glVertex3f((x-0.5)*10,(y-0.5)*10,(z-0.5)*10);
	}

	glEnd();

}


//===============================================


void IsoPlane::draw_lines_flat(Color_Table &c_table, int width, int height){
	float x,y,z;
	float fac2 = 32.0;


	glBegin(GL_LINES);

	//--- Go through all isodensity triangles
	vector <Triangle> :: iterator TriIter;
	for (TriIter=iso_triangles.begin(); TriIter != iso_triangles.end(); TriIter++){

	    //--- Line 1
		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[0]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[0]].xyz[2]*height;
		glVertex3f(x,y,0);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[1]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[1]].xyz[2]*height;
		glVertex3f(x,y,0);

		//--- Line 2
		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[1]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[1]].xyz[2]*height;
		glVertex3f(x,y,0);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[2]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[2]].xyz[2]*height;
		glVertex3f(x,y,0);

		//--- Line 3
		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  isoval*fac2);
		x = iso_vertices[TriIter->vert[0]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[0]].xyz[2]*height;
		glVertex3f(x,y,0);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  pow(iso_vertices[TriIter->vert[2]].alpha[0],1));
		x = iso_vertices[TriIter->vert[2]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[2]].xyz[2]*height;
		glVertex3f(x,y,0);
	}

	glEnd();

}


void IsoPlane::draw_triangles_flat(Color_Table &c_table, int width, int height){
	float x,y,z;
	float fac2 = 16.0;

	glBegin(GL_TRIANGLES);

	//--- Go though all isodensity triangles
	for (vector <Triangle> :: iterator TriIter=iso_triangles.begin(); TriIter != iso_triangles.end(); TriIter++){


		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[0]].alpha[0]*255))/atten_factor*fac2,
				  pow(iso_vertices[TriIter->vert[0]].alpha[0],1));
		x = iso_vertices[TriIter->vert[0]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[0]].xyz[2]*height;
		glVertex3f(x,y,0);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[1]].alpha[0]*255))/atten_factor*fac2,
				  pow(iso_vertices[TriIter->vert[1]].alpha[0],1));
		x = iso_vertices[TriIter->vert[1]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[1]].xyz[2]*height;
		glVertex3f(x,y,0);

		glColor4f(c_table.get_R(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_G(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  c_table.get_B(int(iso_vertices[TriIter->vert[2]].alpha[0]*255))/atten_factor*fac2,
				  pow(iso_vertices[TriIter->vert[2]].alpha[0],1));
		x = iso_vertices[TriIter->vert[2]].xyz[1]*width;
		y = iso_vertices[TriIter->vert[2]].xyz[2]*height;
		glVertex3f(x,y,0);

	}

	glEnd();

}
