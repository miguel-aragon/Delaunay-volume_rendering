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
#ifndef __trackball_h__
#define __trackball_h__

#ifdef __cplusplus
extern "C" {
#endif

void startTrackball (long x, long y, long originX, long originY, long width, long height);
void rollToTrackball (long x, long y, float rot [4]); // rot is output rotation angle
void addToRotationTrackball (float * dA, float * A);

#ifdef __cplusplus
}
#endif

#endif
