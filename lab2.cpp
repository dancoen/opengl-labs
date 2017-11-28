//I am using the updated freeglut api because glut is no longer maintained

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "GL\glew.h"
#include "GL\freeglut.h"
//#include glut.h

#include <assert.h>
#include <math.h>

//todo: keyboard functions not working and use pointers for matrices

#define PI 3.14159265


 GLfloat transmat[16] =
 {
	 //translation matrix

	 //in column major
	 1,0,0,0,
	 0,1,0,0 ,
	 0,0,1,0 ,
	 0,0,0,1
 };


 int speed = 20; //speed of animation
 double walk_speed = .05;
 double location = 0.0;
 int step_count = 0; //(frame_index % speed == 0?) when this == speed, 
					 // switch leg movement (left moving forward->right moving forward)




float time = 0;
int position = 0;
//index of orientation
//================================
// global variables
//================================
// screen size
int g_screenWidth = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;
int g_angle = 0;
bool left_forward = false;

//================================
// init
//================================
void init(void) {
	// init something before main loop...
}

//================================
// update
//================================
void update(void) {
	// do something before rendering...
	location += walk_speed;
	if (g_angle >= 45 || g_angle <= -45) {
		left_forward = !left_forward;
	}
	if (left_forward) g_angle += -5;
	if (!left_forward) g_angle += 5; 


	time += .16;
}

void drawBody(double width, double height) {
	glBegin(GL_QUADS);
	glVertex3f(width / 2, height / 2, -width / 2);
	glVertex3f(-width / 2, height / 2, -width / 2);
	glVertex3f(-width / 2, height / 2, width / 2);
	glVertex3f(width / 2, height / 2, width / 2);

	// Bottom face (y = -1.0f)
	glVertex3f(width / 2, -height / 2, width / 2);
	glVertex3f(-width / 2, -height / 2, width / 2);
	glVertex3f(-width / 2, -height / 2, -width / 2);
	glVertex3f(width / 2, -height / 2, -width / 2);

	// Front face  (z = 1.0f)

	glVertex3f(width / 2, height / 2, width / 2);
	glVertex3f(-width / 2, height / 2, width / 2);
	glVertex3f(-width / 2, -height / 2, width / 2);
	glVertex3f(width / 2, -height / 2, width / 2);

	// Back face (z = -1.0f)
	glVertex3f(width / 2, -height / 2, -width / 2);
	glVertex3f(-width / 2, -height / 2, -width / 2);
	glVertex3f(-width / 2, height / 2, -width / 2);
	glVertex3f(width / 2, height / 2, -width / 2);


	// Left face (x = -1.0f)

	glVertex3f(-width / 2, height / 2, width / 2);
	glVertex3f(-width / 2, height / 2, -width / 2);
	glVertex3f(-width / 2, -height / 2, -width / 2);
	glVertex3f(-width / 2, -height / 2, width / 2);

	// Right face (x = 1.0f)

	glVertex3f(width / 2, height / 2, -width / 2);
	glVertex3f(width / 2, height / 2, width / 2);
	glVertex3f(width / 2, -height / 2, width / 2);
	glVertex3f(width / 2, -height / 2, -width / 2);
	glEnd();
}

void drawRightLeg(double width, double height) {
	glBegin(GL_QUADS);
	glVertex3f(width / 2, height / 2, -width / 4);
	glVertex3f(-width / 2, height / 2, -width / 4);
	glVertex3f(-width / 2, height / 2, width / 4);
	glVertex3f(width / 2, height / 2, width / 4);

	// Bottom face (y = -1.0f)
	glVertex3f(width / 2, -height / 2, width / 4);
	glVertex3f(-width / 2, -height / 2, width / 4);
	glVertex3f(-width / 2, -height / 2, -width / 4);
	glVertex3f(width / 2, -height / 2, -width / 4);

	// Front face  (z = 1.0f)

	glVertex3f(width / 2, height / 2, width / 4);
	glVertex3f(-width / 2, height / 2, width / 4);
	glVertex3f(-width / 2, -height / 2, width / 4);
	glVertex3f(width / 2, -height / 2, width / 4);

	// Back face (z = -1.0f)
	glVertex3f(width / 2, -height / 2, -width / 4);
	glVertex3f(-width / 2, -height / 2, -width / 4);
	glVertex3f(-width / 2, height / 2, -width / 4);
	glVertex3f(width / 2, height / 2, -width / 4);


	// Left face (x = -1.0f)

	glVertex3f(-width / 2, height / 2, width / 4);
	glVertex3f(-width / 2, height / 2, -width / 4);
	glVertex3f(-width / 2, -height / 2, -width / 4);
	glVertex3f(-width / 2, -height / 2, width / 4);

	// Right face (x = 1.0f)

	glVertex3f(width / 2, height / 2, -width / 4);
	glVertex3f(width / 2, height / 2, width / 4);
	glVertex3f(width / 2, -height / 2, width / 4);
	glVertex3f(width / 2, -height / 2, -width / 4);

	glEnd();  // End of drawing cube

}

void drawLeftLeg(double width, double height) {
	glBegin(GL_QUADS);
	glVertex3f(width / 2, height / 2, -width / 4);
	glVertex3f(-width / 2, height / 2, -width / 4);
	glVertex3f(-width / 2, height / 2, width / 4);
	glVertex3f(width / 2, height / 2, width / 4);

	// Bottom face (y = -1.0f)
	glVertex3f(width / 2, -height / 2, width / 4);
	glVertex3f(-width / 2, -height / 2, width / 4);
	glVertex3f(-width / 2, -height / 2, -width / 4);
	glVertex3f(width / 2, -height / 2, -width / 4);

	// Front face  (z = 1.0f)

	glVertex3f(width / 2, height / 2, width / 4);
	glVertex3f(-width / 2, height / 2, width / 4);
	glVertex3f(-width / 2, -height / 2, width / 4);
	glVertex3f(width / 2, -height / 2, width / 4);

	// Back face (z = -1.0f)
	glVertex3f(width / 2, -height / 2, -width / 4);
	glVertex3f(-width / 2, -height / 2, -width / 4);
	glVertex3f(-width / 2, height / 2, -width / 4);
	glVertex3f(width / 2, height / 2, -width / 4);


	// Left face (x = -1.0f)

	glVertex3f(-width / 2, height / 2, width / 4);
	glVertex3f(-width / 2, height / 2, -width / 4);
	glVertex3f(-width / 2, -height / 2, -width / 4);
	glVertex3f(-width / 2, -height / 2, width / 4);

	// Right face (x = 1.0f)

	glVertex3f(width / 2, height / 2, -width / 4);
	glVertex3f(width / 2, height / 2, width / 4);
	glVertex3f(width / 2, -height / 2, width / 4);
	glVertex3f(width / 2, -height / 2, -width / 4);

	glEnd();  // End of drawing cube
}

void walk(double width, double height, double location) { //width and height of body for man to be drawn, and location where he should be

	//translate to location and draw body

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	transmat[12] = location;
	transmat[13] = 0;
	transmat[14] = -20.0;
	glMultMatrixf(transmat);
	drawBody(width, height);	

	//translate and rotate modelview, draw leftleg

	glLoadIdentity();
	transmat[12] = location;
	transmat[14] = 0;
	glMultMatrixf(transmat);	//translate to position
	GLfloat leftrotatemat[16] = //rotate legs about 
	{ cos(g_angle  * (PI/180)),-sin(g_angle  * (PI / 180)),0,0,	
		sin(g_angle  * (PI / 180)),cos(g_angle  * (PI / 180)),0,0,
		0,0,1,0,
		0,0,0,1, };
	glMultMatrixf(leftrotatemat);	//rotate the leg then translate to proper location on body
	transmat[12] = 0;
	transmat[13] = -height + (height / 8.0);
	transmat[14] = -20.0 + (width / 4.0);
	glMultMatrixf(transmat);
	drawLeftLeg(width, height);

	//translate and rotate modelview, draw rightleg

	glLoadIdentity();
	transmat[12] = location;
	transmat[13] = 0;
	transmat[14] = 0;
	glMultMatrixf(transmat);
	GLfloat rightrotatemat[16] =
	{ cos(-g_angle  * (PI / 180)),-sin(-g_angle  * (PI / 180)),0,0,
		sin(-g_angle  * (PI / 180)),cos(-g_angle  * (PI / 180)),0,0,
		0,0,1,0,
		0,0,0,1, };
	glMultMatrixf(rightrotatemat);
	transmat[12] = 0;
	transmat[13] = -height + (height / 8);
	transmat[14] = -20.0 + (width / 4);
	glMultMatrixf(transmat);
	drawRightLeg(width, height);
}

//================================
// render
//================================
void render(void) {
	// clear buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// light source attributes
	GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat LightSpecular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// surface material attributes
	GLfloat material_Ka[] = { 0.11f, 0.06f, 0.11f, 1.0f };
	GLfloat material_Kd[] = { 0.43f, 0.47f, 0.54f, 1.0f };
	GLfloat material_Ks[] = { 0.33f, 0.33f, 0.52f, 1.0f };
	GLfloat material_Ke[] = { 0.1f , 0.0f , 0.1f , 1.0f };
	GLfloat material_Se = 10;

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);
	glMaterialf(GL_FRONT, GL_SHININESS, material_Se);

	//adjust modelview
	glMatrixMode(GL_MODELVIEW);

	float width = 2.0;			//set desired height and width of the man's body
	float height = 4.0;
	walk(width, height, location);
	// disable lighting
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// swap back and front buffers
	glutSwapBuffers();
}

//================================
// keyboard input
//================================
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		//case 'e': euler = true;
		//case 'q': euler = false;
	}
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape(int w, int h) {
	// screen size
	g_screenWidth = w;
	g_screenHeight = h;

	// viewport
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 2000.0);
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer(int value) {
	// increase frame index
	g_frameIndex++;


	update();

	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc(32, timer, 0);
}

//================================
// main
//================================
int main(int argc, char** argv) {
	// create opengL window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 1200);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	// init
	init();

	// set callback functions
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutTimerFunc(16, timer, 0);

	// main loop
	glutMainLoop();

	return 0;
}
