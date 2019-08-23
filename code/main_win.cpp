
#include <stdio.h>
#include <windows.h>

#include "main_win.h"
#include "main.h"
#include "renderer.h"
#include "game_math.h"
#include "memory.h"

WinState winState = {};
bool IsRunning = 1;

MemoryInfo Alloc(uSize size) {
	MemoryInfo mi = {};
	
	mi.base = VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	mi.size = size;
	
	return mi;
}

uSize PlatformReadFile(void *base, char *fileName) {
	HANDLE fileHandle = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (!fileHandle)
		return 0;
	
	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(fileHandle, &fileSize))
	{
		CloseHandle(fileHandle);
		return 0;
	}
	
	if (fileSize.QuadPart <= 0) {
		CloseHandle(fileHandle);
		return 0;
	}
	
	// TODO: fix this windows jank; casting 64bit fileSize to 32bits, returning fileSize not
	// bytes read, unable to read more than 2^32 data and no fail on file too large.
	DWORD bytesRead = 0;
	if (!ReadFile(fileHandle, base, (u32)fileSize.QuadPart, &bytesRead, 0) || bytesRead != fileSize.QuadPart)
	{
		CloseHandle(fileHandle);
		return 0;
	}
	
	CloseHandle(fileHandle);
	return fileSize.QuadPart;
}

v2i PlatformGetScreenSize() {
	return winState.screenSize;
}

void InitInput(GameInput *gameInput) {
	// TODO: load imputs from file, if no file exists create one with default inputs
	gameInput->keyMap[KeyUp] = 'W';
	gameInput->keyMap[KeyDown] = 'S';
	gameInput->keyMap[KeyLeft] = 'A';
	gameInput->keyMap[KeyRight] = 'D';
	gameInput->keyMap[KeyRun] = VK_LSHIFT;
	gameInput->keyMap[KeyPause] = VK_ESCAPE;
}

void OnScreenSizeChange() {
	RECT rect;
	GetClientRect(winState.window, &rect);
	winState.screenSize = V2I(rect.right, rect.bottom);
}

LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    
    switch(message)
    {
        case WM_CLOSE:
        {
			IsRunning = false;
			
            break;
        }
		
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            Assert(!"Input message came through non-dispatched message");
            break;
        }
		
		case WM_EXITSIZEMOVE:
		{
			OnScreenSizeChange();
			
			break;
		}
		
		case WM_SIZE:
		{
			if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
				OnScreenSizeChange();
			
			break;
		}
		
        default:
        {
            result = DefWindowProc(window, message, wParam, lParam);
            break;
        }
    }
    
    return result;
}

void ProcessInput(InputKey *inputKey, bool isDown)
{
    if (inputKey->isDown != isDown)
    {
        inputKey->isDown = isDown;
        ++inputKey->count;
    }
}

void ProcessMessages(GameInput *gameInput)
{
    MSG message;
    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        switch(message.message)
        {
            case WM_QUIT:
            {
				IsRunning = false;
				
                break;
            }
			
            case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				if ((u32)message.wParam == VK_F4 && message.lParam & (1 << 29))
					IsRunning = false;
				
				ProcessInput(&gameInput->keys[(u32)message.wParam], 1);
				TranslateMessage(&message);
				break;
			}
			
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				ProcessInput(&gameInput->keys[(u32)message.wParam], 0);
				break;
			}
            
            case WM_LBUTTONDOWN:
			{
				ProcessInput(&gameInput->keys[VK_LBUTTON], 1);
				break;
			}
            case WM_LBUTTONUP:
            {
                ProcessInput(&gameInput->keys[VK_LBUTTON], 0);
                break;
            }
            
            case WM_RBUTTONDOWN:
			{
                ProcessInput(&gameInput->keys[VK_RBUTTON], 1);
                break;
            }
            case WM_RBUTTONUP:
            {
                ProcessInput(&gameInput->keys[VK_RBUTTON], 0);
                break;
            }
			
			case WM_MBUTTONDOWN:
			{
                ProcessInput(&gameInput->keys[VK_MBUTTON], 1);
                break;
            }
            case WM_MBUTTONUP:
            {
                ProcessInput(&gameInput->keys[VK_MBUTTON], 0);
                break;
            }
			
			/*case WM_MOUSEWHEEL:
			{
				gameInput->wheelDelta = GET_WHEEL_DELTA_WPARAM(message.wParam);
				break;
			}*/
			
            default:
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
                break;
            }
        }
    }
}

bool InitWin() {
	WNDCLASS windowClass = {};
    
    windowClass.style = CS_HREDRAW|CS_VREDRAW;
    windowClass.lpfnWndProc = MainWindowProc;
    windowClass.hInstance = winState.instance;
    windowClass.lpszClassName = "DevillessClass";
	
	if (!RegisterClass(&windowClass))
        return 0;
	
	winState.screenSize = V2I(1280, 720);
	winState.window = CreateWindowEx(0, windowClass.lpszClassName, "Devilless", WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0, winState.screenSize.x, winState.screenSize.y, 0, 0, 0, 0);
	OnScreenSizeChange();
	winState.windowDC = GetDC(winState.window);
	
	return 1;
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode) {
	AllocConsole();
	freopen("CONIN$", "r",stdin);
	freopen("CONOUT$", "w",stdout);
	freopen("CONOUT$", "w",stderr);
	
	winState.instance = instance;
	
	printf("hello sailor!\n");
	
	if (!InitWin())
		return 1;
	
	InitRenderer();
	
	GameState gameState = {};
	GameInput gameInput = {};
	TempMemory tempMemory = {};
	
	MemoryInfo mi = Alloc(Megabytes(128));
	StackInit(&tempMemory.stack, mi);
	
	InitInput(&gameInput);
	
	GameInit(&gameState, &gameInput, &tempMemory);
	
	LARGE_INTEGER countFrequency;
	QueryPerformanceFrequency(&countFrequency);
	
	LARGE_INTEGER frameStartCounter;
	QueryPerformanceCounter(&frameStartCounter);
	
	while (IsRunning) {
		for (u32 i = 0; i < ArrayCount(gameInput.keys); i++)
		{
			gameInput.keys[i].count = 0;
		}
		
		ProcessMessages(&gameInput);
		
		/*if (activeWindow)
		{
			gameInput->mousePos = ;
		}
		else
		{
			ClearInput();
		}*/
		
		
		ClearFrame();
		
		GameUpdate(&gameState, &gameInput, &tempMemory);
		
		PresentFrame();
		
		LARGE_INTEGER frameFinishCounter;
		QueryPerformanceCounter(&frameFinishCounter);
		
		u64 counterElapsed = frameFinishCounter.QuadPart - frameStartCounter.QuadPart;
		gameState.deltaTime = Min(0.05f, (f32)counterElapsed / (f32)countFrequency.QuadPart);
		//printf("%f\n", gameState.deltaTime);
		
		frameStartCounter = frameFinishCounter;
	}
	
	return 0;
}