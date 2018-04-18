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

#include "isosurface.h"
#include "indexed_comparison.h"




/*/////////////////////////////////////////

 PUBLIC FUNCTIONS

 */////////////////////////////////////////

//==============================
//--- Pub: Constructor
//==============================
Isosurface::Isosurface(void) {
	atten_factor=20.0;
}

//==============================
//--- Pub: Destructor
//==============================
Isosurface::~Isosurface(void) {
	iso_triangles.clear();
	iso_vertices.clear();
}

//==============================
//--- Pub: Clear
//==============================
void Isosurface::Clear(void) {
	iso_triangles.clear();
	iso_vertices.clear();
}


float Isosurface::get_isoval(void){
	return isoval;
}

bool Isosurface::isoden_cut_line(float _isoval, float den1, float den2, int vert1, int vert2, std::vector<Vertex> &data_vertices, Vertex &verte_temp, float *scalar){

	//--- Check if the isodensity lies between the two vertices
	if ((den2 > _isoval) && (_isoval > den1)) {
		//--- Parameter
		float t_par;

		//--- Get two point parametric curve of the line along the two vertices
		t_par = (_isoval - scalar[vert1]) / (scalar[vert2] - scalar[vert1]);
		verte_temp.xyz[0] = (1-t_par)*data_vertices[vert1].xyz[0] + t_par*data_vertices[vert2].xyz[0];
		verte_temp.xyz[1] = (1-t_par)*data_vertices[vert1].xyz[1] + t_par*data_vertices[vert2].xyz[1];
		verte_temp.xyz[2] = (1-t_par)*data_vertices[vert1].xyz[2] + t_par*data_vertices[vert2].xyz[2];
		verte_temp.alpha[0] = _isoval;

		return true;
	}
	return false;
}


//==============================
//--- Pub: Compute isosurface. Use the raw tetrahedra
//==============================
void Isosurface::get_isosurface(float  _isoval, std::vector<Tetrahedron> &raw_tetrahedra, std::vector<Vertex> &data_vertices, float *scalar){
	bool flag;

	//--- IMPORTANT! First check if there is already an isosurface. If so then delete it
	//    so we can compute it gain.
	if (iso_triangles.size() != 0) Clear();

	//--- Update the isodensity value
	isoval = _isoval;

	//--- Go though all the combinations and check if the tetrahedra cuts the isosurface
    Triangle    trian_temp;
	Vertex      verte_temp;

	//--- Important, triangle offset index must be initialized!!!
	int off_triangle=0;
	//--- Loop over all tetrahedra and find the intersections with the isosurface
	for (vector <Tetrahedron> :: iterator TetraIter=raw_tetrahedra.begin(); TetraIter != raw_tetrahedra.end(); TetraIter++) {

        //--- Get density at the vertices tetrahedra and sort them
        float tetra_den[4] = {scalar[TetraIter->vert[0]],scalar[TetraIter->vert[1]], scalar[TetraIter->vert[2]], scalar[TetraIter->vert[3]] };
        //float tetra_den[4] = {data_vertices[TetraIter->vert[0]].alpha[0],data_vertices[TetraIter->vert[1]].alpha[0], data_vertices[TetraIter->vert[2]].alpha[0],data_vertices[TetraIter->vert[3]].alpha[0] };

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

			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[1]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[1]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[2]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[2]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[1]], tetra_den[vec_ind[2]], TetraIter->vert[vec_ind[1]], TetraIter->vert[vec_ind[2]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[1]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[1]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[2]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[2]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp,scalar);
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
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[1]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[1]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[2]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[2]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[0]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[0]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[1]], tetra_den[vec_ind[2]], TetraIter->vert[vec_ind[1]], TetraIter->vert[vec_ind[2]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[1]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[1]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp,scalar);
			if (flag) iso_vertices.push_back(verte_temp);
			flag = isoden_cut_line(_isoval, tetra_den[vec_ind[2]], tetra_den[vec_ind[3]], TetraIter->vert[vec_ind[2]], TetraIter->vert[vec_ind[3]], data_vertices,verte_temp,scalar);
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


void Isosurface::draw_lines(Color_Table &c_table){
	float x,y,z;

	glBegin(GL_LINES);

	float fac2 = 16.0;

	//--- Go through all isodensity triangles
	vector <Triangle> :: iterator TriIter;
	for (TriIter=iso_triangles.begin(); TriIter != iso_triangles.end(); TriIter++){

		//--- bad fix for boundaries
		if (iso_vertices[TriIter->vert[0]].xyz[0] < 0.05 || iso_vertices[TriIter->vert[0]].xyz[0] > 0.95 ||
			iso_vertices[TriIter->vert[0]].xyz[1] < 0.05 || iso_vertices[TriIter->vert[0]].xyz[1] > 0.95 ||
			iso_vertices[TriIter->vert[0]].xyz[2] < 0.05 || iso_vertices[TriIter->vert[0]].xyz[2] > 0.95 ||
			iso_vertices[TriIter->vert[1]].xyz[0] < 0.05 || iso_vertices[TriIter->vert[1]].xyz[0] > 0.95 ||
			iso_vertices[TriIter->vert[1]].xyz[1] < 0.05 || iso_vertices[TriIter->vert[1]].xyz[1] > 0.95 ||
			iso_vertices[TriIter->vert[1]].xyz[2] < 0.05 || iso_vertices[TriIter->vert[1]].xyz[2] > 0.95 ||
			iso_vertices[TriIter->vert[2]].xyz[0] < 0.05 || iso_vertices[TriIter->vert[2]].xyz[0] > 0.95 ||
			iso_vertices[TriIter->vert[2]].xyz[1] < 0.05 || iso_vertices[TriIter->vert[2]].xyz[1] > 0.95 ||
			iso_vertices[TriIter->vert[2]].xyz[2] < 0.05 || iso_vertices[TriIter->vert[2]].xyz[2] > 0.95 ) continue;

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


void Isosurface::draw_triangles(Color_Table &c_table){
	//--- Vextices position
	float x1,x2,x3,y1,y2,y3,z1,z2,z3;
	//--- Vectors defining a triangle
	float a_vec[3];
	float a_norm = 0;
	float b_vec[3];
	float b_norm =0;

	glBegin(GL_TRIANGLES);

	//--- Go though all isodensity triangles
	for (vector <Triangle> :: iterator TriIter=iso_triangles.begin(); TriIter != iso_triangles.end(); TriIter++){

		//--- bad fix for boundaries
		if (iso_vertices[TriIter->vert[0]].xyz[0] < 0.05 || iso_vertices[TriIter->vert[0]].xyz[0] > 0.95 ||
			iso_vertices[TriIter->vert[0]].xyz[1] < 0.05 || iso_vertices[TriIter->vert[0]].xyz[1] > 0.95 ||
			iso_vertices[TriIter->vert[0]].xyz[2] < 0.05 || iso_vertices[TriIter->vert[0]].xyz[2] > 0.95 ||
			iso_vertices[TriIter->vert[1]].xyz[0] < 0.05 || iso_vertices[TriIter->vert[1]].xyz[0] > 0.95 ||
			iso_vertices[TriIter->vert[1]].xyz[1] < 0.05 || iso_vertices[TriIter->vert[1]].xyz[1] > 0.95 ||
			iso_vertices[TriIter->vert[1]].xyz[2] < 0.05 || iso_vertices[TriIter->vert[1]].xyz[2] > 0.95 ||
			iso_vertices[TriIter->vert[2]].xyz[0] < 0.05 || iso_vertices[TriIter->vert[2]].xyz[0] > 0.95 ||
			iso_vertices[TriIter->vert[2]].xyz[1] < 0.05 || iso_vertices[TriIter->vert[2]].xyz[1] > 0.95 ||
			iso_vertices[TriIter->vert[2]].xyz[2] < 0.05 || iso_vertices[TriIter->vert[2]].xyz[2] > 0.95 ) continue;

		//--- Color and transparency
		glColor4f(c_table.get_R(isoval*255),
				  c_table.get_G(isoval*255),
				  c_table.get_B(isoval*255),
				  isoval);

		//--- Position of vertices
		x1 = iso_vertices[TriIter->vert[0]].xyz[0];
		y1 = iso_vertices[TriIter->vert[0]].xyz[1];
		z1 = iso_vertices[TriIter->vert[0]].xyz[2];
		x2 = iso_vertices[TriIter->vert[1]].xyz[0];
		y2 = iso_vertices[TriIter->vert[1]].xyz[1];
		z2 = iso_vertices[TriIter->vert[1]].xyz[2];
		x3 = iso_vertices[TriIter->vert[2]].xyz[0];
		y3 = iso_vertices[TriIter->vert[2]].xyz[1];
		z3 = iso_vertices[TriIter->vert[2]].xyz[2];

		//--- Define the to vectors of the triangle
		a_vec[0] = x1-x2;
		a_vec[1] = y1-y2;
		a_vec[2] = z1-z2;
		a_norm = sqrt(a_vec[0]*a_vec[0] + a_vec[1]*a_vec[1] + a_vec[2]*a_vec[2]);
		a_vec[0] = a_vec[0]/a_norm;
		a_vec[1] = a_vec[1]/a_norm;
		a_vec[2] = a_vec[2]/a_norm;

		b_vec[0] = x3-x2;
		b_vec[1] = y3-y2;
		b_vec[2] = z3-z2;
		b_norm = sqrt(b_vec[0]*b_vec[0] + b_vec[1]*b_vec[1] + b_vec[2]*b_vec[2]);
		b_vec[0] = b_vec[0]/b_norm;
		b_vec[1] = b_vec[1]/b_norm;
		b_vec[2] = b_vec[2]/b_norm;

		glNormal3f(a_vec[1]*b_vec[2] - a_vec[2]*b_vec[1], a_vec[2]*b_vec[0] - a_vec[0]*b_vec[2],a_vec[0]*b_vec[1] - a_vec[1]*b_vec[0]);

		glVertex3f((x1-0.5)*10,(y1-0.5)*10,(z1-0.5)*10);
		glVertex3f((x2-0.5)*10,(y2-0.5)*10,(z2-0.5)*10);
		glVertex3f((x3-0.5)*10,(y3-0.5)*10,(z3-0.5)*10);
	}

	glEnd();

}
