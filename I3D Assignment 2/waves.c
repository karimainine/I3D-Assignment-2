#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "waves.h"
#include "gl.h"

/* A amplitude, k (period * 2pi), w omega (speed) */
SineFunction sineWaveX = { 1.0f, 0.5f, 1.0f };
SineFunction sineWaveZ = { 1.0f, 0.5f, 1.0f };

/* An absolute measure of time passed (in seconds) */
static float animationTime = 0;

/* Initialises a 2d grid of the given tessellation
   and size in GL coordinates. Afterward, only
   the grid Y values and normals need to be updated
   via updateGrid() */
void initGrid(Grid *grid, int rows, int cols, float size)
{
	int i, j, index;
	float x, z;
	float start = -size / 2.0f;
	
	if (rows < 2)
		rows = 2;
	if (cols < 2)
		cols = 2;
	
	int nVertices = (rows) * (cols);
	int nIndices = (rows - 1) * (cols - 1) * 6;

	/* Allocate memory for the vertex/normal arrays and index
	   array, this will be for drawing triangles */
	Vec3f *vertices = calloc(nVertices, sizeof(Vec3f));
	Vec3f *normals = calloc(nVertices, sizeof(Vec3f));
	int *indices = calloc(nIndices, sizeof(int));

	/* Iterate through the number of rows and columns, populating
	   the vertex array, the x and z values will never change,
	   although the y value will when applying a wave effect to
	   the grid */
	index = 0;
	for (i = 0; i < rows; i++)
	{
		x = i / (float)(rows - 1); /* range 0 to 1 */
		x = (x - 0.5) * size; /* range -.5 size to .5 size */

		for (j = 0; j < cols; j++)
		{
			z = j / (float)(cols - 1); /* range 0 to 1 */
			z = (z - 0.5) * size; /* range -.5 size to .5 size */
		
			vertices[index].x = x;
			vertices[index].z = z;
			index++;
		}
	}

	/* Iterate again, this time creating an array of indices
	   which form triangles by referencing previously
	   generated vertices. This array will never change */
	index = 0;
	#define GRID_VERTEX(i, j) ((i)*cols+(j))
	for (i = 0; i < rows - 1; i++)
	{
		for (j = 0; j < cols - 1; j++)
		{
			indices[index++] = GRID_VERTEX(i, j);
			indices[index++] = GRID_VERTEX(i, j + 1);
			indices[index++] = GRID_VERTEX(i + 1, j);
			indices[index++] = GRID_VERTEX(i + 1, j);
			indices[index++] = GRID_VERTEX(i, j + 1);
			indices[index++] = GRID_VERTEX(i + 1, j + 1);
		}
	}

	/* Create the grid and assign variables */
	grid->rows = rows;
	grid->cols = cols;
	grid->size = size;
	grid->nVertices = nVertices;
	grid->nIndices = nIndices;
	grid->vertices = vertices;
	grid->normals = normals;
	grid->indices = indices;
	
	/* Update the grid Y values */
	updateGrid(grid, 0.0f);
}

/* Deletes all memory dynamically allocated by initGrid */
void cleanupGrid(Grid *grid)
{
	free(grid->vertices);
	free(grid->normals);
	free(grid->indices);

	grid->nVertices = 0;
	grid->nIndices = 0;
	grid->normals = 0;
	grid->indices = 0;
}

/* Draws a given grid */
void drawGrid(Grid *grid)
{
	/* Draw blue water with a white specular highlight */
	static float diffuse[] = {0, 0.5, 1, 0.85};
	static float ambient[] = {0, 0.5, 1, 1};
	static float specular[] = {1, 1, 1, 1};
	static float shininess = 100.0f;

	int i;

	/* Apply the material, this will interact with the light to
	   produce the final colour */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	/* Draw the grid, as triangles */
	glBegin(GL_TRIANGLES);
	
	for (i = 0; i < grid->nIndices; i++)
	{
		/* A common cause of bugs/crashing when indexing
		   vertices is accessing arrays out of bounds. assert
		   is a great way to catch this early */
		assert(grid->indices[i] >= 0 && grid->indices[i] < grid->nVertices);
		
		Vec3f v = grid->vertices[grid->indices[i]];
		Vec3f n = grid->normals[grid->indices[i]];
		Vec2f t = {(v.x/grid->size) - 0.5, (v.z/grid->size) - 0.5};
		
		
		glTexCoord2f(t.x, t.y);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(v.x, v.y, v.z);
		
		/* printf("%f %f %f\n", v.x, v.y, v.z); */
	}
	glEnd();
}

/* Calculates the y value of an individual sine function given a
   position */
float calcHeight(SineFunction *f, float x, float t)
{
	return f->A * sinf(f->k * x + f->w * t);
}

/* Calculates the un-normalized normal vector of a sine function
   given a position */
Vec2f calcNormal(SineFunction *f, float x, float t)
{
	Vec2f n;
	n.x = - f->A * f->k * cosf(f->k * x + f->w * t);
	n.y = 1.0;
	return n;
}


/* Accumulates all sine functions, returning a vec4 where the first 3
   values represent the normal, and w represents the height */
Vec4f calcSineValue(float x, float z)
{
	float magnitude;
	Vec4f v; /* normal x,y,z and height w */
	float t = animationTime;

	/* Sum the heights */
	v.w = calcHeight(&sineWaveX, x, t) + calcHeight(&sineWaveZ, z, t);
	
	/* Find the normal for each sine wave */
	Vec2f normalX = calcNormal(&sineWaveX, x, t);
	Vec2f normalZ = calcNormal(&sineWaveZ, z, t);
	
	/* Combine the normals */
	/* Note: this only works because the sine waves are linearly
	         independent and do not "share" x/z */
	v.x = normalX.x;
	v.y = 1.0f;
	v.z = normalZ.x;
	
	/* Normalize */
	magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x /= magnitude;
	v.y /= magnitude;
	v.z /= magnitude;
	
	return v;
}

/* Updates the given grid to apply a wave effect based on a sine wave, 
   animates using dt */
void updateGrid(Grid *grid, float dt)
{
	int i;

	animationTime += dt;

	for (i = 0; i < grid->nVertices; i++)
	{
		Vec4f v = calcSineValue(grid->vertices[i].x, grid->vertices[i].z);

		/* Set the height of the vertex and the value of the normal 
		   vector */
		grid->vertices[i].y = v.w;
		grid->normals[i].x = v.x;
		grid->normals[i].y = v.y;
		grid->normals[i].z = v.z;
	}
}

/* Draws normal vectors of the grid as lines, for debugging purposes */
void drawNormals(Grid *grid, float size)
{
	int i;

	/* We'll be changing the current colour and disabling lighting, 
	   best to save these states */
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);

	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 0);

	glBegin(GL_LINES);

	/* Iterate through all vertices and draw a line at each representing 
	   the direction of the normal vector */
	for (i = 0; i < grid->nVertices; i++)
	{
		glVertex3f(grid->vertices[i].x, grid->vertices[i].y, grid->vertices[i].z);
		glVertex3f(grid->vertices[i].x + grid->normals[i].x * size,
			grid->vertices[i].y + grid->normals[i].y * size,
			grid->vertices[i].z + grid->normals[i].z * size);
	}

	glEnd();

	/* Return to original rendering state */
	glPopAttrib();
}
