/*
 * movement.h
 *
 *  Created on: Sep 3, 2023
 *      Author: joo
 */
#ifndef MUSIC_H_
#define MUSIC_H_


#include "open_interface.h"


/**
 * Before calling this method, be sure to initialize the open interface by calling:
 *
 *   oi_t* sensor = oi_alloc();
 *   oi_init(sensor);
 *
 * Loads some songs over the open interface
 */
void load_song();


/*
 * brief Play song
 */
void play_song();


#endif /* MUSIC_H_ */

