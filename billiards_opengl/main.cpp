#include "billiards.h"

/* display list */
GLuint list_number;

/* instence */
ObjectControl object;
Timer space;

/* function prototype */
void initialize(void);
void idle(void);
void display(void);
void resize(int w, int h);
//input
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialFunc(int key, int x, int y);
void specialUpFunc(int key, int x, int y);

/* main function */
GLint main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));									//random generation
	glutInit(&argc, argv);											//initialization environment
	glutInitWindowPosition(_WINDOW_POSITION_X, _WINDOW_POSITION_Y);	
	glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);				
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);		
	glutCreateWindow(_WINDOW_TITLE);								
	glutDisplayFunc(display);										//call draw
	glutReshapeFunc(resize);										//call resize
	glutKeyboardFunc(keyboard);										
	glutKeyboardUpFunc(keyboardUp);									
	glutSpecialFunc(specialFunc);									
	glutSpecialUpFunc(specialUpFunc);								
	glutIgnoreKeyRepeat(GL_TRUE);									//not key repeat

	glutIdleFunc(idle);												
	initialize();													
	glutMainLoop();													//event standby state
	return 0;
}

/* idle funciton */
void idle() {
	glutPostRedisplay(); //glutDisplayFunc run once
}

/* draw(display) function */
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//all object draw
	object.draw();

	glPopMatrix();

	glutSwapBuffers(); //double buffering
}

/* resize function */
void resize(int w, int h) {
	glViewport(0, 0, w, h);

	/* projection transformation matrix */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)_WINDOW_WIDTH / (double)_WINDOW_HEIGHT, 0.1, 1000.0);	//visual volume

	/* modelview transformation matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* initialize function */
void initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);	//back ground color
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glDisable(GL_CULL_FACE);

	/* lighting */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, _LIGHT_POSITION_0);

	/* display list */
	list_number = glGenLists(1);
	glNewList(list_number, GL_COMPILE);	//compile only
	glEndList();

	/* create texture */
	object.texture_init();
}

/** input(keyboard) function **/
/* normal key down */
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case _KEY_SPACE:
		if (object.ball_all_stop()) {
			object.key_space = true;
			space.restart();
		}
		break;
	//exit behavior
	case _KEY_ESC:
		exit(0);
		break;
	default:
		break;
	}
	//refresh game
	if (key == 'r') {
		ObjectControl new_object;
		object = new_object;
		object.texture_init();
	}
}
/* normal key up */
void keyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case _KEY_SPACE:
		if (object.key_space) {
			object.key_space = false;
			object.push_for_space(space.elapsed());
		}
		break;
	}
}
/* special key down */
void specialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		object.key_left = true;
		break;
	case GLUT_KEY_RIGHT:
		object.key_right = true;
		break;
	}
}
/* special key up */
void specialUpFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		object.key_left = false;
		break;
	case GLUT_KEY_RIGHT:
		object.key_right = false;
		break;
	}
}


