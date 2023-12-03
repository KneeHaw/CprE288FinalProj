/*
 * music.h
 *
 *  Created on: Dec 1, 2023
 *      Author: kneehaw
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "open_interface.h"

void load_song();

void play_songs(oi_t *self);

void do_music();


#endif /* MUSIC_H_ */
