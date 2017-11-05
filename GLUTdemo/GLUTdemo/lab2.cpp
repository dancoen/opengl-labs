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


//decide whether to use quaternion or euler rotation here
bool euler = true;

//enter desired coordinates and rotation here
float points[2][3] = 

//x,y,z, start and end of figure
{{ -8, -8, -20},
 { -4, -4, -20},
  
};

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


float xRotate[16];
float yRotate[16];
float zRotate[16];
//three euler rotation matrices

float time = 0;
int position = 0;
//index of orientation
bool start = false;

float basisfunc(float n) {
	//basis function for interpolation
	if (n <= -2 || n >= 2) return 0;
	if (n <= -1) return (pow(n, 3.0)/6.0 + 2.0 * n + 4.0 / 3.0 + pow(n, 2.0));
	if (n <= 0) return (2.0 / 3.0 - pow(n, 2.0) - pow(n, 3.0) / 2.0);
	if (n <= 1) return (2.0 / 3.0 - pow(n, 2.0) + pow(n, 3.0) / 2.0);
	if (n <= 2) return (4.0 / 3.0 - 2.0 * n + pow(n, 2.0) - pow(n, 3.0) / 6.0);
}

float bsplinefunc(float time, int idx) {	
	//using bsplines for interpolation
	//idx reps x,y,z cartesian of object
	int i = 0;
	float result = 0;
	while (i < 6) {
		result += points[i][idx] * basisfunc(time - (float)i);
		i++;
	}
	return result;
}

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

	/*if (start && time < 2) {
		//first adjust translation matrix 
		transmat[12] = bsplinefunc(time, 0);
		transmat[13] = bsplinefunc(time, 1);
		transmat[14] = bsplinefunc(time, 2);
	}
	*/
	location += walk_speed;
	if (g_angle >= 45 || g_angle <= -45) {
		left_forward = !left_forward;
	}
	if (left_forward) g_angle += -5;
	if (!left_forward) g_angle += 5; 


	time += .16;
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
	glLoadIdentity();
	// render objects
	//glutSolidTeapot(1.0);
	glTranslatef(location, 0.0, -20.0);
	//glMultMatrixf(transmat);

	//glRotated(15, 0.0, 1.0, 0.0);

	//This example is taken from Nehe OpenGL Tutorial Lesson # 5 (@ http://nehe.gamedev.net/) I used this code just to draw the objects
	glBegin(GL_QUADS);               
	// Begin drawing the color cube with 6 quads
	// Define vertices in counter-clockwise (CCW) order with normal pointing out 

	float width = 2.0;
	float height = 4.0;

	//Draw Body

	//
	//
	//
	//
	//

	// Top face (y = 1.0f)

	glVertex3f(width/2, height/2, -width/2);
	glVertex3f(-width/2, height/2, -width/2);
	glVertex3f(-width/2, height/2, width/2);
	glVertex3f(width/2, height/2, width/2);

	// Bottom face (y = -1.0f)
	glVertex3f(width/2, -height/2, width/2);
	glVertex3f(-width/2, -height/2, width/2);
	glVertex3f(-width/2, -height/2, -width/2);
	glVertex3f(width/2, -height/2, -width/2);

	// Front face  (z = 1.0f)

	glVertex3f(width/2, height/2, width/2);
	glVertex3f(-width/2, height/2, width/2);
	glVertex3f(-width/2, -height/2, width/2);
	glVertex3f(width/2, -height/2, width/2);

	// Back face (z = -1.0f)
	glVertex3f(width/2, -height/2, -width/2);
	glVertex3f(-width/2, -height/2, -width/2);
	glVertex3f(-width/2, height/2, -width/2);
	glVertex3f(width/2, height/2, -width/2);


	// Left face (x = -1.0f)

	glVertex3f(-width/2, height/2, width/2);
	glVertex3f(-width/2, height/2, -width/2);
	glVertex3f(-width/2, -height/2, -width/2);
	glVertex3f(-width/2, -height/2, width/2);

	// Right face (x = 1.0f)

	glVertex3f(width/2, height/2, -width/2);
	glVertex3f(width/2, height/2, width/2);
	glVertex3f(width/2, -height/2, width/2);
	glVertex3f(width/2, -height/2, -width/2);
	glEnd();

	glLoadIdentity();
	//glTranslatef(/*width/4*/0, -height, -20.0 - (width/4));

	glTranslatef(location, 0, 0);
	glRotated((-g_angle), 0.0, 0.0, 1.0);
	glTranslatef(0, -height + height / 8, -20.0 + (width / 4));
	//glMultMatrixf(transmat);

	//glRotated(90, 0.0, 1.0, 0.0);
	//Draw left Leg
	//
	//
	//
	//
	//

	// Top face (y = 1.0f)
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

	//Draw right Leg
	//
	//
	//
	//
	//
	glLoadIdentity();
	glTranslatef(location, 0, 0);
	glRotated((g_angle), 0.0, 0.0, 1.0);
	glTranslatef(0, -height + height / 8, -20.0 + (width / 4));
	//glMultMatrixf(transmat);

	//glTranslatef(width/2, -height + height / 4, -20.0 + (width / 4));
	// Top face (y = 1.0f)
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

	//gl

	//glRectd(2,30,4,5);


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
	start = true;
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
	glutTimerFunc(60/*16*/, timer, 0);
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