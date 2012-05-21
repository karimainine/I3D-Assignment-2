#include <stdlib.h>

#include "gl.h"
#include "camera.h"
#include "waves.h"
#include "utils.h"
#include "light.h"
#include "boat.h"
#include "keys.h"
#include "controls.h"
#include "texture.h"
#include "seabed.h"

/* Some global variables */
Camera camera;
Grid grid;
Light dayLight;
Light nightLight;
Boat boat;
Keys keys;
Controls controls;
static GLuint texture;

/* Draws a 3d axis at the given position, with the given length */
void drawAxes(Vec3f pos, Vec3f len)
{
	/* We will be changing colour and disabling lighting, save the current state of these so
	   they can be changed back when we are done */
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);

	/* Disable lighting so we can see the axes in all conditions */
	glDisable(GL_LIGHTING);

	/* Draw the axes, red for x, green for y and blue for z */
	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3f(pos.x, 0, 0); glVertex3f(pos.x + len.x, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, pos.y, 0); glVertex3f(0, pos.y + len.y, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, pos.z); glVertex3f(0, 0, pos.z + len.z);

	glEnd();

	glPopAttrib();
}

void display(void)
{
	/* Put the scene into a default rendering state by resetting the modelview projection and clearing the colour and depth buffers */
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (controls.wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (controls.mainCamera)
	{
		gluLookAt(0.0, 20.0, 20.0, boat.pos.x, boat.pos.y, boat.pos.z, 0, 1, 0);
	}
	else
	{
		setupCamera(&camera);
	}

	if (controls.axes)
		drawAxes(cVec3f(0, 0, 0), cVec3f(10, 10, 10));

	if (controls.day)
		setupLight(&dayLight);
	else
		setupLight(&nightLight);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	drawGrid(&grid);
	
	glDisable(GL_BLEND);
	
	drawSeaBed();
	
	drawBoat(&boat);

	if (controls.normals)
		drawNormals(&grid, 1);

	/* Display result (swaps front and back buffers) */
	glutSwapBuffers();
}

void reshape(int x, int y)
{
	float aspect = x / (float)y;

	/* Set the viewport to be rendered to */
	glViewport(0, 0, x, y);

	/* We need to resize the projection, to do this we need to modify the projection matrix */
	glMatrixMode(GL_PROJECTION);

	/* Clear previous projection (important, otherwise next step won't work) */
	glLoadIdentity();

	/* Create new projection */
	gluPerspective(camera.fov, aspect, camera.clipNear, camera.clipFar);

	/* Go back to modifying the modelview matrix */
	glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
	static int t1 = -1;
	int t2 = 0;
	float dt = 0;

	if (t1 == -1)
		t1 = glutGet(GLUT_ELAPSED_TIME);

	t2 = glutGet(GLUT_ELAPSED_TIME);
	dt = (t2 - t1) / 1000.0;
	t1 = t2;

	updateGrid(&grid, dt);
	updateBoat(&boat, keys.up, keys.down, keys.left, keys.right, dt);

	glutPostRedisplay();
}

void mouseMove(int x, int y)
{
	static int prevX, prevY;

	/* How much movement has occurred since last frame */
	int movX = x - prevX;
	int movY = y - prevY;

	if (camera.rotating)
	{
		/* Rotate the camera */
		camera.rot.x += movY * camera.sensitivity;
		camera.rot.y += movX * camera.sensitivity;
	}

	if (camera.zooming)
	{
		/* Zoom the camera in/out (speed is proportional to current zoom) */
		camera.zoom -= movY * camera.zoom * camera.sensitivity * 0.05;

		/* Clamp so camera's zoom won't go too far out of the scene */
		camera.zoom = clamp(camera.zoom, camera.clipNear, camera.clipFar / 2.0);
	}

	prevX = x;
	prevY = y;
}

void mouseDown(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		camera.rotating = state == GLUT_DOWN;
	if (button == GLUT_RIGHT_BUTTON)
		camera.zooming = state == GLUT_DOWN;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
		case 'q':
			/* Exit if esc or q is pressed */
			exit(0);

		case 'w':
			controls.wireframe = !controls.wireframe;
			break;

		case 'n':
			controls.normals = !controls.normals;
			break;

		case 'a':
			controls.axes = !controls.axes;
			break;

		case 'l':
			controls.day = !controls.day;
			break;

		case 'e':
			controls.mainCamera = !controls.mainCamera;
			break;

		case '-':
		case '_':
			cleanupGrid(&grid);
			initGrid(&grid, grid.rows / 2, grid.cols / 2, grid.size);
			break;

		case '+':
		case '=':
			cleanupGrid(&grid);
			initGrid(&grid, grid.rows * 2, grid.cols * 2, grid.size);
			break;

		default:
			break;
	}
}

void updateKey(int key, bool state)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			keys.up = state;
			break;

		case GLUT_KEY_DOWN:
			keys.down = state;
			break;

		case GLUT_KEY_LEFT:
			keys.left = state;
			break;

		case GLUT_KEY_RIGHT:
			keys.right = state;
			break;

		default:
			break;
	}
}

void keyboardSpecialDown(int key, int x, int y)
{
	updateKey(key, true);
}

void keyboardSpecialUp(int key, int x, int y)
{
	updateKey(key, false);
}

void init(void)
{
	/* Setup the camera in a default position */
	initCamera(&camera);

	/* Setup key controls */
	initKeys(&keys);
	initControls(&controls);

	/* Setup the grid */
	initGrid(&grid, 200, 200, 200);

	/* Setup the lights */
	initLight(&dayLight, cVec4f(1.2, 1, -1.5, 0), cVec4f(0.4, 0.3, 0.2, 1), cVec4f(0.5, 0.5, 0.5, 1), cVec4f(1, 1, 1, 0), 128);
	initLight(&nightLight, cVec4f(1, 1, -2, 0), cVec4f(0, 0, 0.2, 1), cVec4f(0, 0, 0.2, 1), cVec4f(1, 1, 1, 0), 128);

	/* Load the boat */
	initBoat(&boat, "galleon.obj");

	/* Set appropriate defaults */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0);

	/* Enable lighting and lights */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	
	/* load Textures */
	texture = texture_load("waterTexture.jpg");

	reshape(640, 480);
}

int main(int argc, char **argv)
{
	/* Init glut, create window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("I3D Ass");

	/* Set glut callbacks */
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseDown);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecialDown);
	glutSpecialUpFunc(keyboardSpecialUp);

	/* Init variables, create scene etc */
	init();

	/* Start the main loop (this never returns!) */
	glutMainLoop();

	return EXIT_SUCCESS;
}
