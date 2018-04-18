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
#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "ofMain.h"

class Line {
public:
	GLint vert[2];
};

//--- Compare two Tetrahedron objects based on their indexes.
//    The indexes must be ordered for this class to work
class Compare_Lines{
public:
	bool operator()(Line line1 , Line line2){

		//-- Top indexes
		if      (line1.vert[0] > line2.vert[0]) {
			return true;
		}
		else if (line1.vert[0] == line2.vert[0]) {
			if      (line1.vert[1] > line2.vert[1]) {
				return true;
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


#endif // LINE_H_INCLUDED
