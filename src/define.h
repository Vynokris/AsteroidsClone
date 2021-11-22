#pragma once
#include "my_math.h"

#define SCREEN_SHAKE_DURATION 15

#define FPS             60
#define MUSIC_BPM       120.0f
#define FRAMES_PER_BEAT roundInt(FPS / (MUSIC_BPM / 60))


#define PLAYER_MAX_VELOCITY 8
#define PLAYER_SPEED        0.5
#define BEAT_COLOR          (Color){ 149, 204, 222, 255 }


#define BULLET_MAX_AMOUNT 120
#define BULLET_SPEED      12
#define BULLET_LIFESPAN   120


#define ASTEROID_MAX_AMOUNT 64
#define ASTEROID_SIZE(type)  (type==0 ? 0    : (type==1 ? 30   : (type==2 ? 60   : 90)))   // A_DESTROYED: 0,   A_SMALL: 30,  A_MEDIUM: 60,  A_LARGE: 90
#define ASTEROID_SIDES(type) (type==0 ? 0    : (type==1 ? 5    : (type==2 ? 8    : 11)))   // A_DESTROYED: 0,   A_SMALL: 5,   A_MEDIUM: 8,   A_LARGE: 11
#define ASTEROID_SPEED(type) (type==0 ? 0.0f : (type==1 ? 2.5f : (type==2 ? 2.0f : 1.5f))) // A_DESTROYED: 0.0, A_SMALL: 2.5, A_MEDIUM: 2.0, A_LARGE: 1.5


#define PARTICLE_MAX_AMOUNT 256

// Returns a value between 1 and max_scale that corresponds to beat scaling depending on the number of frames until the next beat.
static inline double get_beat_scale(int frames_till_beat, double max_scale, int num_beats)
{
    if (frames_till_beat > FRAMES_PER_BEAT / 8) {
        return remap(frames_till_beat, FRAMES_PER_BEAT * num_beats, FRAMES_PER_BEAT / 8, max_scale, 1);
    }
    else {
        return remap(frames_till_beat, FRAMES_PER_BEAT / 8, 0, 1, max_scale);
    }
}