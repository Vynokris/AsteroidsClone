#pragma once
#include "my_math.h"

#define SCREEN_SHAKE_DURATION 15


#define MUSIC_BPM 120.0f
#define FRAMES_PER_BEAT (60 / (MUSIC_BPM / 60))


#define PLAYER_MAX_VELOCITY 8
#define PLAYER_SPEED 0.5


#define BULLET_MAX_AMOUNT 120
#define BULLET_SPEED 12
#define BULLET_LIFESPAN 120


#define ASTEROID_MAX_AMOUNT 64
#define ASTEROID_SIZE(type) (type==0 ? 0 : (type==1 ? 30 : (type==2 ? 60 : 90))) // A_DESTROYED: 0, A_SMALL: 30, A_MEDIUM: 60, A_LARGE: 90
#define ASTEROID_SIDES(type) (type==0 ? 0 : (type==1 ? 5 : (type==2 ? 8 : 11))) // A_DESTROYED: 0, A_SMALL: 5, A_MEDIUM: 8, A_LARGE: 11
#define ASTEROID_SPEED(type) (type==0 ? 0.0f : (type==1 ? 2.5f : (type==2 ? 2.0f : 1.5f))) // A_DESTROYED: 0.0, A_SMALL: 2.5, A_MEDIUM: 2.0, A_LARGE: 1.5


#define PARTICLE_MAX_AMOUNT 256


// Returns a value between 1 and 1.05 that corresponds to beat scaling at the current frame index.
// This function updates the given frame index.
static inline double get_beat_scale(int* frame_i, double max_scale, int num_beats)
{
    double output;

    // Make the ui scale to the bpm.
    if (*frame_i <= FRAMES_PER_BEAT / 8) {
        output = remap(*frame_i, 0, FRAMES_PER_BEAT / 8, 1, max_scale);
    }
    else {
        output = remap(*frame_i, FRAMES_PER_BEAT / 8, FRAMES_PER_BEAT * num_beats - 1, max_scale, 1);
    }

    (*frame_i)++;

    // Reset the frames counter when it is too high.
    if (*frame_i >= FRAMES_PER_BEAT * num_beats) { 
        *frame_i = 0; 
    }

    return output;
}