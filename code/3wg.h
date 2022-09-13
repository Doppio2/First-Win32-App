#if !defined(_3WG_H)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
// TODO(denis): swap, min, max ...macros???

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

struct game_button_state
{
    int HalfTransitionCount;
    bool32 EndedDown;
};

struct game_controller_input
{
    bool32 IsAnalog;
    
    real32 StartX;
    real32 StartY;

    real32 MinX;
    real32 MinY;

    real32 MaxX;
    real32 MaxY;

    real32 EndY;
    real32 EndX;
    
    union
    {
        game_button_state Buttons[6];
        struct
        {
            game_button_state Up;
            game_button_state Down;
            game_button_state Left;
            game_button_state Right;
            game_button_state LeftShoulder;
            game_button_state RightShoulder;
        };
    };   
};

struct game_input
{
    game_controller_input Controllers[4];
};

internal void GameUpdateAndRender(game_input *Input, game_offscreen_buffer *Buffer, game_sound_output_buffer *SoundBuffer); 

#define _3WG_H
#endif

