#ifndef KEYS_H
#define KEYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

/* Struct to store the state of movement keys for the boat */
typedef struct
{
	/* Flags to determine if keys are pressed */
	bool up;
	bool down;
	bool left;
	bool right;
	bool a;
	bool s;
	bool d;
	bool w;
	bool boat1FireLeft;
	bool boat1FireRight;
	bool boat2FireLeft;
	bool boat2FireRight;
} Keys;


/* Initialises key states to false */
void initKeys(Keys *keys);


#ifdef __cplusplus
}
#endif

#endif
