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
#include "testApp.h"
#include "Textures/MSAImage.h"


//--- Point sprites
int  pointSize  = 1;
bool useTexture = true;
bool useSprite  = true;

MSAImage    image;
MSATexture *tex;



//--------------------------------------------------------------
void testApp::setup(){

	flag_draw_volume_points = 1;
	flag_draw_volume_lines = 0;
	flag_draw_volume_triangles = 0;
	flag_draw_volume_sprites = 0;
	flag_draw_isosurface = 0;
	flag_draw_cut_plane = 0;


	//--- Read tessellation data and initialize
	char file_tetra[255];
	char file_vertex[255];
	char file_densi[255];
	char file_ctable[255];

	sprintf(file_tetra,   "../../../data/100Mpc_S_100.TETRA");
	sprintf(file_vertex,  "../../../data/100Mpc_S_100.CGAL");
	sprintf(file_densi,   "../../../data/100Mpc_S_100.den");

	sprintf(file_ctable,  "../../../data/CT_04B.ct");
	//--- Read data from files
	tesselation.read_positions(file_vertex);
	tesselation.read_densities(file_densi);
	tesselation.read_tetrahedra(file_tetra);
	tesselation.c_table.read_color_table(file_ctable);
	tesselation.Init();

	//--- Read scalar arrays
	//tesselation.scalar1.read_file((char *) "/Users/miguel/Work/Codes/OF-0.06/apps/Projects/DelauVolu-2.3/bin/data/100Mpc_X_100.gau");
	tesselation.scalar1.read_file((char *) "../../../data/100Mpc_S_100.01.0.gau");
	//tesselation.scalar1.read_file((char *) "/Users/miguel/Work/Codes/OF-0.06/apps/Projects/DelauVolu-2.3/bin/data/100Mpc_L_100-half.0.01.gau");

	tesselation.get_IsoSurface(0.4);
	tesselation.get_CutPlane(0.5);


	//--- Start shaders
	shader_isosurf.loadShader((char *) "../../../data/Shaders/VBOShader");
	shader_volume.loadShader((char *) "../../../data/Shaders/VolumeShader");


	//--- Load image to be used as texture for point sprites
	ofDisableArbTex();
	image.loadImage((char *) "../../../data/Images/halo_016.png");
	tex = image.getMSATexture();

	//--- Scene zoom
	scene_z_dis = 10.0;

	//--- Window title
	ofSetWindowTitle("DelauVolu rendered 2.1 GUI menu");

	//--- Decent framerate
	ofSetFrameRate(30); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.

	//--- Start trackball object
	initTrackball();

	//--- Start display list
	index_list = glGenLists(256);
	Init_display_list();

	//--- Setup orthographic projection
	x_off_ortho = 0;
	y_off_ortho = 0;
	z_off_ortho = 0;

	//--- Set default to perspective
	scene_context = 0;

	//--- Set no movie at start
	flag_movie = 0;

}



//--------------------------------------------------------------
void testApp::update(){

	if (flag_movie ==1) {
		printf("frame_%5i.png \n", cont_ima); fflush(stdout);
		cont_ima++;
		ImageTemp.grabScreen(0,0,800,600);
		ImageSeq.push_back(ImageTemp);
	}

}


//--------------------------------------------------------------
void testApp::draw(){

	ofEnableAlphaBlending();
	switch (scene_context) {
		case 0:
			//--- Draw
			drawScene();
			//--- User interface
			MyGUI();
			break;
		case 1:
			//--- Ortographic projection
			drawCutPlaneOrtho();
			//--- User interface
			MyGUI();
			break;
	} //--- end switch
	ofDisableAlphaBlending();

}

void testApp::drawScene(void){

	//--- Set perspective view mode and clean buffer
	Set_3D_world();

	//--- Do transformation
	glPushMatrix();

	//--- Transform scene
	glTranslatef( 0.0,0.0,-scene_z_dis);
	glRotatef( gTrackBallRotation[0], gTrackBallRotation[1], gTrackBallRotation[2], gTrackBallRotation[3] );
	glRotatef( gWorldRotation[0], gWorldRotation[1], gWorldRotation[2], gWorldRotation[3] );

	//-----------------------
	//--- Display list
	//-----------------------
	cont_list = 1;

	//=====================
	//--- Volume Points
	//=====================
	if (flag_draw_volume_points == 1) {
		lists[0] = 0;
		glListBase(index_list);
		glCallLists(cont_list, GL_UNSIGNED_BYTE, lists);
	}

	//=====================
	//--- Volume Lines
	//=====================
	if (flag_draw_volume_lines == 1) {
		lists[0] = 1;
		glListBase(index_list);
		glCallLists(cont_list, GL_UNSIGNED_BYTE, lists);
	}

	//=====================
	//--- Volume Triangles
	//=====================
	if (flag_draw_volume_triangles == 1) {
		lists[0]  = 2;
		shader_volume.setShaderActive(true);
		//shader_volume.setUniformVariable1f((char *) "den_threshold", 0.9f);
		glListBase(index_list);
		glCallLists(cont_list, GL_UNSIGNED_BYTE, lists);
		shader_volume.setShaderActive(false);
	}

	//=====================
	//--- Isosurface
	//=====================
	if (flag_draw_isosurface == 1) {
		lists[0] = 3;
		shader_isosurf.setShaderActive(true);
		shader_isosurf.setUniformVariable3f((char *) "myColor", 0.7, 0.1, 0.1);
		glListBase(index_list);
		glCallLists(cont_list, GL_UNSIGNED_BYTE, lists);
		shader_isosurf.setShaderActive(false);
	}

	//=====================
	//--- Cutting plane
	//=====================
	if (flag_draw_cut_plane	== 1) {
		lists[0] = 4;
		glListBase(index_list);
		glCallLists(cont_list, GL_UNSIGNED_BYTE, lists);
	}

	//=====================
	//--- Textured points
	//=====================
	if (flag_draw_volume_sprites == 1) {
		lists[0] = 5;
		glListBase(index_list);
		glCallLists(cont_list, GL_UNSIGNED_BYTE, lists);
	}

	//=====================
	//--- Simulation box
	//=====================
	Simulation_box box(0.0f,0.0f,0.0f,10.0f);
	box.set_cubo();
	box.draw();

	glPopMatrix();

}


void testApp::Init_display_list(void){

	//--- Compile display lists
	//-----------------------
	//   Particles
	//-----------------------
	glNewList(index_list + 0, GL_COMPILE);
	glPointSize(1.0);
	tesselation.draw_volume(0,0);
	glEndList();

	//-----------------------
	//   Particles
	//-----------------------
	glNewList(index_list + 1, GL_COMPILE);
	glPointSize(1.0);
	tesselation.draw_volume(1,0);
	glEndList();

	//-----------------------
	//   Triangles
	//-----------------------
	glNewList(index_list + 2, GL_COMPILE);
	//glDepthMask(false);
	//glEnable(GL_DEPTH_TEST);
	tesselation.draw_volume(2,0);
	//glDisable(GL_DEPTH_TEST);
	//glDepthMask(true);
	glEndList();

	//-----------------------
	//   Isosurface
	//-----------------------
	glNewList(index_list + 3, GL_COMPILE);
	tesselation.draw_IsoSurface(index_list + 3, 0);
	glEndList();

	//-----------------------
	//   Cut Plane
	//-----------------------
	glNewList(index_list + 4, GL_COMPILE);
	tesselation.draw_CutPlane(index_list + 4, 0);
	glEndList();

	//-----------------------
	//   Textured points
	//-----------------------
	glNewList(index_list + 5, GL_COMPILE);
	//--- Change point size with distance from camera
	float att[3] = {0.0f, 0.0f, 1.0f};
	glPointParameterf(GL_POINT_SIZE_MIN, 1.0f);
	glPointParameterf(GL_POINT_SIZE_MAX, 256.0f);
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, att);
	//--- Textured point sprites
	tex->bind();
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glPointSize(256.0);
	tesselation.draw_volume(0,0);
	glDisable(GL_POINT_SPRITE);
	tex->unbind();
	glEndList();

}

//==============================================
//--- Draw cut plane in orthographic projection
//==============================================
void testApp::drawCutPlaneOrtho(void){

	//--- Clear screen and setup orthographic projections
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ofSetupGraphicDefaults();
	ofSetupScreen();

	//--- Do transformation
	glPushMatrix();

	//--- Transform scene (pan and zoom)
	glTranslatef( x_off_ortho, y_off_ortho, z_off_ortho);

	//=========================
	//--- Draw axis cut plane
	//=========================
	tesselation.draw_CutPlane_flat(0, ofGetWidth(),ofGetWidth());

	glPopMatrix();

}


//==============================================
//--- Minimalistic graphic user interface
//==============================================
void testApp::MyGUI(void){

	int GUI_width  = 200;
	int GUI_height = ofGetHeight();

	//-- Setup orthographic projection for graphic user interface
	ofSetupGraphicDefaults();
	ofSetupScreen();

	//--- Draw opaque frame for GUI
	ofSetColor(50,50,50);
	ofFill();// draw "filled shapes"
	ofRect(ofGetWidth()-GUI_width, 0, GUI_width, GUI_height);


	//--- Draw Button 1:
	ofSetColor(255,130,0);
	float button_radius = 10;
	glLoadName(7);
	ofCircle(ofGetWidth()-50,50,button_radius);

	//--- Draw Button 1:
	ofSetColor(255,130,0);
	glLoadName(14);
	ofCircle(ofGetWidth()-50,100,button_radius);

	// use the bitMap type
	// note, this can be slow on some graphics cards
	// because it is using glDrawPixels which varies in
	// speed from system to system.  try using ofTrueTypeFont
	// if this bitMap type slows you down.
	ofSetColor(0xAAAAAA);
	ofDrawBitmapString("Info", ofGetWidth()-70, 80);

	//--- Display framerate
	char tempStr1[255];
	sprintf(tempStr1,  "FrameRate : %lf", ofGetFrameRate());
	ofDrawBitmapString(tempStr1, ofGetWidth()-180, 20);

	//--- Elapsed time
	sprintf(tempStr1,  "Millisecs : %d", ofGetElapsedTimeMillis());
	//ofDrawBitmapString(tempStr1, 20, 40);

	ofSetColor(0x000000);

}

//==============================================
//--- Set perspective view mode and clean buffer
//==============================================
void testApp::Set_3D_world(void){

	//--- Calculate The Aspect Ratio Of The Window
    gluPerspective(35.0f,(GLfloat)ofGetWidth()/(GLfloat)ofGetHeight()/2.0, 0.001f,1000.0f);
    glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
    glLoadIdentity();							    // Reset The Modelview Matrix

	glDisable(GL_NORMALIZE);
	//--- Enable color
	glEnable(GL_COLOR_MATERIAL);
	//--- Change the background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//-- Disable back culling
	glDisable(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);

	//--- Enable alpha blending
	glEnable(GL_BLEND);
	//--- Define blending
	//glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	//glBlendFunc (GL_ONE, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//--- Important for point sprites
	ofDisableArbTex();

}

//==============================================
//--- Handle keyboard
//==============================================
void testApp::keyPressed  (int key){

	switch (key)
    {

			//-------------------------------
			//---  Draw flags
			//-------------------------------
		case 'p':
			if (flag_draw_volume_points == 0) flag_draw_volume_points = 1;
			else                              flag_draw_volume_points = 0;
			draw();
			break;
		case 'o':
			if (flag_draw_volume_lines == 0) flag_draw_volume_lines = 1;
			else                             flag_draw_volume_lines = 0;
			draw();
			break;
		case 'i':
			if (flag_draw_volume_triangles == 0) flag_draw_volume_triangles = 1;
			else                                 flag_draw_volume_triangles = 0;
			draw();
			break;
		case 'u':
			if (flag_draw_volume_sprites == 0) flag_draw_volume_sprites = 1;
			else                               flag_draw_volume_sprites = 0;
			draw();
			break;
		case 'y':
			if (flag_draw_isosurface == 0) flag_draw_isosurface = 1;
			else                           flag_draw_isosurface = 0;
			draw();
			break;
		case 't':
			if (flag_draw_cut_plane == 0) flag_draw_cut_plane = 1;
			else                          flag_draw_cut_plane = 0;
			draw();
			break;
			//-------------------------------
			//---  Brightness
			//-------------------------------
		case 'a':
			for (int i=0;i<cont_list;i++) glDeleteLists(lists[i], 1);
			tesselation.DelauVolume.atten_factor += 1;
			printf("Atten %f\n", tesselation.DelauVolume.atten_factor);
			Init_display_list();
			draw();
			break;
		case 'z':
			for (int i=0;i<cont_list;i++) glDeleteLists(lists[i], 1);
			tesselation.DelauVolume.atten_factor -= 1;
			if (tesselation.DelauVolume.atten_factor < 1) tesselation.atten_factor = 1;
			printf("Atten %f\n", tesselation.DelauVolume.atten_factor);
			Init_display_list();
			draw();
			break;
			//-------------------------------
			//---  Contrast
			//-------------------------------
		case 's':
			for (int i=0;i<cont_list;i++) glDeleteLists(lists[i], 1);
			tesselation.c_table.add_brightnes(0.1);
			Init_display_list();
			draw();
			break;
		case 'x':
			for (int i=0;i<cont_list;i++) glDeleteLists(lists[i], 1);
			tesselation.c_table.add_brightnes(-0.1);
			Init_display_list();
			draw();
			break;
			//-------------------------------
			//---  Cheap alpha
			//-------------------------------
		case 'd':
			for (int i=0;i<cont_list;i++) glDeleteLists(lists[i], 1);
			tesselation.DelauVolume.densi_threshold = tesselation.DelauVolume.densi_threshold + 0.05;
			Init_display_list();
			draw();
			break;
		case 'c':
			for (int i=0;i<cont_list;i++) glDeleteLists(lists[i], 1);
			tesselation.DelauVolume.densi_threshold = tesselation.DelauVolume.densi_threshold - 0.05;
			if (tesselation.DelauVolume.densi_threshold < 0) tesselation.DelauVolume.densi_threshold = 0.0;
			Init_display_list();
			draw();
			break;
			//-------------------------------
			//---  Isodensity values
			//-------------------------------
		case 'f':
			tesselation.get_IsoSurface(tesselation.IsoSurfObject.get_isoval()+0.005);
			glDeleteLists(tesselation.IsoSurfObject.index_list_Id,1);
			glNewList(tesselation.IsoSurfObject.index_list_Id, GL_COMPILE);
			tesselation.draw_IsoSurface(tesselation.IsoSurfObject.index_list_Id, 0);
			glEndList();
			draw();
			break;
		case 'v':
			tesselation.get_IsoSurface(tesselation.IsoSurfObject.get_isoval()-0.005);
			glDeleteLists(tesselation.IsoSurfObject.index_list_Id,1);
			glNewList(tesselation.IsoSurfObject.index_list_Id, GL_COMPILE);
			tesselation.draw_IsoSurface(tesselation.IsoSurfObject.index_list_Id, 0);
			glEndList();
			draw();
			break;
			//-------------------------------
			//---  Cutting plane values
			//-------------------------------
		case 'g':
			tesselation.get_CutPlane(tesselation.IsoPlaneObject.get_isoval()+0.01);
			glDeleteLists(tesselation.IsoPlaneObject.index_list_Id,1);
			glNewList(tesselation.IsoPlaneObject.index_list_Id, GL_COMPILE);
			tesselation.draw_CutPlane(tesselation.IsoPlaneObject.index_list_Id, 0);
			glEndList();
			draw();
			break;
		case 'b':
			tesselation.get_CutPlane(tesselation.IsoPlaneObject.get_isoval()-0.01);
			glDeleteLists(tesselation.IsoPlaneObject.index_list_Id,1);
			glNewList(tesselation.IsoPlaneObject.index_list_Id, GL_COMPILE);
			tesselation.draw_CutPlane(tesselation.IsoPlaneObject.index_list_Id, 0);
			glEndList();
			draw();
			break;
			//-------------------------------
			//---  Perspective - Ortho views
			//-------------------------------
		case '1':
			scene_context = 0;
			break;
		case '2':
			scene_context = 1;
			break;

			//-------------------------------
			//---  Dump Image
			//-------------------------------
		case 'm':
			if (flag_movie == 1) flag_movie = 0;
			else
			{
				//--- Allocate space for image
				ImageTemp.allocate(800, 600, OF_IMAGE_COLOR);
				ImageSeq.reserve(500);
				cont_ima = 0;
				flag_movie = 1;
			}
			break;

		case 'n':
			//--- Stop taking frames
			flag_movie = 0;
			int i=0;
			for (vector <ofImage> :: iterator ImaIter = ImageSeq.begin(); ImaIter != ImageSeq.end(); ImaIter++) {
				char file_image[255];
				sprintf(file_image, "dump_%.5i.png", i);
				printf(file_image); fflush(stdout);
				ImaIter->saveImage(file_image);
				i++;
			    if (i > cont_ima) break;
			}
			ImageSeq.clear();
			break;
	}

}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

	switch (scene_context)
    {
			//--- PERSPECTIVE
		case 0:
			//--- Middle button
			if (button == 1) {
				scene_z_dis += (y-mouse_y_old)/500.0;
			}
			else {
				mouseTrackballMove( x, ofGetHeight()-y);
			}
			break;
			//--- ORTHOGRAPHIC
		case 1:
			//--- Middle button
			if (button == 1) {
				z_off_ortho += (y-mouse_y_old)/10.0;
			}
			else {
				y_off_ortho += (y-mouse_y_old)/10.0;
				x_off_ortho += (x-mouse_x_old)/10.0;
			}
			break;
	}


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){


	switch (scene_context)
    {
			//--- PERSPECTIVE
		case 0:
			//--- Left button
			if (button == 0) {
				//--- y axis is inverted
				mouseTrackballDown( x, ofGetHeight()-y);
			}
			//--- Middle button keep the current y position as reference for zooming
			if (button == 1) {
				mouse_y_old = y;
			}

			if (button == 2) {
				printf("Mouse button %d pressed at %d %d\n", button, x, y);
				gl_select(x,ofGetHeight()-y); //Important: gl (0,0) ist bottom left but window coords (0,0) are top left so we have to change this!
			}

			break;
			//--- ORTHOGRAPHIC
		case 1:
			//--- Left button
			if (button == 0) {
				//--- y axis is inverted
				mouse_x_old = x;
				mouse_y_old = y;
			}
			//--- Middle button keep the current y position as reference for zooming
			if (button == 1) {
				mouse_y_old = y;
			}
			break;

	}//--- end switch
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	if (button == 1) {

	}
	else {
		mouseTrackballUp( x, ofGetHeight()-y );
	}


}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

	//--- y axis is inverted
	mouseTrackballMove( x, ofGetHeight()-y);

}

//========================================
//           TRACKBALL.
//========================================
void testApp::initTrackball ()
{
	gTrackBallRotation[ 0 ] = 0.0f;
	gTrackBallRotation[ 1 ] = 0.0f;
	gTrackBallRotation[ 2 ] = 0.0f;
	gTrackBallRotation[ 3 ] = 0.0f;

	//--- World rotation keeps track of the position of the scene
	gWorldRotation[ 0 ]	= 0.0f;
	gWorldRotation[ 1 ]	= 0.0f;
	gWorldRotation[ 2 ]	= 0.0f;
	gWorldRotation[ 3 ]	= 0.0f;

	gTrackBall = false;
}

//--------------------------------------------------------------
void testApp :: mouseTrackballDown( int x, int y )
{
	startTrackball( x, y, 0, 0, ofGetWidth(), ofGetHeight() );
	gTrackBall = true;
}

//--------------------------------------------------------------
void testApp::mouseTrackballUp( int x, int y )
{
	gTrackBall = false;
	rollToTrackball( x, y, gTrackBallRotation );
	if( gTrackBallRotation[0] != 0.0 ) {
		addToRotationTrackball( gTrackBallRotation, gWorldRotation );
	}
	gTrackBallRotation[ 0 ] = gTrackBallRotation[ 1 ] = gTrackBallRotation[ 2 ] = gTrackBallRotation[ 3 ] = 0.0f;
}


//--------------------------------------------------------------
void testApp::mouseTrackballMove( int x, int y )
{
	if (gTrackBall)
	{
		rollToTrackball( x, y, gTrackBallRotation );
	}
}


//--------------------------------------------------------------
//--- Shameless cut-paste from: http://gpwiki.org/index.php/OpenGL:Tutorials:Picking
//--------------------------------------------------------------
void testApp::gl_select(int x, int y)
{
 	GLuint buff[64] = {0};
 	GLint hits, view[4];
 	int id;

 	/*
	 This choose the buffer where store the values for the selection data
	 */
 	glSelectBuffer(64, buff);

 	/*
	 This retrieves info about the viewport
	 */
 	glGetIntegerv(GL_VIEWPORT, view);

 	/*
	 Switching in selecton mode
	 */
 	glRenderMode(GL_SELECT);

 	/*
	 Clearing the names' stack
	 This stack contains all the info about the objects
	 */
 	glInitNames();

 	/*
	 Now fill the stack with one element (or glLoadName will generate an error)
	 */
 	glPushName(0);

 	/*
	 Now modify the viewing volume, restricting selection area around the cursor
	 */
 	glMatrixMode(GL_PROJECTION);
 	glPushMatrix();
	glLoadIdentity();

	/*
	 restrict the draw to an area around the cursor
	 */
	gluPickMatrix(x, y, 1.0, 1.0, view);
	gluPerspective(60, (float)view[2]/(float)view[3], 0.0001, 1000.0);

	/*
	 Draw the objects onto the screen
	 */
	glMatrixMode(GL_MODELVIEW);

	/*
	 draw only the names in the stack, and fill the array
	 */
	draw();

	/*
	 Do you remeber? We do pushMatrix in PROJECTION mode
	 */
	glMatrixMode(GL_PROJECTION);
 	glPopMatrix();

 	/*
	 get number of objects drawed in that area
	 and return to render mode
	 */
 	hits = glRenderMode(GL_RENDER);

 	/*
	 Print a list of the objects
	 */
 	list_hits(hits, buff);

 	/*
	 uncomment this to show the whole buffer
	 * /
	 gl_selall(hits, buff);
	 */

 	glMatrixMode(GL_MODELVIEW);
}

void testApp::list_hits(GLint hits, GLuint *names)
{
 	int i;

 	/*
	 For each hit in the buffer are allocated 4 bytes:
	 1. Number of hits selected (always one,
	 beacuse when we draw each object
	 we use glLoadName, so we replace the
	 prevous name in the stack)
	 2. Min Z
	 3. Max Z
	 4. Name of the hit (glLoadName)
	 */

 	printf("%d hits:\n", hits);

 	for (i = 0; i < hits; i++)
 		printf(	"Number: %d\n"
			   "Min Z: %d\n"
			   "Max Z: %d\n"
			   "Name on stack: %d\n",
			   (GLubyte)names[i * 4],
			   (GLubyte)names[i * 4 + 1],
			   (GLubyte)names[i * 4 + 2],
			   (GLubyte)names[i * 4 + 3]
			   );

 	printf("\n");
}
