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

#include "simulation_box.h"


Simulation_box::Simulation_box() {
  x        = 0;
  y        = 0;
  z        = 0;
  scale_2X = 0.5f;
  scale_2Y = 0.5f;
  scale_2Z = 0.5f;
  //--- Drawing default modes
  flag_draw_cubo = 0;
  flag_draw_axis = 0;
}

Simulation_box::Simulation_box(float _x, float _y, float _z, float _scale) {
  x        = _x;
  y        = _y;
  z        = _z;
  scale_2X = _scale/2.0f;
  scale_2Y = _scale/2.0f;
  scale_2Z = _scale/2.0f;
  //--- Drawing default modes
  flag_draw_cubo = 0;
  flag_draw_axis = 0;
}

Simulation_box::Simulation_box(float _x, float _y, float _z, float _scaleX, float _scaleY, float _scaleZ) {
  x        = _x;
  y        = _y;
  z        = _z;
  scale_2X = _scaleX/2.0f;
  scale_2Y = _scaleY/2.0f;
  scale_2Z = _scaleZ/2.0f;
  //--- Drawing default modes
  flag_draw_cubo = 0;
  flag_draw_axis = 0;
}

void Simulation_box::set_axis(void){
  if (flag_draw_axis == 1)
    flag_draw_axis = 0;
  else
    flag_draw_axis = 1;
}

void Simulation_box::set_cubo(void){
  if (flag_draw_cubo == 1)
    flag_draw_cubo = 0;
  else
    flag_draw_cubo = 1;
}

void Simulation_box::draw(void) {

  if (flag_draw_cubo == 1) {

    glColor4f(0.5f,0.5f,1.0f,1.0f);
    glBegin(GL_LINES);
    //-- X
    glVertex3f(x-scale_2X,y+scale_2Y,z+scale_2Z);
    glVertex3f(x+scale_2X,y+scale_2Y,z+scale_2Z);
    //--
    glVertex3f(x-scale_2X,y-scale_2Y,z+scale_2Z);
    glVertex3f(x+scale_2X,y-scale_2Y,z+scale_2Z);
    //--
    glVertex3f(x-scale_2X,y+scale_2Y,z-scale_2Z);
    glVertex3f(x+scale_2X,y+scale_2Y,z-scale_2Z);
    //--
    glVertex3f(x-scale_2X,y-scale_2Y,z-scale_2Z);
    glVertex3f(x+scale_2X,y-scale_2Y,z-scale_2Z);
    //-- Y
    glVertex3f(x+scale_2X,y-scale_2Y,z+scale_2Z);
    glVertex3f(x+scale_2X,y+scale_2Y,z+scale_2Z);
    //--
    glVertex3f(x-scale_2X,y-scale_2Y,z+scale_2Z);
    glVertex3f(x-scale_2X,y+scale_2Y,z+scale_2Z);
    //--
    glVertex3f(x+scale_2X,y-scale_2Y,z-scale_2Z);
    glVertex3f(x+scale_2X,y+scale_2Y,z-scale_2Z);
    //--
    glVertex3f(x-scale_2X,y-scale_2Y,z-scale_2Z);
    glVertex3f(x-scale_2X,y+scale_2Y,z-scale_2Z);
    //-- Z
    glVertex3f(x+scale_2X,y+scale_2Y,z-scale_2Z);
    glVertex3f(x+scale_2X,y+scale_2Y,z+scale_2Z);
    //--
    glVertex3f(x-scale_2X,y+scale_2Y,z-scale_2Z);
    glVertex3f(x-scale_2X,y+scale_2Y,z+scale_2Z);
    //--
    glVertex3f(x+scale_2X,y-scale_2Y,z-scale_2Z);
    glVertex3f(x+scale_2X,y-scale_2Y,z+scale_2Z);
    //--
    glVertex3f(x-scale_2X,y-scale_2Y,z-scale_2Z);
    glVertex3f(x-scale_2X,y-scale_2Y,z+scale_2Z);
    glEnd( );

  }

  if (flag_draw_axis == 1) {
    glBegin(GL_LINES);

    //-- X
    glColor4f(1.0f,0.5f,0.5f,1.0f);
    glVertex3f(-scale_2X,y,z);
    glVertex3f(+scale_2X,y,z);

    //-- Y
    glColor4f(0.5f,1.0f,0.5f,1.0f);
    glVertex3f(x,-scale_2Y,z);
    glVertex3f(x,+scale_2Y,z);

    //-- Z
    glColor4f(0.5f,0.5f,1.0f,1.0f);
    glVertex3f(x,y,-scale_2Z);
    glVertex3f(x,y,+scale_2Z);

    glEnd( );
  }

}
