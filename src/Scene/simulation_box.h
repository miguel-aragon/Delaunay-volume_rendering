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
#ifndef SIMULATION_BOX_H
#define SIMULATION_BOX_H

#include "ofMain.h"

class Simulation_box {

 private:
  float x,y,z;
  float scale_2X,scale_2Y,scale_2Z;

  //--- Drawing flags
  int   flag_draw_cubo;
  int   flag_draw_axis;

 public:

  //--- Constructors
  Simulation_box();
  Simulation_box(float _x, float _y, float _z, float _scale);
  Simulation_box(float _x, float _y, float _z, float _scaleX, float _scaleY, float _scaleZ);

  //--- Main drawing function
  void draw(void);

  //--- Set cubo for drawing
  void set_cubo(void);

  //--- Set axis for drawing
  void set_axis(void);

};

#endif
