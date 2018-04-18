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
#ifndef _TEST_APP
#define _TEST_APP

#include <math.h>
#include <string>
#include <vector>

#include "ofMain.h"
#include "Tessellation/tessel.h"
#include "trackball/trackball.h"
#include "simulation_box.h"

#include "ofxShader.h"
#include "ofxVectorMath.h"

class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void keyPressed  (int key);

	//=================
	//--- MY METHODS
	//=================

	//--- GUI interface
	void MyGUI(void);
	//--- OpenGL object Selection
	void gl_select(int x, int y);
	//--- Print selected objects
	void list_hits(GLint hits, GLuint *names);

	//-- Global drawing methods
	void drawScene(void);
	void drawCutPlaneOrtho(void);

	//--- Display list
	GLuint index_list;
	// create 1024 display lists
	GLubyte lists[1024];
	//--- Initialize display list
	void Init_display_list(void);
	//--- Display list counter
	int cont_list;

	//--- OpenGL functions
	void OpenGL_Init(void);
	void Set_3D_world(void);

	//--- TrackBall methods
	void initTrackball();
	void mouseTrackballDown( int x, int y );
	void mouseTrackballUp( int x, int y );
	void mouseTrackballMove( int x, int y );


	//=================
	//--- MY VARIABLES
	//=================

	//--- Scene descriptors
	float scene_z_dis;
	int   mouse_y_old;

	//--- For orthographic projection (bad programming)
	int   mouse_x_old;

	//--- TrackBall variables
	GLfloat gTrackBallRotation[4];
	GLfloat gWorldRotation[4];
	bool	gTrackBall;

	//--- The full tessellation object
	Tessel tesselation;

	//--- Framerate
	int   FrameRate;

	//--- Shader
	ofxShader shader_isosurf;
	ofxShader shader_volume;

	//--- Ortho projection zoom and pan (bad programming)
	float x_off_ortho;
	float y_off_ortho;
	float z_off_ortho;

	int flag_draw_volume_points;
	int flag_draw_volume_lines;
	int flag_draw_volume_triangles;
	int flag_draw_volume_sprites;
	int flag_draw_isosurface;
	int flag_draw_cut_plane;

	//--- Define what we are drawing (perspective or ortho)
	int   scene_context;

	//--- Image dump
	ofImage ImageTemp;
	int     cont_ima;
	std::vector<ofImage> ImageSeq;
	int flag_movie;

};

#endif
