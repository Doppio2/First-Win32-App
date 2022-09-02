#include "3wg.h"

internal void RenderWeiredGradient(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
   // TODO(denis): Let's see what the optimizer does

    uint8 *Row = (uint8 *)Buffer->Memory;
    for(int Y = 0; Y < Buffer->Height; ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int X = 0; X < Buffer->Width; ++X)
        {
            uint8 Blue = (X + BlueOffset);
            uint8 Green = (Y + GreenOffset);
            
            *Pixel++ = ((Green << 8) | Blue);
        }

        Row += Buffer->Pitch;
    }
}


internal void GameUpdateAndRender(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
    RenderWeiredGradient(Buffer, BlueOffset, GreenOffset);
}
