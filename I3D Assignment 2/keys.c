#include "keys.h"

/* Initialises key states to false */
void initKeys(Keys *keys)
{
	keys->up = false;
	keys->down = false;
	keys->left = false;
	keys->right = false;
	keys->w = false;
	keys->a = false;
	keys->s = false;
	keys->d = false;
	keys->boat1FireLeft = false;
	keys->boat1FireRight = false;
	keys->boat2FireLeft = false;
	keys->boat2FireRight = false;
}

