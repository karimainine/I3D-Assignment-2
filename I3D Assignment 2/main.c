#include <stdlib.h>

#include "gl.h"
#include "camera.h"
#include "waves.h"
#include "utils.h"
#include "light.h"
#include "boat.h"
#include "keys.h"
#include "controls.h"
#include "seabed.h"
#include "screen.h"
#include "texture.h"

/* Some global variables */
Camera camera;
Grid grid;
Light dayLight;
Light nightLight;
Boat boat1;
Boat boat2;
Keys keys;
Controls controls;
Screen screen;
Terrain terrain;
static GLuint waterTexture;
static GLuint terrainTexture;

void drawScene(){
	float diffuse1 [] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float ambient1 [] = { 1.0f, 0.0f, 0.0f, 1.0f };
	
	float diffuse2 [] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float ambient2 [] = { 0.0f, 1.0f, 0.0f, 1.0f };
	
	if (controls.wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if (controls.axes)
		drawAxes(cVec3f(0, 0, 0), cVec3f(10, 10, 10));
	
	if (controls.day)
		setupLight(&dayLight);
	else
		setupLight(&nightLight);
	
	glBindTexture(GL_TEXTURE_2D, terrainTexture);
	drawTerrain(&terrain);
	
	drawBoat(&boat1, diffuse1, ambient1);
	drawBoat(&boat2, diffuse2, ambient2);
	
	glBindTexture(GL_TEXTURE_2D, waterTexture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawGrid(&grid);
	glDisable(GL_BLEND);
	
	if (controls.normals)
		drawNormals(&grid, 1);
}

void drawLeftScreen(){
	/* Set the viewport to be rendered to */
	glViewport(0, 0, screen.x/2, screen.y);
	/* Clear previous projection (important, otherwise next step won't work) */
	glLoadIdentity();
	if (controls.mainCamera)
	{
		gluLookAt(boat1.pos.x, boat1.pos.y + 5.0, boat1.pos.z, boat2.pos.x, boat2.pos.y, boat2.pos.z, 0, 1, 0);
	}
	else
	{
		setupCamera(&camera);
	}
	drawScene();
}

void drawRightScreen(){
	/* Set the viewport to be rendered to */
	glViewport(screen.x/2, 0, screen.x/2, screen.y);
	/* Clear previous projection (important, otherwise next step won't work) */
	glLoadIdentity();
	if (controls.mainCamera)
	{
		gluLookAt(boat2.pos.x, boat2.pos.y + 5.0, boat2.pos.z, boat1.pos.x, boat1.pos.y, boat1.pos.z, 0, 1, 0);
	}
	else
	{
		setupCamera(&camera);
	}
	drawScene();
}

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawLeftScreen();
	drawRightScreen();
	/* Display result (swaps front and back buffers) */
	glutSwapBuffers();
}

void reshape(int x, int y)
{
	screen.x = x;
	screen.y = y;
	float aspect = x / (float)y;
	aspect /= 2;
	
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
	updateBoat(&boat2, keys.up, keys.down, keys.left, keys.right, dt);
	updateBoat(&boat1, keys.w, keys.s, keys.a, keys.d, dt);

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

void updateKey(int key, bool state)
{
	switch (key)
	{
		case 'w':
			keys.w = state;
			break;
			
		case 's':
			keys.s = state;
			break;
			
		case 'a':
			keys.a = state;
			break;
			
		case 'd':
			keys.d = state;
			break;
			
		default:
			break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
		case 'q':
			/* Exit if esc or q is pressed */
			exit(0);

		case 'W':
			controls.wireframe = !controls.wireframe;
			break;

		case 'N':
			controls.normals = !controls.normals;
			break;

		case 'A':
			controls.axes = !controls.axes;
			break;

		case 'L':
			controls.day = !controls.day;
			break;

		case 'E':
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
			
		case 'w':
		case 'a':
		case 's':
		case 'd':
			updateKey(key, true);
			break;

		default:
			break;
	}
}

void updateKeySpecial(int key, bool state)
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

void keyUp(unsigned char key, int x, int y){
	updateKey(key, false);
}

void keyboardSpecialDown(int key, int x, int y)
{
	updateKeySpecial(key, true);
}

void keyboardSpecialUp(int key, int x, int y)
{
	updateKeySpecial(key, false);
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
	
	/* Setup the terrain */
	initTerrain(&terrain, 500, 500, 500, 20);

	/* Setup the lights */
	initLight(&dayLight, cVec4f(1.2, 1, -1.5, 0), cVec4f(0.4, 0.3, 0.2, 1), cVec4f(0.5, 0.5, 0.5, 1), cVec4f(1, 1, 1, 0), 128);
	initLight(&nightLight, cVec4f(1, 1, -2, 0), cVec4f(0, 0, 0.2, 1), cVec4f(0, 0, 0.2, 1), cVec4f(1, 1, 1, 0), 128);

	/* Load the boat1 */
	initBoat(&boat1, "galleon.obj", cVec3f(0, 0.3, -40));
	initBoat(&boat2, "galleon.obj", cVec3f(0, 0.3, 40));

	/* Set appropriate defaults */
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0);

	/* Enable lighting and lights */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glEnable(GL_TEXTURE_2D);
	
	/* load Textures */
	waterTexture = texture_load("textures/waterTexture.jpg");
	terrainTexture = texture_load("textures/testTexture.png");

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
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(keyboardSpecialDown);
	glutSpecialUpFunc(keyboardSpecialUp);

	/* Init variables, create scene etc */
	init();

	/* Start the main loop (this never returns!) */
	glutMainLoop();

	return EXIT_SUCCESS;
}
