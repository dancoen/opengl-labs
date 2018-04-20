#include "stdafx.h"

// standard
#include <assert.h>
#include <math.h>
#include <cstdlib>

// glut
#include <GL/glut.h>


//set num particles here
#define MAXPARTICLES 10000


//particle used for explosion and fire
class Particle {
public:
	double x, y, z;				//initial position and velocities
	double xvel, yvel, zvel;
	double gravity;				//apply to explosion but not fire/smoke
	double time_remaining;		//lifetime
	double size;
	
	Particle() {}

	Particle(double xpos, double ypos, double zpos, double xvelocity, double yvelocity, double zvelocity, double time, double grav) {
		x = xpos; y = ypos; z = zpos;
		xvel = xvelocity;
		yvel = yvelocity;
		zvel = zvelocity;
		time_remaining = time;
		gravity = grav;
		size = .03;
	}

	double sign(double i) {	//sign value for reflecting velocity components
		if (i >= 0) {
			return 1.0;
		}
		return -1.0;
	}

	void move() {
		//update position depending on velocity
		x += xvel;
		yvel += gravity;		//gravity - if any
		y += yvel;
		z += zvel;
		glLoadIdentity();
		glTranslatef(x, y, z);
		//glutSolidSphere(radius, 20, 20);	//draw particle
		glutSolidCube(size);
		time_remaining--;
	}
};

Particle *particles;	//particle array



int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

bool start = false;		//check if 's' key has been pressed to start animation


double fRand(double fMin, double fMax)		//random double generator
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

//================================
// init
//================================
void init( void ) {
	// init something before main loop...
}

//================================
// update
//================================
void update( void ) {
	// do something before rendering...

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
	GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

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
	glEnable(GL_COLOR_MATERIAL);
	if (start) {
		//create explosion - the particles are initialized in the keyboard function
		for (int i = 0; i < MAXPARTICLES; i++) {
			if (particles[i].time_remaining > 0) {
				if (particles[i].time_remaining < 40) {
					//if dying soon, turn grey (smoke)
					glColor3f(.75, .75, .75);
				}
				else { glColor3f(1, (particles[i].time_remaining / 120.0) * .5, .2 * (particles[i].time_remaining / 120.0)); }
				//stregth of reds, orange and blue depending on life left; newest is more orange, older is more red.
				particles[i].move();
			}
			else {
				//if dead draw new particle
				particles[i] = particles[i] = Particle(fRand(-.5,.5), fRand(-6,-4), -15, fRand(-.03, .03), fRand(.05, .1), fRand(-.05, .05), rand() % 160, -.0005);
			}
		}
	}
	else {
		//draw "bomb"
		glColor3f(.75, .75, .75);
		glLoadIdentity();
		glTranslatef(0, -5, -15);
		glutSolidSphere(.5, 20, 20);
	}
	

	// disable lighting
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// swap back and front buffers
	glutSwapBuffers();
}

//================================
// keyboard input
//================================
void keyboard( unsigned char key, int x, int y ) {
	switch(key)
		case 's': 
			particles = new Particle[MAXPARTICLES];
			for (int i = 0; i < MAXPARTICLES; i++) {
				//initialize particles for explosion
				particles[i] = Particle(0, -8, -20, fRand(-.2, .2), fRand(.2, .7), fRand(-.1, .1), rand() % 60, -.01);
			}
			start = true;
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
	glutTimerFunc( 32, timer, 0 );
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
	glutTimerFunc( 32, timer, 0 );
	
	// main loop
	glutMainLoop();

	return 0;
}