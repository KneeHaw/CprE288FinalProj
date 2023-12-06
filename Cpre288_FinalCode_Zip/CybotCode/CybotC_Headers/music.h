/*
 * music.h
 *
 *  Created on: Dec 1, 2023
 *      Author: Ethan Rogers (kneehaw@iastate.edu)
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "open_interface.h"

void play_songs(oi_t *self);

#endif /* MUSIC_H_ */
