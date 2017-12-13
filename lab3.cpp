#include "stdafx.h"

// standard
#include <assert.h>
#include <math.h>
#include <stdlib.h>

// glut
#include <GL/glut.h>

//int numBalls = 20; 
int numBalls = 2; //set number of balls here

class ball{
	public:
		double x, y, z; //position
		double xvelocity, yvelocity, zvelocity; //velocities
		double radius;
		ball() {
		}

		ball(double xpos, double ypos, double zpos, double xvel, double yvel, double zvel, double rad) {
			x = xpos;
			y = ypos;
			z = zpos;
			xvelocity = xvel;
			yvelocity = yvel;
			zvelocity = zvel;
			radius = rad;
		}

		double sign(double i) {	//sign value for reflecting velocity components
			if (i >= 0) {
				return 1.0;
			}
			return -1.0;
		}

		void floorCollision(void) {
			if (y <= -4.4) {
				y = -4.4;	
				if (fabs(yvelocity) <= .05) {   //object should rest with such low velocity
					yvelocity = 0.0;			
				}
				else {
					yvelocity = -yvelocity;		//bounce: reflect y velocity and damp
					y += yvelocity - .0001;		//update position with dampening
					yvelocity -= .02;			//add acceleration due to gravity
				}
			}
			else {
				y += yvelocity - .015;			//if no collision just apply gravity with dampening
				yvelocity -= .01;
			}
		}

		void sideWallCollisions(void) {
			if (fabs(xvelocity) <= .0005) {
				xvelocity = 0.0;
				if (x <= -4.5) x = -4.5;
				else if (x >= 4.5) x = 4.5;
				return;
			}
			if (x <= -4.5 || x >= 4.5) {
				if (x <= -4.5) x = -4.5; 
				else if (x >= 4.5) x = 4.5;  
				xvelocity = -xvelocity;									 //reflect velocity
				sign(xvelocity) * (fabs(xvelocity) - .003);
				x += xvelocity;
			}
			else{
				x += xvelocity;											 //update position
				xvelocity = sign(xvelocity) * (fabs(xvelocity) - .0005);	 //damped velocity
			}
		}

		void frontWallCollisions(void) {
			if (fabs(zvelocity) <= .0005) {
				zvelocity = 0.0;
			}
			else if (z <= -4.5 || z >= 4.5) {		//should rest if going so slow
				if (z <= -4.5) z = -4.5; 
				else if (z >= 4.5) z = 4.5; 
				zvelocity = -zvelocity;				//reflect velocity
				sign(z) * (fabs(zvelocity) - .003);
				z += zvelocity;
			}
			else {
				z += zvelocity;											 //update position
				zvelocity = sign(zvelocity) * (fabs(zvelocity) - .0005);	 //damped velocity
			}
		}

		void checkRoomCollisions(void) {		
			floorCollision();
			sideWallCollisions();
			frontWallCollisions();
		}

		bool checkBallCollision(ball otherBall) {	//find distance between balls center and see if greater than their combined radius
			double distSquared = pow(x - otherBall.x, 2.0) + pow(y - otherBall.y, 2.0) + pow(z - otherBall.z, 2.0);
			double radiusSquared = pow(radius + otherBall.radius, 2);
			if (distSquared <= radiusSquared) return true;
			else {
				return false;
			}
		}

		int collide(ball otherBall) {
			//TODO: use geometric coordinates to find angle of the collision, and reflect components of this angle about the normal for both balls.
			if (checkBallCollision(otherBall)) {
				double tempxvel = xvelocity;
				double tempyvel = yvelocity;
				double tempzvel = zvelocity;
				xvelocity = sign(otherBall.xvelocity) * (fabs(otherBall.xvelocity) - .003);
				yvelocity = sign(otherBall.xvelocity) * (fabs(otherBall.xvelocity) - .003);
				zvelocity = sign(otherBall.xvelocity) * (fabs(otherBall.xvelocity) - .003);
				otherBall.xvelocity = sign(tempxvel) * (fabs(tempxvel) - .003);
				otherBall.yvelocity = sign(tempyvel) * (fabs(tempyvel) - .003);
				otherBall.zvelocity = sign(tempzvel) * (fabs(tempzvel) - .003);
				x += xvelocity;
				y += yvelocity;
				z += zvelocity;
				otherBall.x += otherBall.xvelocity;
				otherBall.y += otherBall.yvelocity;
				otherBall.z += otherBall.zvelocity;
				checkRoomCollisions();
				otherBall.checkRoomCollisions();
				return 0;
			}
			return 1;
		}

};

double randVal(int max) {
	max = max * 2;
	return (rand() % max) - max;
}

void drawRoom(double width, double height) {	//draws a square floor and walls except wall facing view
	glBegin(GL_QUADS);

	//floor
	glColor3f(0.2, 0.5, 0.8);
	glVertex3f(width / 2, -height / 2, -width/2);
	glVertex3f(-width / 2, -height / 2, -width/2);
	glVertex3f(-width / 2, -height / 2, width/2);
	glVertex3f(width / 2, -height / 2, width/2);

	/*
	//ceiling
	glColor3f(0.3, 0.5, 0.6);
	glVertex3f(width / 2, height / 2, width / 2);
	glVertex3f(-width / 2, height / 2, width / 2);
	glVertex3f(-width / 2, height / 2, -width / 2);
	glVertex3f(width / 2, height / 2, -width / 2);
	*/

	// Back wall
	glColor3f(0.4, 0.2, 0.2);
	glVertex3f(width / 2, -height / 2, -width / 2);
	glVertex3f(-width / 2, -height / 2, -width / 2);
	glVertex3f(-width / 2, height / 2, -width / 2);
	glVertex3f(width / 2, height / 2, -width / 2);


	// Left wall
	glColor3f(0.1, 0.5, 0.6);
	glVertex3f(-width / 2, height / 2, width / 2);
	glVertex3f(-width / 2, height / 2, -width / 2);
	glVertex3f(-width / 2, -height / 2, -width / 2);
	glVertex3f(-width / 2, -height / 2, width / 2);

	// Right wall
	glColor3f(0.7, 0.2, 0.6);
	glVertex3f(width / 2, height / 2, -width / 2);
	glVertex3f(width / 2, height / 2, width / 2);
	glVertex3f(width / 2, -height / 2, width / 2);
	glVertex3f(width / 2, -height / 2, -width / 2);

	glEnd();  // End of drawing cube
}

//================================
// global variables
//================================
// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

// angle for rotation
int g_angle = 0;

ball *balls;
//================================
// init
//================================
void init( void ) {
	// init something before main loop...
	balls = new ball[numBalls];
	balls[0] = ball(0, 0, 0, -.15, .20, -.20, 1);
	balls[1] = ball(-2, -2, -2, .4, .25, .35, 1); 

	//for more balls...
	/*for (int i = 0; i < numBalls; i++) {
		balls[i] = ball(randVal(4), randVal(4), randVal(4), randVal(1), randVal(1), randVal(1), 1);
	}*/
}

//================================
// update
//================================

void update( void ) {
	//TODO: wall collision, collision with other balls.
	// do something before rendering...
	//collision detection
	//ceilingCollision();
	for (int i = 0; i < numBalls; i++) {
		balls[i].checkRoomCollisions();
		for (int j = i + 1; j < numBalls; j++) {
			if (balls[i].checkBallCollision(balls[j])) {
				balls[i].collide(balls[j]);
			}
		}
	}
}

//================================
// render
//================================
void render( void ) {
	// clear buffer
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth (1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// light source attributes
	GLfloat LightAmbient[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat LightSpecular[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightPosition[] = { 5.0f, 5.0f,15.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// surface material attributes
	GLfloat material_Ka[]	= { 0.11f, 0.06f, 0.11f, 1.0f };
	GLfloat material_Kd[]	= { 0.43f, 0.47f, 0.54f, 1.0f };
	GLfloat material_Ks[]	= { 0.33f, 0.33f, 0.52f, 1.0f };
	GLfloat material_Ke[]	= { 0.1f , 0.0f , 0.1f , 1.0f };
	GLfloat material_Se		= 10;

	glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
	glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);
	

	// modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef (0.0, 0, -20.0);
	//glRotated(g_angle, 0.0, 1.0, 0.0);

	// render objects
	//glutSolidTeapot(1.0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	drawRoom(10,10);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	int i = 0;
	for (i; i < numBalls; i++) {
		//TODO check ball collisions...put this in update
		//glLoadIdentity();
		glLoadIdentity();
		glTranslatef(0.0, 0, -20.0);
		glTranslatef(balls[i].x, balls[i].y, balls[i].z);
		glColor3f(0.2, 0.5, 0.8);
		glutSolidSphere(balls[i].radius, 20, 20);
	}
	//TODO for each ball render
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	// disable lighting

	// swap back and front buffers
	glutSwapBuffers();
}

//================================
// keyboard input
//================================
void keyboard( unsigned char key, int x, int y ) {

}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {
	// screen size
	g_screenWidth  = w;
	g_screenHeight = h;	
	
	// viewport
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );

	// projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {	
	// increase frame index
	g_frameIndex++;

	update();
	
	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 16, timer, 0 );
}

//================================
// main
//================================
int main( int argc, char** argv ) {
	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( 600, 600 ); 
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( argv[0] );

	// init
	init();
	
	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutTimerFunc( 16, timer, 0 );
	
	// main loop
	glutMainLoop();

	return 0;
}