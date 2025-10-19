#pragma once

namespace gdis1 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get screen dimensions
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Get screen device context
        HDC hScreenDC = GetDC(0);

        // Create a memory device context compatible with screen
        HDC hMemDC = CreateCompatibleDC(hScreenDC);

        // Define BITMAPINFO for 24-bit RGB
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;  // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 24;  // RGBTRIPLE-like
        bmi.bmiHeader.biCompression = BI_RGB;

        // Pointer to pixel buffer
        void* pixels = nullptr;

        // Create a DIBSection so we can write directly to pixel memory
        HBITMAP hBitmap = CreateDIBSection(hMemDC, &bmi, DIB_RGB_COLORS, &pixels, 0, 0);
        SelectObject(hMemDC, hBitmap);

        // Animation time variable
        float t = 0.0f;

        while (1) {
            RGBTRIPLE* pixel = (RGBTRIPLE*)pixels;

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    // Plasma formula (can be customized)
                    BYTE r = (BYTE)(128 + 127 * sin(x / 40.0 + t));
                    BYTE g = (BYTE)(128 + 127 * sin(y / 30.0 + t));
                    BYTE b = (BYTE)(128 + 127 * sin((x + y) / 50.0 + t));

                    // Write pixel (note: RGBTRIPLE is BGR)
                    pixel->rgbtRed = r;
                    pixel->rgbtGreen = g;
                    pixel->rgbtBlue = b;
                    pixel++;
                }
            }

            // BitBlt using SRCERASE raster op (weird effect)
            BitBlt(hScreenDC, 0, 0, w, h, hMemDC, 0, 0, SRCERASE);

            // Increment time
            t += 0.05f;

            // Slight delay to reduce CPU load
            Sleep(10);
        }

        // Cleanup (unreachable due to while(1))
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(0, hScreenDC);
        return 0;
    }
}
namespace gdis2 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Screen device context
        HDC hdcScreen = GetDC(NULL);

        // Create compatible DC and bitmap
        HDC memDC = CreateCompatibleDC(hdcScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, w, h);
        SelectObject(memDC, hBitmap);

        // Set up pixel buffer using DIB
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 24;
        bmi.bmiHeader.biCompression = BI_RGB;

        // Pixel buffer (RGBTRIPLE-style: 3 bytes per pixel)
        unsigned char* buffer = new unsigned char[w * h * 3];

        float t = 0.0f;

        while (1) {
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    float fx = (float)x / w;
                    float fy = (float)y / h;

                    // RGB sine wave & ripple effect
                    float ripple = sinf((x * x + y * y) * 0.00005f - t);
                    unsigned char r = (unsigned char)(127 + 127 * sinf(fx * 10.0f + t));
                    unsigned char g = (unsigned char)(127 + 127 * sinf(fy * 10.0f + t + 2.0f));
                    unsigned char b = (unsigned char)(127 + 127 * ripple);

                    int i = (y * w + x) * 3;
                    buffer[i + 0] = b;
                    buffer[i + 1] = g;
                    buffer[i + 2] = r;
                }
            }

            // Draw buffer to memory DC
            SetDIBitsToDevice(memDC, 0, 0, w, h, 0, 0, 0, h, buffer, &bmi, DIB_RGB_COLORS);

            // BitBlt effects to screen
            BitBlt(hdcScreen, 0, 0, w, h, memDC, 0, 0, SRCCOPY);     // Normal copy
            BitBlt(hdcScreen, 0, 0, w, h, memDC, 0, 0, NOTSRCCOPY);  // Inverted effect
            BitBlt(hdcScreen, 0, 0, w, h, memDC, 0, 0, SRCPAINT);    // Paint over

            t += 0.03f; // animate over time
            Sleep(1);   // sleep to avoid 100% CPU
        }

        // (Unreachable cleanup)
        delete[] buffer;
        DeleteObject(hBitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdis3 {
    inline BYTE xor8(BYTE a, BYTE b) {
        return a ^ b;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get full screen resolution
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Get screen device context
        HDC hdcScreen = GetDC(NULL);

        // Create memory device context and bitmap
        HDC memDC = CreateCompatibleDC(hdcScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, w, h);
        SelectObject(memDC, hBitmap);

        // RGB pixel buffer
        unsigned char* buffer = new unsigned char[w * h * 3];

        // Set up DIB structure
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;  // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 24; // RGB
        bmi.bmiHeader.biCompression = BI_RGB;

        float t = 0.0f;

        while (1) {
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    int dx = x - w / 2;
                    int dy = y - h / 2;
                    float dist = sqrtf((float)(dx * dx + dy * dy));

                    // Ripple + rotating cube pattern
                    float ripple = sinf(dist * 0.03f - t);
                    float cubeX = sinf(t + x * 0.01f) * 0.5f + 0.5f;
                    float cubeY = cosf(t + y * 0.01f) * 0.5f + 0.5f;

                    // XOR-style RGB pattern
                    BYTE r = xor8((BYTE)(cubeX * 255), (BYTE)(ripple * 127 + 128));
                    BYTE g = xor8((BYTE)(cubeY * 255), (BYTE)(ripple * 127 + 128));
                    BYTE b = xor8((BYTE)(255 - ripple * 128), (BYTE)(cubeX * cubeY * 255));

                    int i = (y * w + x) * 3;
                    buffer[i + 0] = b;
                    buffer[i + 1] = g;
                    buffer[i + 2] = r;
                }
            }

            // Copy pixel buffer to memory DC
            SetDIBitsToDevice(memDC, 0, 0, w, h, 0, 0, 0, h, buffer, &bmi, DIB_RGB_COLORS);

            // Blit to screen using SRCCOPY only
            BitBlt(hdcScreen, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

            // Animate time
            t += 0.025f;

            // Sleep for smoother CPU usage
            Sleep(1);
        }

        // Cleanup (never reached)
        delete[] buffer;
        DeleteObject(hBitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdis4 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Seed random
        srand((unsigned)time(NULL));

        // Get full screen dimensions
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Get desktop DC (full screen)
        HDC hdcScreen = GetDC(NULL);
        if (!hdcScreen) return 1;

        // Create compatible DC and bitmap for manipulation
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HBITMAP hbmMem = CreateCompatibleBitmap(hdcScreen, width, height);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hbmMem);

        POINT plg[3]; // For PlgBlt
        RECT rectFull = { 0, 0, width, height };

        while (1) {
            // 1. Fill with random flashing TRGBQUAD colors
            for (int y = 0; y < height; y += 10) {
                for (int x = 0; x < width; x += 10) {
                    COLORREF col = RGB(rand() % 256, rand() % 256, rand() % 256);
                    HBRUSH hBrush = CreateSolidBrush(col);
                    RECT rc = { x, y, x + 10, y + 10 };
                    FillRect(hdcMem, &rc, hBrush);
                    DeleteObject(hBrush);
                }
            }

            // 2. Fast shaking screen effect: shift points randomly by ±5 pixels
            int shakeRange = 5;
            plg[0].x = shakeRange - rand() % (shakeRange * 2);
            plg[0].y = shakeRange - rand() % (shakeRange * 2);
            plg[1].x = width + (shakeRange - rand() % (shakeRange * 2));
            plg[1].y = 0 + (shakeRange - rand() % (shakeRange * 2));
            plg[2].x = 0 + (shakeRange - rand() % (shakeRange * 2));
            plg[2].y = height + (shakeRange - rand() % (shakeRange * 2));

            // 3. Rotate screen effect: simple rotation around center by shifting points
            // We'll do a simple rotation matrix for the three points
            // Angle increments quickly each loop
            static double angle = 0.0;
            angle += 0.05; // rotate by small step

            // Center of screen
            double cx = width / 2.0;
            double cy = height / 2.0;

            // Define original points (corners of rect)
            POINT orig[3] = { {0,0}, {width,0}, {0,height} };

            for (int i = 0; i < 3; ++i) {
                double dx = orig[i].x - cx;
                double dy = orig[i].y - cy;
                plg[i].x = (LONG)(cx + dx * cos(angle) - dy * sin(angle));
                plg[i].y = (LONG)(cy + dx * sin(angle) + dy * cos(angle));
            }

            // 4. Use PlgBlt to blit rotated image to screen DC
            // First copy our memDC bitmap to screen
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

            // Then PlgBlt with our plg points, from screen to screen
            // This causes rotation and shaking effect
            PlgBlt(hdcScreen, plg, hdcScreen, 0, 0, width, height, NULL, 0, 0);

            // 5. Use BitBlt with NOTSRCCOPY + SRCAND for funky effects
            // First invert screen colors with NOTSRCCOPY
            BitBlt(hdcScreen, 0, 0, width, height, hdcScreen, 0, 0, NOTSRCCOPY);
            // Then AND with memDC (random colors)
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCAND);

            // Sleep 1 ms
            Sleep(1);
        }

        // Cleanup - never reached due to infinite loop
        SelectObject(hdcMem, hOldBitmap);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
    DWORD WINAPI RGDI2(LPVOID lpParam)
    {
        // Get the full screen size
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Get the desktop device context
        HDC hdcScreen = GetDC(NULL);

        // Create a memory DC compatible with the screen
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        // Create a bitmap to draw TRGBQUAD data
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pvBits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
        SelectObject(hdcMem, hBitmap);

        srand((unsigned)time(NULL));

        while (1) {
            // Fill the bitmap with random flashing colors (TRGBQUAD)
            DWORD* pixels = (DWORD*)pvBits;
            for (int i = 0; i < width * height; i++) {
                // Random TRGBQUAD with alpha = 0xFF (opaque)
                BYTE r = rand() % 256;
                BYTE g = rand() % 256;
                BYTE b = rand() % 256;
                pixels[i] = (0xFF << 24) | (r << 16) | (g << 8) | b;
            }

            // Fast shaking: random offset between -10 and +10 pixels
            int shakeX = (rand() % 21) - 10;
            int shakeY = (rand() % 21) - 10;

            // BitBlt with NOTSRCCOPY (0x00330008) and SRCAND (0x008800C6)
            // We can combine NOTSRCCOPY and SRCAND by doing two BitBlt calls
            // Because BitBlt only takes one raster op at a time

            // First, NOTSRCCOPY from memDC to screen
            BitBlt(hdcScreen, shakeX, shakeY, width, height, hdcMem, 0, 0, NOTSRCCOPY);

            // Then SRCAND with the same bitmap (mask effect)
            BitBlt(hdcScreen, shakeX, shakeY, width, height, hdcMem, 0, 0, SRCAND);

            Sleep(1);
        }

        // Cleanup (never reached)
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
