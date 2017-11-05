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
float points[6][3] = 

//x,y,z
{{ -8, -8, -15},
 { -4, -4, -20},
 { -0, -0, -15},
 { 4, 4, -10},
 { 8, 8, -20}, 
 { 2, 2, -15},
};

 GLfloat rotatemat[16] =
	 //for quarternion rotations
{ 
  //in column major 	
  1,0,0,0,
  0,1,0,0 ,
  0,0,1,0 ,
  0,0,0,1
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


float eOrient[5][3] = 
//enter euler rotation values (x,y,z)


{{ 5,4,2 },
 { 3,4,9 },
 { 2,3,5 },
 { 2,4,3 },
 { 9,3,2 },
};

float xRotate[16];
float yRotate[16];
float zRotate[16];
//three euler rotation matrices


float qOrient[6][4] =
// (qw,qx,qy,qz)
// These are nomalized upon startup of program
{{ 1,1,1,1 },
 { 3,3,8,9 },
 { 2,9,11,31 },
 { 7,18,13,2 },
 { 13,5,2,8 },
 { 7,1,2, 3},
};


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

void normalize() {	
	//normalize the quaternions entered as input
	for (int i = 0; i < 6; i++) {
		float n = sqrt(pow(qOrient[i][0], 2) + pow(qOrient[i][1], 2) + pow(qOrient[i][2], 2) + pow(qOrient[i][3], 2));
		qOrient[i][0] = qOrient[i][0] / n;
		qOrient[i][1] = qOrient[i][1] / n;
		qOrient[i][2] = qOrient[i][2] / n;
		qOrient[i][3] = qOrient[i][3] / n;
	}
}

//================================
// global variables
//================================
// screen size
int g_screenWidth = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

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

	if (start && time < 5){
		//first adjust translation matrix 
		transmat[12] = bsplinefunc(time, 0);
		transmat[13] = bsplinefunc(time, 1);
		transmat[14] = bsplinefunc(time, 2);

		if (euler == true) {
			if (int(time) > position) {
				position++; 

				//get the desire euler rotations
				float a = eOrient[position][0];
				float b = eOrient[position][1];
				float c = eOrient[position][2];


				//create matrices
				GLfloat tempxRotate[16] = {
					1,0,0,0,
					0,cos(a),-sin(a),0,
					0,sin(a),cos(a),0,
					0,0,0,1,
				};

				GLfloat tempyRotate[16] = {
					cos(b),0,sin(b),0,
					0,1,0,0,
					-sin(b),0,cos(b),0,
					0,0,0,1,
				};

				GLfloat tempzRotate[16] = {
					cos(c),-sin(c),0,0,
					sin(c),cos(c),0,0,
					0,0,1,0,
					0,0,0,1,
				};

				//set x y z matrices 
				for (int j = 0; j < 16; j++) {
					xRotate[j] = tempxRotate[j];
				}
				for (int j = 0; j < 16; j++) {
					yRotate[j] = tempyRotate[j];
				}
				for (int j = 0; j < 16; j++) {
					zRotate[j] = tempzRotate[j];
				}
			}
		}
		if(euler == false) {
			//divide by time frame then normalize? 
			//return;
			if (int(time) > position) {
				position++;

				//get the 4 terms of the quaternion -- w,x,y,z
				float w = qOrient[position][0];
				float x = qOrient[position][1];
				float y = qOrient[position][2];
				float z = qOrient[position][3];

				GLfloat mat[12] =
					//create quarternion matrix
				{
					1.0 - 2.0 * pow(y,2.0) - 2.0 * pow(z,2.0),   2.0 * x*y + 2.0 * z*w,   2.0 * x*z - 2.0 * y*w,    0.0,
					2.0 * x*y - 2.0 * z*w,    1.0 - 2.0 * pow(x,2.0) - 2.0 * pow(z,2.0),   2.0 * y*z + 2.0 * x*w,     0.0,
					2.0 * x*z + 2.0 * y*w,    2.0 * y*z - 2.0 * x*w,    1.0 - 2.0 * pow(x,2.0) - 2.0 * pow(y,2.0),      0.0,

				};

				//set rotate mat
				for (int j = 0; j < 12; j++) {
					rotatemat[j] = mat[j];
				}
			}
		}
		time += .16;
	}
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
	if (start) {
		glMultMatrixf(transmat);	//interpolate translation

		if (!euler)  glMultMatrixf(rotatemat);	//apply quaternion rotations

		if (euler) {
			//apply 3 euler rotations
			glMultMatrixf(xRotate);
			glMultMatrixf(yRotate);
			glMultMatrixf(zRotate);
		}
	}

	// render objects
	glutSolidTeapot(1.0);


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
	normalize();
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