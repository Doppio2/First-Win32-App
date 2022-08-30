/*
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
First Win32_application
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

#include <windows.h>
#include <stdint.h>
#include <xinput.h>
#include <dsound.h>

// TODO(denis): Implement sine ourselves
#include <math.h>

#define internal static
#define local_persist static
#define global_variable static

#define Pi32 3.1415926535f

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};

// TODO(denis): This is global for now. 
global_variable bool32 GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackbuffer;
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;

// NOTE(denis): XInputGetState
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return(ERROR_DEVICE_NOT_CONNECTED);
}
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

// NOTE(denis): XInputSetState
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
    return(ERROR_DEVICE_NOT_CONNECTED);
}
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

internal void Win32LoadXInput(void)
{
    HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if(!XInputLibrary)
    {
        // TODO(denis): Diagnostic
        XInputLibrary = LoadLibraryA("xinput9_1_0.dll");
    }

    if(!XInputLibrary)
    {
        // TODO(denis): Diagnostic
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }
    
    if(XInputLibrary)
    {
        XInputGetState = (x_input_get_state *)GetProcAddress(XInputLibrary,"XInputGetState");
        if(!XInputGetState) {XInputGetState = XInputGetStateStub;}
        
        XInputSetState = (x_input_set_state *)GetProcAddress(XInputLibrary,"XInputSetState");
        if(!XInputGetState) {XInputSetState = XInputSetStateStub;}
        
    }
}

internal void Win32InitDSound(HWND Window, int32 SamplesPerSecond, int32 BufferSize)
{
    // NOTE(denis): Load the library
    HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
    if(DSoundLibrary)
    { 
        // NOTE(deins): Get a a DirectSound object! - cooperative
        direct_sound_create *DirectSoundCreate = (direct_sound_create *) GetProcAddress(DSoundLibrary, "DirectSoundCreate");

        LPDIRECTSOUND DirectSound;
        if(DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
        {
            WAVEFORMATEX WaveFormat = {};
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nChannels = 2;
            WaveFormat.nSamplesPerSec = SamplesPerSecond;
            WaveFormat.wBitsPerSample = 16;
            WaveFormat.nBlockAlign = (WaveFormat.nChannels*WaveFormat.wBitsPerSample) / 8;
            WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec*WaveFormat.nBlockAlign;
            WaveFormat.cbSize = 0;

            if(SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
            {
                DSBUFFERDESC BufferDescription = {};
                BufferDescription.dwSize = sizeof(BufferDescription);
                BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

                // TODO(denis): DSBCAPS_GLOBALFOCUS?
                // NOTE(denis): "Create" a primary buffer
                LPDIRECTSOUNDBUFFER PrimaryBuffer;
                if(SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0)))
                {
                    HRESULT Error = (PrimaryBuffer->SetFormat(&WaveFormat));
                    if(SUCCEEDED(Error))
                    {
                        // NOTE(denis): We have finally set the format!
                        OutputDebugStringA("Primary buffer format was set.\n");
                    }
                    else
                    {
                        // TODO(denis): Diagnostic
                    }
                }
                else
                {
                    // TODO(denis): Diagnostic
                }
            }
            else
            {
                // TODO(denis): Diagnostic
            }

            // TODO(casey): DSBCAPS_GETCURRENTPOSITION2
            DSBUFFERDESC BufferDescription = {};
            BufferDescription.dwSize = sizeof(BufferDescription);
            BufferDescription.dwFlags = 0;
            BufferDescription.dwBufferBytes = BufferSize;
            BufferDescription.lpwfxFormat = &WaveFormat;
            HRESULT Error = DirectSound->CreateSoundBuffer(&BufferDescription, &GlobalSecondaryBuffer, 0); 
            if(SUCCEEDED(Error))
            {
                OutputDebugStringA("Secondary buffer created successfully .\n");
            }
        }
        else
        {
            // TODO(denis): Diagnostic
        }
    }
    else
    {
        // TODO(denis): Diagnostic
    }
}

internal win32_window_dimension Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;

    return(Result);
}

internal void RenderWeiredGradient(win32_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
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

internal void Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
    // TODO(Deins): Bulletproof this.
    // maybe don't free first, fre after, then free first if that fails.

    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;
    Buffer->BytesPerPixel = 4;

    // TODO(denis): When the biHeight is negative, this is the clue to
    // Windows to treat this bitmap as top-down, not bottom-up, meaning that
    // the first three bytes of the image are the color for is the top left pixel
    // in the bitmap, not the bottom left!
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;    
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    // NOTE(Deins): thank for clarifying the deal with StretchDIBts and and BitBlit!
    // No more DC for us.
    int BitmapMemorySize = (Buffer->Width*Buffer->Height)*Buffer->BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    Buffer->Pitch = Width*Buffer->BytesPerPixel;

    // TODO(denis): Probably clear this to black
}

internal void Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    // TODO(denis): Aspect ration correctoin
    StretchDIBits(DeviceContext, 0, 0, WindowWidth, WindowHeight, 0, 0, Buffer->Width, Buffer->Height, Buffer->Memory, &Buffer->Info, DIB_RGB_COLORS, SRCCOPY);
}

internal LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM  LParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_SIZE:
        {            
        } break;

        case WM_DESTROY:
        {
            // Todo(denis): Handle this as an error - recreate window?
            GlobalRunning = false;
        } break;

        case WM_CLOSE:
        {
            // Todo(denis): Handle this with a message to user?
            GlobalRunning = false;
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVEATEAPP\n");
        } break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32 VKCode = WParam;
            bool32 WasDown = ((LParam & (1 << 30)) != 0);
            bool32 IsDown = ((LParam & (1 << 31)) == 0);
            if(WasDown != IsDown)
            {
                if(VKCode == 'W')
                {
                }
                else if(VKCode == 'A')
                {
                }
                else if(VKCode == 'S')
                {
                }
                else if(VKCode == 'D')
                {
                }
                else if(VKCode == 'Q')
                {
                }
                else if(VKCode == 'E')
                {
                }
                else if(VKCode == VK_UP)
                {            
                }
                else if(VKCode == VK_LEFT)
                {            
                }
                else if(VKCode == VK_DOWN)
                {            
                }
                else if(VKCode == VK_RIGHT)
                {            
                }
                else if(VKCode == VK_ESCAPE)
                {
                    OutputDebugStringA("ESCAPE: ");
                    if(IsDown)
                    {
                        OutputDebugStringA("IsDown");
                    }
                    if(WasDown)
                    {
                        OutputDebugStringA("WasDown");
                    }
                    OutputDebugStringA("\n");
                }
                else if(VKCode == VK_SPACE)
                {            
                }
            }

            bool32 AltKeyWasDown = (LParam & (1 << 29));
            if((VKCode == VK_F4) && AltKeyWasDown)
            {
                GlobalRunning = false;
            }
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            
            win32_window_dimension Dimension = Win32GetWindowDimension(Window);            
            Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        
        } break;

        default:
        {
//            OutputDebugStringA("default\n");
            Result = DefWindowProcA(Window, Message, WParam, LParam); 
        } break;
    }

    return(Result);
}

struct win32_sound_output
{
    // NOTE(denis): Sound test
    int SamplesPerSecond;
    int ToneHz;
    int16 ToneVolume;
    uint32 RunningSampleIndex;
    int WavePeriod;
    int BytesPerSample;
    int SecondaryBufferSize;
    real32 tSine;
    int LatencySampleCount;
};

internal void Win32FillSoundBuffer(win32_sound_output *SoundOutput, DWORD ByteToLock, DWORD BytesToWrite)
{
    // TODO(denis): More strenuouse test!
    VOID *Region1;
    DWORD Region1Size;
    VOID *Region2;
    DWORD Region2Size;
    if(SUCCEEDED(GlobalSecondaryBuffer->Lock(ByteToLock, BytesToWrite, &Region1, &Region1Size, &Region2, &Region2Size, 0)))
    {

        // TODO(denis): assert that Region1Size/Region2Size is valid

        // TODO(denis): Collapse these two loops
        int16 *SampleOut = (int16 *)Region1;
        DWORD Region1SampleCount = Region1Size/SoundOutput->BytesPerSample;
        for(DWORD SampleIndex = 0; SampleIndex < Region1SampleCount; ++SampleIndex)
        {
            // TODO(denis): Draw this out of people
            real32 SineValue = sinf(SoundOutput->tSine);
            int16 SampleValue = (int16)(SineValue * SoundOutput->ToneVolume);
            *SampleOut++ = SampleValue;
            *SampleOut++ = SampleValue;

            SoundOutput->tSine += 2.0f*Pi32*1.0f/(real32)SoundOutput->WavePeriod;
            ++SoundOutput->RunningSampleIndex;
        }
        
        DWORD Region2SampleCount = Region2Size/SoundOutput->BytesPerSample;
        SampleOut = (int16 *)Region2;
        for(DWORD SampleIndex = 0; SampleIndex < Region2SampleCount; ++SampleIndex)
        {
            real32 SineValue = sinf(SoundOutput->tSine);
            int16 SampleValue = (int16)(SineValue * SoundOutput->ToneVolume);
            *SampleOut++ = SampleValue;
            *SampleOut++ = SampleValue;
            *SampleOut++ = SampleValue;

            SoundOutput->tSine += 2.0f*Pi32*1.0f/(real32)SoundOutput->WavePeriod;
            ++SoundOutput->RunningSampleIndex;
        }

        GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
    }
}
    
int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandeLine, int ShowCode)
{
    Win32LoadXInput();

    WNDCLASSA WindowClass = {};

    Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);
    
    WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;      
    WindowClass.hInstance = Instance;
//  WindowClass.hIcon = ;
    WindowClass.lpszClassName = "3WGWindowClass";

    if(RegisterClass(&WindowClass))
    {
        HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "3WG", WS_OVERLAPPEDWINDOW|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, Instance, 0);
        if(Window)
        { 
            HDC DeviceContext = GetDC(Window);

            // NOTE(denis): Graphic test
            int XOffset = 0;
            int YOffset = 0;


            win32_sound_output SoundOutput = {};

            // NOTE(denis): Make this like sixty seconds?
            SoundOutput.SamplesPerSecond = 48000;
            SoundOutput.ToneHz = 256;
            SoundOutput.ToneVolume = 3000;
            SoundOutput.WavePeriod = SoundOutput.SamplesPerSecond / SoundOutput.ToneHz;
            SoundOutput.BytesPerSample = sizeof(int16)*2;
            SoundOutput.SecondaryBufferSize = SoundOutput.SamplesPerSecond*SoundOutput.BytesPerSample;;
            SoundOutput.LatencySampleCount = SoundOutput.SamplesPerSecond / 15;
            Win32InitDSound(Window, SoundOutput.SamplesPerSecond, SoundOutput.SecondaryBufferSize);
            Win32FillSoundBuffer(&SoundOutput, 0, SoundOutput.LatencySampleCount*SoundOutput.BytesPerSample);
            GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
            bool32 SoundIsPlaying = false;            
            
            GlobalRunning = true;
            while(GlobalRunning)
            {
                MSG Message;
                while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                {
                    if(Message.message == WM_QUIT)
                    {
                        GlobalRunning = false;
                    }

                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);                
                }

                // TODO(denis): Should we poll this more more frequently 
                for(DWORD ControllerIndex = 0; ControllerIndex < XUSER_MAX_COUNT; ++ControllerIndex)
                {
                    XINPUT_STATE ControllerState;
                    if(XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
                    {
                        // NOTE(denis): This controller is plugged in
                        // TODO(denis): See if ControllerState.dwPacketNumber increments too rapidly
                        XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

                        bool32 Up = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                        bool32 Down = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                        bool32 Left = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                        bool32 Right = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                        bool32 Start = (Pad->wButtons & XINPUT_GAMEPAD_START);
                        bool32 Back = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
                        bool32 LeftShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                        bool32 RightShoulder = (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                        bool32 AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
                        bool32 BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
                        bool32 XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
                        bool32 YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);

                        int16 StickX = Pad->sThumbLX;
                        int16 StickY = Pad->sThumbLY;

                        // TODO(denis): We will do deadzone handling later using
                        // XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
                        // XINPUT_GAMEPAD_RIGHT_THUMG_DEADZONE 8689

                        XOffset += StickX / 4096;
                        YOffset += StickY / 4096;

                        SoundOutput.ToneHz = 512 + (int)(256.0f*((real32)StickY / 30000.0f));
                        SoundOutput.WavePeriod = SoundOutput.SamplesPerSecond/SoundOutput.ToneHz;
                    }
                    else
                    {
                        // NOTE(denis): The controller is not availible
                    }
                }
                
                // TODO(denis): Checkout why YOffset not working
                RenderWeiredGradient(&GlobalBackbuffer, XOffset, YOffset);                                

                // NOTE(denis): DirectSound output sound
                DWORD PlayCursor;
                DWORD WriteCursor;
                if(!SoundIsPlaying && SUCCEEDED(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor)))
                {
                    DWORD ByteToLock = (SoundOutput.RunningSampleIndex*SoundOutput.BytesPerSample) % SoundOutput.SecondaryBufferSize;
                    DWORD TargetCursor = ((PlayCursor + (SoundOutput.LatencySampleCount*SoundOutput.BytesPerSample)) % SoundOutput.SecondaryBufferSize);
                    DWORD BytesToWrite;

                    // TODO(denis): Change this to using a lower latency offset from the TargetCursor
                    // when we actually start having sound effects.
                    if(ByteToLock > TargetCursor)
                    {
                        BytesToWrite = (SoundOutput.SecondaryBufferSize - ByteToLock);
                        BytesToWrite += TargetCursor;
                    }
                    else
                    {
                        BytesToWrite = TargetCursor - ByteToLock;
                    }

                    Win32FillSoundBuffer(&SoundOutput, ByteToLock, BytesToWrite);
                }

                win32_window_dimension Dimension = Win32GetWindowDimension(Window);
                Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);                
            }
        }
        else
        {
            // TODO(denis) Logging
        }
    }
    else
    {
        // TODO(denis) Logging
    }
    
    return(0);
}
