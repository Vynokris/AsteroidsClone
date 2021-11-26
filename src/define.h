#pragma once
#include "my_math.h"
#include "raylib.h"
#include <time.h>
#include <string.h>

#define SCREEN_SHAKE_DURATION 15

#define FPS                  60
#define MUSIC_BPM            120.0f
#define MS_PER_BEAT          roundInt(1000 / (MUSIC_BPM / 60))
#define BEATS_TO_FRAMES(int) (double)FPS / 1000 * MS_PER_BEAT * int


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

// Returns a value between 1 and max_scale that corresponds to beat scaling depending on the number of milliseconds until the next beat.
static inline double get_beat_scale_ms(double max_scale, int num_beats, int beat_offset)
{
    // Get the current time.
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    // Get the time since the last beat. Convert the current time in nanoseconds to milliseconds and take it's modulo by the number of milliseconds in a beat.
    int time_since_beat = (floorInt(spec.tv_nsec / 1.0e6) + MS_PER_BEAT * beat_offset) % (MS_PER_BEAT * num_beats);

    // Reduce the scale slowly after the beat.
    if (time_since_beat < MS_PER_BEAT * num_beats - MS_PER_BEAT / 8) {
        return remap(time_since_beat, 0, MS_PER_BEAT * num_beats - MS_PER_BEAT / 8, max_scale, 1);
    }
    // Scale up quickly before the beat.
    else {
        return remap(time_since_beat, MS_PER_BEAT * num_beats - MS_PER_BEAT / 8, MS_PER_BEAT * num_beats, 1, max_scale);
    }
}

// Returns the number of milliseconds since the last beat.
static inline long get_time_since_beat()
{
    // Get the current time.
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    // Get the time since the last beat. Convert the current time in nanoseconds to milliseconds and take it's modulo by the number of milliseconds in a beat.
    return floorInt(spec.tv_nsec / 1.0e6) % MS_PER_BEAT;
}