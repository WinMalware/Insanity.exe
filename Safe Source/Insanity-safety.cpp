// APM 08279+5255.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <windows.h>
#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
//#include <time.h>
//#include "bootrec.h"
#include <cmath>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "amogus.h" //yes I know I'm not funny
#define M_PI   3.14159265358979323846264338327950288
//#define PI   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE Reserved;
	};
} _RGBQUAD, *PRGBQUAD;

typedef struct {
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors {
	HSL rgb2hsl(_RGBQUAD rgb) {
		HSL hsl;

		BYTE rgbRed = rgb.r;
		BYTE rgbGreen = rgb.g;
		BYTE rgbBlue = rgb.b;

		FLOAT _r = (FLOAT)rgbRed / 255.f;
		FLOAT _g = (FLOAT)rgbGreen / 255.f;
		FLOAT _b = (FLOAT)rgbBlue / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f) {
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	_RGBQUAD hsl2rgb(HSL hsl) {
		_RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f) {
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant) {
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.r = (BYTE)(r * 255.f);
		rgb.g = (BYTE)(g * 255.f);
		rgb.b = (BYTE)(b * 255.f);

		return rgb;
	}
}

int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //credits to Void_/GetMBR again
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}

COLORREF GetColor(bool toggle) {
	return toggle ? RGB(255, 0, 0) : RGB(255, 255, 255); // Red or White
}


DWORD WINAPI shader1(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h;  // Use a top-down DIB
	bmi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbScreen = nullptr;
	HBITMAP hbmTemp = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, NULL, 0);
	SelectObject(hdcMem, hbmTemp);

	INT verticalOffset = 0; // Initial vertical offset

	for (;;) {
		// Capture the screen to hdcMem
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

		// Modify the pixels in the DIB section with downward motion
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w;
			INT y = (i / w + verticalOffset) % h;  // Apply the vertical offset, wrapping around

												   // Rainbow effect
			float hue = (x + y) * 0.01f; // Adjust 0.01 factor for speed of color change
			rgbScreen[i].rgbRed = (BYTE)(sin(hue) * 127 + 128);    // Cycle red
			rgbScreen[i].rgbGreen = (BYTE)(sin(hue + 2) * 127 + 128); // Offset for green
			rgbScreen[i].rgbBlue = (BYTE)(sin(hue + 4) * 127 + 128);  // Offset for blue
		}

		// Display the modified image
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

		// Increase the vertical offset to move down over time
		verticalOffset = (verticalOffset + 1) % h;

		// Small delay to prevent excessive CPU usage
		Sleep(10);
	}

	// Cleanup (this code will never be reached due to the infinite loop)
	DeleteObject(hbmTemp);
	DeleteDC(hdcMem);
	ReleaseDC(0, hdcScreen);

	return 0;
}

DWORD WINAPI shader2(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h;  // Use a top-down DIB
	bmi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbScreen = nullptr;
	HBITMAP hbmTemp = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, NULL, 0);
	SelectObject(hdcMem, hbmTemp);

	INT verticalOffset = 0; // Initial vertical offset for upward motion

	for (;;) {
		// Capture the screen to hdcMem
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

		// Apply XOR fractal pattern with upward motion
		for (INT y = 0; y < h; y++) {
			for (INT x = 0; x < w; x++) {
				// Calculate the color based on XOR operation
				BYTE color = (BYTE)((x ^ (y + verticalOffset)) & 0xFF);

				// Set the red channel to color and make it red and white
				rgbScreen[y * w + x].rgbRed = color;
				rgbScreen[y * w + x].rgbGreen = color / 2;  // Slightly adjust green to add a softer red
				rgbScreen[y * w + x].rgbBlue = color / 2;   // Slightly adjust blue to add a softer red
			}
		}

		// Display the modified image
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

		// Increase the vertical offset to move upward over time
		verticalOffset = (verticalOffset + 1) % h;

		// Small delay to prevent excessive CPU usage
		Sleep(10);
	}

	// Cleanup (this code will never be reached due to the infinite loop)
	DeleteObject(hbmTemp);
	DeleteDC(hdcMem);
	ReleaseDC(0, hdcScreen);

	return 0;
}


DWORD WINAPI shader3(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(SM_CXSCREEN);
	INT h = GetSystemMetrics(SM_CYSCREEN);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h; // Top-down DIB
	bmi.bmiHeader.biCompression = BI_RGB;

	PRGBQUAD rgbScreen = nullptr;
	HBITMAP hbmTemp = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, NULL, 0);
	SelectObject(hdcMem, hbmTemp);

	INT offsetY = 0; // Initial vertical offset

	for (;;) {
		// Capture the screen to hdcMem
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

		// Apply XOR fractal with rainbow color effect
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w;
			INT y = (i / w + offsetY) % h;

			// XOR fractal calculation
			INT fractalValue = x ^ y;

			// Calculate hue based on fractal value and position
			float hue = fractalValue * 0.02f; // Adjust the factor for color cycle speed
			rgbScreen[i].r = (BYTE)(sin(hue) * 127 + 128);    // Cycle red
			rgbScreen[i].g = (BYTE)(sin(hue + 2) * 127 + 128); // Offset for green
			rgbScreen[i].b = (BYTE)(sin(hue + 4) * 127 + 128);  // Offset for blue
		}

		// Display the modified image with rainbow fractal effect
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

		// Move the fractal pattern up over time
		offsetY = (offsetY + 1) % h;

		// Small delay to prevent excessive CPU usage
		Sleep(10);
	}

	// Cleanup (this code will never be reached due to the infinite loop)
	DeleteObject(hbmTemp);
	DeleteDC(hdcMem);
	ReleaseDC(0, hdcScreen);

	return 0;
}

DWORD WINAPI textout1(LPVOID lpvd)
{
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	LPCSTR text1 = "Insanity.exe";
	LPCSTR text2 = "Regret what you did.";

	while (1)
	{
		HDC hdc = GetDC(0);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 255));

		// Create a random angle for text rotation and font
		HFONT font = CreateFontA(
			43, 0, // Height and Width (Width set to 0 for auto)
			rand() % 360, // Random rotation angle for text
			0, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			"Arial"); // Changed to Arial for readability

		SelectObject(hdc, font);

		// Display first line of text at random position
		TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));

		// Display second line of text at another random position
		TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));

		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(50); // Slightly longer delay to reduce CPU usage
	}
}

DWORD WINAPI bouncingTriangle(LPVOID lpParam)
{
	HDC hdcScreen = GetDC(0);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Triangle vertices
	POINT triangle[3] = { { 0, 0 },{ 30, 60 },{ 60, 0 } };
	int x = 100, y = 100;             // Starting position
	int dx = 5, dy = 5;               // Speed in x and y direction
	int size = 60;                    // Size of the triangle

	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));  // Red color for the triangle

	while (1)
	{
		// Clear the previous triangle by drawing over it with a white brush
		SelectObject(hdcScreen, GetStockObject(WHITE_BRUSH));
		Polygon(hdcScreen, triangle, 3);

		// Update the triangle's position
		x += dx;
		y += dy;

		// Update triangle vertices based on new position
		triangle[0] = { x, y };
		triangle[1] = { x + size / 2, y + size };
		triangle[2] = { x + size, y };

		// Bounce off the edges
		if (x <= 0 || x + size >= screenWidth)
			dx = -dx;
		if (y <= 0 || y + size >= screenHeight)
			dy = -dy;

		// Draw the triangle in the new position
		SelectObject(hdcScreen, brush);
		Polygon(hdcScreen, triangle, 3);

		Sleep(10);  // Delay for smooth movement
	}

	// Cleanup (will never reach here due to infinite loop)
	DeleteObject(brush);
	ReleaseDC(0, hdcScreen);

	return 0;
}

DWORD WINAPI blur(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	BITMAPINFO bmpi = { 0 };
	BLENDFUNCTION blur;
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, 0, NULL, 0);
	SelectObject(dcCopy, bmp);

	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 10;

	int offsetX = 0; // Initial offset for leftward movement

	while (1) {
		hdc = GetDC(0);

		// StretchBlt with a horizontal offset to create a leftward motion effect
		StretchBlt(dcCopy, offsetX - 20, -20, w + 40, h + 40, hdc, 0, 0, w, h, SRCCOPY);
		AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);

		// Update the offset for continuous leftward movement
		offsetX = (offsetX - 1) % w;

		ReleaseDC(0, hdc);

		// Small delay to control the speed of motion
		Sleep(10);
	}
}


DWORD WINAPI shader4(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	_RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;

	while (1) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
		_RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++) {
			for (int y = 0; y < screenHeight; y++) {
				int index = y * screenWidth + x;
				FLOAT fx = ((x + (10 * i)) ^ y) + (i + i * 10);

				rgbquadCopy = rgbquad[index];

				// Convert RGB to HSL
				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				// Fix hue to red and adjust lightness dynamically
				hslcolor.h = 0.0f; // Fixed hue for red
				hslcolor.s = 1.0f; // Full saturation
				hslcolor.l = 0.5f + 0.5f * sin(fx / 100.0f); // Transition between red and white

															 // Convert HSL back to RGB
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		// Display the modified frame
		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}


DWORD WINAPI cur(LPVOID lpParam) {
	POINT cursor;
	while (1) {
		INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
		int X = rand() % w;
		int Y = rand() % h;
		SetCursorPos(X, Y);
		HDC hdc = GetDC(0);
		DrawIcon(hdc, X, Y, LoadCursor(0, IDC_ARROW));
		ReleaseDC(0, hdc);
	}
}

LPCWSTR generateRandomText(int len) {
	wchar_t* ustr = new(std::nothrow) wchar_t[len + 1]; // +1 for '\0'
	if (ustr == nullptr) {
		return L""; // If allocation fails, return an empty string
	}

	for (int i = 0; i < len; i++) {
		// Randomly pick between generating a letter (A-Z) or a number (1-10)
		if (rand() % 2 == 0) {
			ustr[i] = L'A' + (rand() % 26); // Letter (A-Z)
		}
		else {
			ustr[i] = L'1' + (rand() % 10); // Number (1-10)
		}
	}

	ustr[len] = L'\0'; // Null-terminate the string
	return ustr;
}

// Callback to set text for windows
BOOL CALLBACK SetTextProc(HWND hwnd, LPARAM lParam) {
	// Ensure the window is visible and can receive text
	if (IsWindowVisible(hwnd)) {
		// Generate a random string of length between 10 and 20
		LPCWSTR randomText = generateRandomText(rand() % 11 + 10); // Length: 10–20

																   // Set the text to the window
		SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)randomText, SMTO_ABORTIFHUNG, 100, NULL);

		// Free the dynamically allocated string
		delete[] randomText;
	}

	return TRUE;
}

// Callback to enumerate top-level windows and their children
BOOL CALLBACK EnumTopLevelProc(HWND hwnd, LPARAM lParam) {
	// Set text for the top-level window
	SetTextProc(hwnd, lParam);

	// Enumerate and set text for child windows
	EnumChildWindows(hwnd, SetTextProc, lParam);

	return TRUE;
}

// Thread function to enumerate all windows
DWORD WINAPI textz(LPVOID lpParam) {
	while (true) {
		EnumWindows(EnumTopLevelProc, NULL); // Enumerate all top-level windows
		Sleep(0); // Add a small delay to avoid excessive CPU usage
	}
	return 0;
}

DWORD WINAPI triangles(LPVOID lpParam) {
	srand(static_cast<unsigned>(time(nullptr))); // Seed random number generator

	bool toggle = false; // Toggle between red and white

	while (true) {
		int w = GetSystemMetrics(SM_CXSCREEN); // Screen width
		int h = GetSystemMetrics(SM_CYSCREEN); // Screen height

		HDC hdc = GetDC(0); // Get the device context for the entire screen

							// Determine the color based on the toggle
		COLORREF currentColor = GetColor(toggle);

		// Create and select a pen
		HPEN hPen = CreatePen(PS_SOLID, 2, currentColor);
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

		// Create and select a brush
		HBRUSH hBrush = CreateSolidBrush(currentColor);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		// Define triangle vertices with random positions
		POINT vertices[3] = {
			{ rand() % w, rand() % h },
			{ rand() % w, rand() % h },
			{ rand() % w, rand() % h }
		};

		// Draw the triangle
		Polygon(hdc, vertices, 3);

		// Cleanup GDI objects
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);

		SelectObject(hdc, hOldPen);
		DeleteObject(hPen);

		ReleaseDC(0, hdc); // Release the device context

						   // Toggle the color for the next triangle
		toggle = !toggle;

		Sleep(10); // Small delay to reduce CPU usage
	}

	return 0;
}

DWORD WINAPI shader5(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = nullptr;

	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h; // Top-down DIB section
	bmi.bmiHeader.biCompression = BI_RGB;

	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, NULL, 0);
	SelectObject(hdcMem, hbmTemp);

	for (;;) {
		// Capture the screen
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

		// Apply grayscale effect
		for (INT i = 0; i < w * h; i++) {
			BYTE red = rgbScreen[i].r;
			BYTE green = rgbScreen[i].g;
			BYTE blue = rgbScreen[i].b;

			// Calculate luminance using standard weights for grayscale
			BYTE luminance = static_cast<BYTE>(0.299 * red + 0.587 * green + 0.114 * blue);

			// Set all color channels to the luminance value
			rgbScreen[i].r = luminance;
			rgbScreen[i].g = luminance;
			rgbScreen[i].b = luminance;
		}

		// Render the modified image back to the screen
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

		// Reduce CPU usage
		Sleep(10);
	}

	// Cleanup (though this code is unreachable due to the infinite loop)
	DeleteObject(hbmTemp);
	DeleteDC(hdcMem);
	ReleaseDC(0, hdcScreen);

	return 0;
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t + (t&t >> 8)*(~t >> 3));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t*(t&t >> 8)&(t >> 0 & t * 6));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t^t*(t&t >> 4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(500 * (t*t >> 6));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t*(t | t >> 5) ^ (t >> 6 & 3));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22000, 22000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t << 2 ^ t >> 4 ^ t << 4 & t >> 4 | t << 2 & t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((100 * (t&t >> 5)) + t >> 3);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}


int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"Run Short GDI Only?", L"Insanity-safety.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"Are you sure?", L"Last Warning.", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			Sleep(3000);
			CreateThread(0, 0, textz, 0, 0, 0);
			HANDLE threadstarter = CreateThread(0, 0, shader1, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(threadstarter, 0);
			CloseHandle(threadstarter);
			HANDLE thread1 = CreateThread(0, 0, shader2, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			HANDLE thread2 = CreateThread(0, 0, shader3, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			HANDLE thread3 = CreateThread(0, 0, blur, 0, 0, 0);
			HANDLE thread3dot2 = CreateThread(0, 0, textout1, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, bouncingTriangle, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			HANDLE thread4 = CreateThread(0, 0, shader4, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			HANDLE thread5 = CreateThread(0, 0, triangles, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			HANDLE thread6 = CreateThread(0, 0, shader5, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
		}
	}
}
