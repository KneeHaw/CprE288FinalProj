#include "music.h"


#define IMERPIAL_MARCH      1


/**
 * Before calling this method, be sure to initialize the open interface by calling:
 *
 *   oi_t* sensor = oi_alloc();
 *   oi_init(sensor); 
 *
 * Loads some songs over the open interface
 */
void load_song() {
	
	unsigned char ImperialMarchNumNotes = 10;
	unsigned char ImperialMarchNotes[10]     = {55, 55, 55, 51, 58, 55, 51, 58, 55, 0};
	unsigned char ImperialMarchDurations[10] = {32, 32, 32, 20, 12, 32, 20, 12, 32, 32};
	oi_loadSong(IMERPIAL_MARCH, ImperialMarchNumNotes, ImperialMarchNotes, ImperialMarchDurations);

}


/*
 * brief Play song
 */
void play_song(){
    oi_play_song(IMERPIAL_MARCH);
}

