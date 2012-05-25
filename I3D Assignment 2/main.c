#include <stdlib.h>
#include "main.h"

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
Sky sky;

bool gameOver;

int frame=0, time, timebase=0;

void drawScene(){
	if(gameOver){
		printGO();
	}else{
		float ambient1 [] = { 45/255.0, 35/255.0, 33/255.0, 1.0f };
		float ambient2 [] = { 191/255.0, 163/255.0, 141/255.0, 1.0f };
		
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
		
		printFPS(-6, 6, -10);
		
		glBindTexture(GL_TEXTURE_2D, terrainTexture);
		drawTerrain(&terrain);
		
		drawBoat(&boat1, ambient1);
		drawAllBalls(&boat1);
		
		drawBoat(&boat2, ambient2);
		drawAllBalls(&boat2);
		
		glBindTexture(GL_TEXTURE_2D, waterTexture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		drawGrid(&grid);
		glDisable(GL_BLEND);
		
		if (controls.normals){
			drawNormals(&grid, 1);
			drawTerrainNormals(&terrain, 1);
		}	
	}
}

void drawLeftScreen(){
	/* Set the viewport to be rendered to */
	glViewport(0, 0, screen.x/2, screen.y);
	/* Clear previous projection (important, otherwise next step won't work) */
	glLoadIdentity();
	if (controls.mainCamera)
	{
		float modelview[16];
		Vec3f pos;
		
		gluLookAt(boat1.pos.x, boat1.pos.y + 5.0, boat1.pos.z, boat2.pos.x, boat2.pos.y, boat2.pos.z, 0, 1, 0);
		glPushMatrix();
		glTranslatef(boat1.pos.x, boat1.pos.y + 5.0, boat1.pos.z);
		drawSky(&sky);
		glPopMatrix();
		
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
		
		glPushMatrix();
		glTranslatef(boat2.pos.x, boat2.pos.y + 5.0, boat2.pos.z);
		drawSky(&sky);
		glPopMatrix();
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

	if(!gameOver){
		updateGrid(&grid, dt);
		updateBoat(&boat2, keys.up, keys.down, keys.left, keys.right, dt, &keys.boat2FireLeft, &keys.boat2FireRight);
		updateAllBalls(&boat2);
		ballsHitBoat(&boat1, &boat2);
		
		updateBoat(&boat1, keys.w, keys.s, keys.a, keys.d, dt, &keys.boat1FireLeft, &keys.boat1FireRight);
		updateAllBalls(&boat1);
		ballsHitBoat(&boat2, &boat1);
		
		checkCollision();
	}

	glutPostRedisplay();
}

void checkCollision(){
	bool boats_collided = boatsCollided(&boat1, &boat2);
	bool boat1Hit = boatDestroyed(&boat1);
	bool boat2Hit = boatDestroyed(&boat2);
	
	gameOver = (boats_collided || boat1Hit || boat2Hit);
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
		
		case 'q':
			keys.boat1FireLeft = state;
			break;
		
		case 'e':
			keys.boat1FireRight = state;
			break;
		
		case ',':
			keys.boat2FireLeft = state;
			break;
		
		case '.':
			keys.boat2FireRight = state;
			break;
		
		default:
			break;
	}
}

void printFPS(float x, float y, float z){
	int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
	char s[1024];
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	
	static float fps;
	if (time - timebase > 1000) {
		fps = frame*1000.0/(time-timebase);
		timebase = time;
		frame = 0;
	}
	snprintf(s, 1024,"FPS:%4.2f", fps);
	//printf("%s\n", s);
	
	glColor3f(0.1f,0.1f,0.1f);
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(x, y, z, (void *)font, s);
	glPopMatrix();
}

void printGO(){
	int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
	
	glColor3f(0.1f,0.1f,0.1f);
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(0, 0, -10,(void *)font,"Game Over");
	glPopMatrix();
}

void renderBitmapString(float x, float y, float z, void *font, char *string){  
	char *c;
	//printf("FPS:%s\n", string);
	glRasterPos3f(x, y, z);
	//glTranslatef(x, y, z);
	if(string){
		for (c=string; *c != '\0'; c++) {
			glutBitmapCharacter(font, *c);
		}
	}
	
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
		case 'Q':
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
		case 'q':
		case 'e':
		case ',':
		case '.':
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
	gameOver = false;
	initSky(&sky, 1);
	/* Setup the terrain */
	initTerrain(&terrain, 200, 200, 200, 20);
	
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

	/* Load the boat1 */
	initBoat(&boat1, "galleon.obj", cVec3f(0, 0.3, -25));
	initBoat(&boat2, "galleon.obj", cVec3f(0, 0.3, 25));

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	/* load Textures */
	waterTexture = texture_load("textures/ocean.jpg");
	terrainTexture = texture_load("textures/wetRocks.jpg");

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
