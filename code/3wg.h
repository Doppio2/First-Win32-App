#if !defined(_3WG_H)

/*
  NOTE(denis): 

  _3WG_INTERNAL:
      0 - Build for public release.
      1 - Build for developer only.
      
  _3WG_SLOW:
      0 - Not slow code allowed!
      1 - Slow code welcome.
 */

#if _3WG_SLOW
// TODOD(denis): Complete assertion macro
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
// TODO(denis): swap, min, max ...macros???

inline uint32 SafeTrancateUInt64(uint64 Value)
{
    // TODO(denis): Defines for maximum values Uint32Max
    Assert(Value <= 0xFFFFFFFF);
    uint32 Result = (uint32)Value;
    return(Result);
}

/*
  NOTE(denis): Services that the platform layer to the game.
 */
#if _3WG_INTERNAL
/* IMPORTANT(denis):

   These are NOT for doing anything in the shipping game - they are
   blocking and the write doesn't protect against lost data!
 */
struct debug_read_file_result
{
    uint32 ContentsSize;
    void *Contents;
};
internal debug_read_file_result DEBUGPlatformReadEntireFile(char *Filename);
internal void DEBUGPlatformFreeFileMemory(void *Memory);
internal bool32 DEBUGPlatformWriteEntireFile(char *Filename, uint32 MemorySize, void *Memory);
#endif

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
    // TODO(denis): insert clock values here.
    game_controller_input Controllers[4];
};

struct game_memory
{
    bool32 IsInitialized;
    
    uint64 PermanentStorageSize;
    void *PermanentStorage; // NOTE(denis): REQUIRED to cleared to zero at startup

    uint64 TransientStorageSize;
    void *TransientStorage; // NOTE(denis): REQUIRED to cleared to zero at startup
};

internal void GameUpdateAndRender(game_memory *Memory, game_input *Input, game_offscreen_buffer *Buffer, game_sound_output_buffer *SoundBuffer); 

//
//
//

struct game_state
{
    int ToneHz;
    int GreenOffset;
    int BlueOffset;
};

#define _3WG_H
#endif

