#if !defined(_3WG_H)

/*
  TODO(denis): Services that the platform layer to the game.
 */

/*
  NOTE(denis): Services that the game provides to the platform layer.
  (this may expand in the future - sound on separate thread, etc.)
 */

// FOUR TIHNGS - timing, controller/keyboard input, bitmap to use, sound buffer to use 

// TODO(denis): In the future, rendering _specifically_ will be come a thread-tiered abstraction!!!
struct game_offscreen_buffer
{
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};

struct game_sound_output_buffer
{
    int SamplesPerSecond;
    int SampleCount;
    int16 *Samples;
};

internal void GameUpdateAndRender(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset, game_sound_output_buffer *SoundBuffer); 

#define _3WG_H
#endif

