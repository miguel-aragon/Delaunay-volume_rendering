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

#include "tessel.h"
#include "indexed_comparison.h"


//==============================
//--- Pub: Constructor
//==============================
Tessel::Tessel(void) {

	atten_factor=10.0;
	densi_threshold = 0.2;

}

//==============================
//--- Pub: Destructor
//==============================
Tessel::~Tessel(void) {
	data_tetrahedra.clear();
	data_triangles.clear();
	data_vertices.clear();
}


//==============================
//--- Pub: Initialize objects in Tessel.
//    Call this method just after reading all the data
//==============================
void Tessel::Init(void){

	DelauVolume.BoxSize = BoxSize;

}

//==============================
//--- Pub: Draw Delaunay Volume
//==============================
void Tessel::draw_volume(int draw_option, int table_Id){

	if (draw_option == 0) DelauVolume.draw_points(data_vertices, c_table);
	if (draw_option == 1) DelauVolume.draw_lines(data_vertices, data_triangles, c_table);
	if (draw_option == 2) DelauVolume.draw_volume(data_vertices, data_triangles, c_table);


}

//==============================
//--- Pub: Compute isosurface. Use the raw tetrahedra
//==============================
void Tessel::get_IsoSurface(float  _isoval){
	//--- Create isosurface
	IsoSurfObject.get_isosurface(_isoval, raw_tetrahedra, data_vertices, scalar1.get_scalar());
	//IsoSurfObject.get_isosurface(_isoval, raw_tetrahedra, data_vertices, densities);
}

//==============================
//--- Pub: Draw isosurface
//==============================
void Tessel::draw_IsoSurface(GLuint _index_list_Id, int table_Id){
	IsoSurfObject.index_list_Id = _index_list_Id;
	//IsoSurfObject.draw_lines(c_table);
	IsoSurfObject.draw_triangles(c_table);
}

//==============================
//--- Pub: Compute iso plane. Use the raw tetrahedra
//==============================
void Tessel::get_CutPlane(float  _isoval){
	//--- Create isoplane
	IsoPlaneObject.get_isoPlane(0, _isoval, raw_tetrahedra, data_vertices);
}

//==============================
//--- Pub: Draw iso plane
//==============================
void Tessel::draw_CutPlane(GLuint _index_list_Id, int table_Id){
	IsoPlaneObject.index_list_Id = _index_list_Id;
	IsoPlaneObject.draw_lines(c_table);
	IsoPlaneObject.draw_triangles(c_table);
}

//==============================
//--- Pub: Draw iso plane in orthogonal projection
//==============================
void Tessel::draw_CutPlane_flat(int table_Id, int width, int height){
	IsoPlaneObject.draw_triangles_flat(c_table, width, height);
	IsoPlaneObject.draw_lines_flat(c_table, width, height);
}

//==============================
//--- Pub: Order triangles by density
//==============================
void Tessel::order_triangles_by_density(void){
	float x,y,z;

	std::vector <float> density;
	std::vector <int>   density_ind;

	//--- Go though all isodensity triangles
	int count=0;
	for (vector <Triangle> :: iterator TriIter=data_triangles.begin(); TriIter != data_triangles.end(); TriIter++){

		density.push_back( (data_vertices[TriIter->vert[0]].alpha[0]+data_vertices[TriIter->vert[1]].alpha[0]+data_vertices[TriIter->vert[2]].alpha[0])/3.0 );
		density_ind.push_back(count);

		count++;
	}

}


//======================================================================
//                            IO FUNCTIONS
//======================================================================

//==============================
//--- Pub: Read tetrahedra from file
//==============================
void Tessel::read_tetrahedra(char *fname) {

	FILE *file_in;
	char  buffer[200];
	size_t   dummy;
	int   tempi;
	float tempf;

	//--- Attempt to open file
	sprintf(buffer,"%s",fname);
	if(!(file_in=fopen(buffer,"r"))) {
		printf("    Can't open read tetrahedra file '%s'\n",buffer);
		exit(0);
	}

	//--- Read number of tetrahedra
	dummy = fread(&tempi, sizeof(int), 1, file_in);

	printf("    Reading %d tetrahedra\n", tempi);fflush(stdout);
	for (i=0; i<tempi; i++){

        Tetrahedron tetra_temp;
		dummy = fread(&tetra_temp.vert[0], sizeof(tetra_temp), 1, file_in);

        //--- If the tetrahedra connects periodic boundaries skip
        //if (check_boundary_tetra(tetra_temp) == 1) continue;

		//--- Store the raw tetrahedra
		raw_tetrahedra.push_back(tetra_temp);

		//--- Sort vertices in tetrahedra and add to set
		sort(&tetra_temp.vert[0], &tetra_temp.vert[4]);
		uni_tetrahedra.insert(tetra_temp);

		//--- Check for repeated indexes
		if (tetra_temp.vert[0] == tetra_temp.vert[1] ||
			tetra_temp.vert[1] == tetra_temp.vert[2] ||
			tetra_temp.vert[2] == tetra_temp.vert[3])   printf(">>>>>>>>>>>>>> Repeated vertices !!!\n");

		//--- Add triangles to set
        Triangle     trian_temp;
		trian_temp.vert[0] = tetra_temp.vert[0];
		trian_temp.vert[1] = tetra_temp.vert[1];
		trian_temp.vert[2] = tetra_temp.vert[2];
		uni_triangles.insert(trian_temp);
		//---
		trian_temp.vert[0] = tetra_temp.vert[1];
		trian_temp.vert[1] = tetra_temp.vert[2];
		trian_temp.vert[2] = tetra_temp.vert[3];
		uni_triangles.insert(trian_temp);
		//---
		trian_temp.vert[0] = tetra_temp.vert[0];
		trian_temp.vert[1] = tetra_temp.vert[2];
		trian_temp.vert[2] = tetra_temp.vert[3];
		uni_triangles.insert(trian_temp);
		//---
		trian_temp.vert[0] = tetra_temp.vert[0];
		trian_temp.vert[1] = tetra_temp.vert[1];
		trian_temp.vert[2] = tetra_temp.vert[3];
		uni_triangles.insert(trian_temp);

	}

	cout << "\n\n >>> Unique tetrahedra: " << uni_tetrahedra.size() << endl;
	cout << " >>> Unique tiangles  : " << uni_triangles.size()  << ". Original: " << tempi*4 << "\n "<< endl;


	//--- Move unique triangles to vector instead of set
	set <Triangle, Compare_Triangles> :: iterator TriIter;
	for (TriIter=uni_triangles.begin(); TriIter != uni_triangles.end(); TriIter++){
        Triangle     trian_temp;
		trian_temp.vert[0] = TriIter->vert[0];
		trian_temp.vert[1] = TriIter->vert[1];
		trian_temp.vert[2] = TriIter->vert[2];
		data_triangles.push_back(trian_temp);
	}

	//--- Move unique triangles to vector instead of set
	set <Tetrahedron, Compare_Tetrahedra> :: iterator TetraIter;
	for (TetraIter=uni_tetrahedra.begin(); TetraIter != uni_tetrahedra.end(); TetraIter++){
        Tetrahedron tetra_temp;
		tetra_temp.vert[0] = TetraIter->vert[0];
		tetra_temp.vert[1] = TetraIter->vert[1];
		tetra_temp.vert[2] = TetraIter->vert[2];
        tetra_temp.vert[3] = TetraIter->vert[3];
		data_tetrahedra.push_back(tetra_temp);
	}

	fclose(file_in);
}


//==============================
//--- Pub: Read densities from file
//==============================
void Tessel::read_densities(char *fname) {
	FILE *file_in;
	char  buffer[200];
	int   dummy;
	int   tempi;
	float tempf;

	float minDen,maxDen;
	vector<float>::iterator minDenIter;
	vector<float>::iterator maxDenIter;


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
		densities.push_back(tempf);
	}

    //--- Normalize densities
	minDenIter = min_element(densities.begin(), densities.end());
	minDen     = *minDenIter;
	maxDenIter = max_element(densities.begin(), densities.end());
	maxDen     = *maxDenIter;
	vector<float>::iterator densiIter;
	int i=0;
	for (densiIter=densities.begin(); densiIter!=densities.end(); densiIter++) {
		*densiIter = pow( double((*densiIter-minDen)/(maxDen-minDen)), 0.1);//0.12);
        //--- Read only the right number of vertices (from periodic buffer);
		if (i < data_vertices.size()) data_vertices[i].alpha[0] = *densiIter;
		i++;
	}


	fclose(file_in);
}

//==============================
//--- Pub: Read positions from file
//==============================
void Tessel::read_positions(char *fname) {
	FILE *file_in;
	char  buffer[200];
	int   dummy;
	int   tempi;
	float tempf;

	//--- Attempt to open file
	sprintf(buffer,"%s",fname);
	if(!(file_in=fopen(buffer,"r"))) {
		printf("    Can't open read positions file '%s'\n",buffer);
		exit(0); }

	//--- Read header
	dummy = fread(&BoxSize, sizeof(float), 1, file_in); //--- BoxSize
	dummy = fread(&tempi, sizeof(int)  , 1, file_in); //--- Number of points

    printf("Reading %d points... \n", tempi);fflush(stdout);

	for (int i=0;i<tempi;i++){
        //--- Temporary vertex
        Vertex vert_temp;
        dummy = fread(&vert_temp.xyz[0], sizeof(float), 1, file_in);
		dummy = fread(&vert_temp.xyz[1], sizeof(float), 1, file_in);
		dummy = fread(&vert_temp.xyz[2], sizeof(float), 1, file_in);
		data_vertices.push_back(vert_temp);
	}


	fclose(file_in);

}
