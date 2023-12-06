/*
 * music.c
 *
 *  Created on: Dec 1, 2023
 *      Author: Ethan Rogers (kneehaw@iastate.edu)
 */

#include "music.h"
#include "stdio.h"
#include "open_interface.h"

#define FS5 90
#define F5 89
#define E5 88
#define DS5 87
#define D5n 86
#define CS5 85
#define C5 84
#define B4 83
#define AS4 82
#define A4 81
#define GS4 80
#define nl0 15
#define nl1 10
#define nl2 5
//Sounds best if (nl1 + nl2) % 3 == 0



void play_songs(oi_t *self) {
    int i, j;

    unsigned char note_array1[] = {GS4, AS4, CS5, D5n, E5}; //Play 8 times
    unsigned char note_array2[] = {A4, CS5, E5,
                                   A4, CS5, E5};
    unsigned char note_array3[] = {A4, D5n, FS5,
                                   A4, D5n, FS5};
    unsigned char note_array4[] = {GS4, CS5, E5,
                                   GS4, CS5, E5};
    unsigned char note_array5[] = {GS4, CS5, DS5,
                                   GS4, CS5, DS5,
                                   GS4, CS5, GS4};

    unsigned char length_array1[] = {nl1, nl2, nl1, nl2, nl1}; //Play 8 times
    unsigned char length_array234[] = {nl0, nl0, nl0, nl0, nl0, nl0};
    unsigned char length_array5[] = {nl0, nl0, nl0, nl0, nl0, nl0, nl0, nl0, nl0};


    for (i = 0; i < 1; i++) {
        for (j = 0; j < 4; j++) {
            oi_loadSong(0, 5, note_array1, length_array1);
            oi_play_song(0);
            do {oi_update(self);} while (self->songPlaying);
        }
        oi_loadSong(0, 6, note_array2, length_array234);
        oi_play_song(0);
        do {oi_update(self);} while (self->songPlaying);

        oi_loadSong(0, 6, note_array3, length_array234);
        oi_play_song(0);
        do {oi_update(self);} while (self->songPlaying);

        oi_loadSong(0, 6, note_array4, length_array234);
        oi_play_song(0);
        do {oi_update(self);} while (self->songPlaying);

        oi_loadSong(0, 9, note_array5, length_array5);
        oi_play_song(0);
        do {oi_update(self);} while (self->songPlaying);
    }

}

