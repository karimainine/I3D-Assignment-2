#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "seabed.h"
#include "png_loader.h"
#include "gl.h"

static Image* terrainLoader;

/* Initialises a 3d terrain of the given tessellation
 and size in GL coordinates. Afterward, only
 the grid Y values and normals need to be updated
 via updateTerrain() */
void initTerrain(Terrain *terrain, int rows, int cols, float size, float height_offset)
{
	int initial_x, initial_z, last_x, last_z, initial_i, last_i;
	int i, j, index, new_index, new_i, new_j;
	float x, z, y;
	
	if (rows < 2)
		rows = 2;
	if (cols < 2)
		cols = 2;
	
	int nVertices = (rows) * (cols);
	int nIndices = (rows - 1) * (cols - 1) * 6;
	
	/* load terrain heightmap */
	terrainLoader = load_png("heightmap.png");
	
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
	
	initial_x = -0.5*size;
	initial_z = -0.5*size;
	last_x = 0.5*size;
	last_z = 0.5*size;
	
	for (i = 0; i < rows; i++)
	{
		x = i / (float)(rows - 1); /* range 0 to 1 */
		x = (x - 0.5) * size; /* range -.5 size to .5 size */
		
		for (j = 0; j < cols; j++)
		{
			z = j / (float)(cols - 1); /* range 0 to 1 */
			z = (z - 0.5) * size; /* range -.5 size to .5 size */
			
			//Y = (X-A)/(B-A) * (D-C) + C
			new_i = (x - initial_x)/(last_x - initial_x) * terrainLoader->width;
			new_j = (z - initial_z)/(last_z - initial_z) * terrainLoader->width;
			new_index = (new_j*terrainLoader->width + new_i) * terrainLoader->channels;
			y = (height_offset - 255) + terrainLoader->data[new_index];
			
			vertices[index].x = x;
			vertices[index].y = y;
			vertices[index].z = z;
			
			//printf("%f %f %f\n", vertices[index].x, vertices[index].y, vertices[index].z);
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
	terrain->rows = rows;
	terrain->cols = cols;
	terrain->size = size;
	terrain->nVertices = nVertices;
	terrain->nIndices = nIndices;
	terrain->vertices = vertices;
	terrain->normals = normals;
	terrain->indices = indices;
	
	calcTerrainNormals(terrain);
}

/* Deletes all memory dynamically allocated by initGrid */
void cleanupTerrain(Terrain *terrain)
{
	free(terrain->vertices);
	free(terrain->normals);
	free(terrain->indices);
	
	terrain->nVertices = 0;
	terrain->nIndices = 0;
	terrain->normals = 0;
	terrain->indices = 0;
}

/* Draws a given grid */
void drawTerrain(Terrain *terrain)
{	
	/* Draw blue water with a white specular highlight */
	static float diffuse[] = {0, 1, 1, 1};
	static float ambient[] = {0, 1, 1, 1};
	static float specular[] = {1, 1, 1, 1};
	static float shininess = 0.0f;
	
	int i;
	
	/* Apply the material, this will interact with the light to
	 produce the final colour */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	/* Draw the grid, as triangles */
	glBegin(GL_TRIANGLES);
	
	for (i = 0; i < terrain->nIndices; i++)
	{
		/* A common cause of bugs/crashing when indexing
		 vertices is accessing arrays out of bounds. assert
		 is a great way to catch this early */
		assert(terrain->indices[i] >= 0 && terrain->indices[i] < terrain->nVertices);
		
		Vec3f v = terrain->vertices[terrain->indices[i]];
		Vec3f n = terrain->normals[terrain->indices[i]];
		Vec2f t = {(v.x/terrain->size) - 0.5, (v.z/terrain->size) - 0.5};
		
		glTexCoord2f(t.x, t.y);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(v.x, v.y, v.z);		
	}
	glEnd();
}

void calcTerrainNormals(Terrain* terrain)
{
	int i, j, index = 0;
	
	for(i=0; i<terrain->rows; i++){
		for(j=0; j<terrain->cols; j++){
			
			//(i,j) -> (i)*cols+(j)
			Vec3f vup = {0,0,0}, vdown = {0,0,0}, vleft = {0,0,0}, vright = {0,0,0};
			
			Vec3f v = terrain->vertices[(i)*terrain->cols+(j)];
			
			if(j<terrain->cols){
				//(i, j+1)
				vdown = terrain->vertices[(i)*terrain->cols+(j+1)];
			}
			
			if(j>0){
				//(i, j-1)
				vup = terrain->vertices[(i)*terrain->cols+(j-1)];
			}
			
			if(i<terrain->rows){
				//(i+1, j)
				vright = terrain->vertices[(i+1)*terrain->cols+(j)];
			}
			
			if(i>0){
				//(i-1, j)
				vleft = terrain->vertices[(i-1)*terrain->cols+(j)];
			}			
			
			Vec3f verticalVector = {vup.x - vdown.x, vup.y - vdown.y, vup.z - vdown.z};
			Vec3f horizontalVector = {vright.x - vleft.x, vright.y - vleft.y, vright.z - vleft.z};
			
			Vec3f normal = getCrossProduct(verticalVector, horizontalVector);
			
			terrain->normals[index++] = normal;
		}
	}
}

Vec3f getCrossProduct(Vec3f a, Vec3f b){
	Vec3f normal;
	float magnitude;
	
	normal.x = (a.y*b.z) - (a.z*b.y);
	normal.y = -((a.z*b.x) - (a.x*b.z));
	normal.z = (a.x*b.y) - (a.y*b.x);
	
	/* Normalize */
	magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	normal.x /= magnitude;
	normal.y /= magnitude;
	normal.z /= magnitude;
	
	return normal;
}

/* Draws normal vectors of the grid as lines, for debugging purposes */
void drawTerrainNormals(Terrain *terrain, float size)
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
	for (i = 0; i < terrain->nVertices; i++)
	{
		glVertex3f(terrain->vertices[i].x, terrain->vertices[i].y, terrain->vertices[i].z);
		glVertex3f(terrain->vertices[i].x + terrain->normals[i].x * size,
					  terrain->vertices[i].y + terrain->normals[i].y * size,
					  terrain->vertices[i].z + terrain->normals[i].z * size);
	}
	glEnd();
	
	/* Return to original rendering state */
	glPopAttrib();
}
