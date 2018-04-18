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
#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "ofMain.h"

class Tetrahedron {
public:
	GLint vert[4];

	Tetrahedron(void);
	Tetrahedron(int _v1,int _v2,int _v3, int _v4);
};




//--- Compare two Tetrahedron objects based on their indexes.
//    The indexes must be ordered for this class to work
class Compare_Tetrahedra{
public:
	bool operator()(Tetrahedron tetra1 , Tetrahedron tetra2){

		//-- Top indexes
		if      (tetra1.vert[0] > tetra2.vert[0]) {
			return true;
		}
		else if (tetra1.vert[0] == tetra2.vert[0]) {
			if      (tetra1.vert[1] > tetra2.vert[1]) {
				return true;
			}
			else if (tetra1.vert[1] == tetra2.vert[1]){
				if  (tetra1.vert[2] >  tetra2.vert[2]) {
					return true;
				}
				else if (tetra1.vert[2] == tetra2.vert[2]){
					if (tetra1.vert[3] >  tetra2.vert[3]) {
						return true;
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	} //--- operator
};

#endif
