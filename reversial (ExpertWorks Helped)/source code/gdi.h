#pragma once
// credits for ChatGPT Ai For A Make GDI
DWORD WINAPI iamohioball(LPVOID lpParam)
{
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    // Base bouncing position
    int rx = rand() % sw;
    int ry = rand() % sh;

    int signX = 1;
    int signY = 1;
    int speed = 8;
    int radius = 10;

    while (1)
    {
        HDC desk = GetDC(0);

        // Update bounce position
        rx += speed * signX;
        ry += speed * signY;

        // Screen collision
        if (rx <= 0 || rx >= sw) signX *= -1;
        if (ry <= 0 || ry >= sh) signY *= -1;

        // Draw trail using your original math
        for (int t = 0; t < 120; ++t)
        {
            int x = (int)(radius + t * tan(t + radius * 3) + rx);
            int y = (int)(radius + t * cos(t + radius * 3) + ry);

            if (rand() % 5 == 0)
                DrawIcon(desk, x, y, LoadIcon(0, IDI_ERROR));
            if (rand() % 5 == 0)
                DrawIcon(desk, x, y, LoadIcon(0, IDI_WARNING));
            if (rand() % 5 == 0)
                DrawIcon(desk, x, y, LoadIcon(0, IDI_INFORMATION));
            if (rand() % 5 == 0)
                DrawIcon(desk, x, y, LoadIcon(0, IDI_ASTERISK));
        }

        ReleaseDC(0, desk);
        Sleep(16); // ~60 FPS
    }
}
namespace p1 {
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE r;
            BYTE g;
            BYTE b;
            BYTE a;
        };
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w >> 1;
        int hs = h >> 1;

        HDC dc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(dc);

        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* buffer = 0;
        HBITMAP dib = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&buffer, NULL, 0);
        SelectObject(memDC, dib);

        int i = 0;

        while (1)
        {
            StretchBlt(
                memDC, 0, 0, ws, hs,
                dc, 0, 0, w, h,
                SRCCOPY
            );

            for (int y = 0; y < hs; y++)
            {
                int yw = y * ws;
                for (int x = 0; x < ws; x++)
                {
                    int index = yw + x;

                    int code = (int)(
                        (8 * i) + ((8 * i) * sin(x / 32.0)) +
                        (4 * i) + ((4 * i) * sin(y / 24.0))
                        );

                    buffer[index].rgborg +=
                        (code / 4) << (code / 1024);
                }
            }

            StretchBlt(
                dc, 0, 0, w, h,
                memDC, 0, 0, ws, hs,
                SRCCOPY
            );

            i++;
            Sleep(1);
        }

        DeleteObject(dib);
        DeleteDC(memDC);
        ReleaseDC(NULL, dc);
        return 0;
    }
    DWORD WINAPI GDI2(LPVOID lpParam)
    {

        HDC hdc = GetDC(0);
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        int t = 0;

        while (true)
        {
            hdc = GetDC(0);

            // Wave motion
            int waveX = (int)(sin(t * 0.15) * 30);
            int waveY = (int)(cos(t * 0.10) * 30);

            // Random glitch slice
            int sliceW = rand() % 200 + 50;
            int sliceH = rand() % 200 + 50;
            int x = rand() % (sw - sliceW);
            int y = rand() % (sh - sliceH);

            // Kaleidoscope mirror effect
            StretchBlt(
                hdc,
                x + waveX, y + waveY,
                sliceW, sliceH,
                hdc,
                x, y,
                -sliceW, sliceH,
                SRCCOPY
            );

            StretchBlt(
                hdc,
                x - waveX, y - waveY,
                sliceW, sliceH,
                hdc,
                x, y,
                sliceW, -sliceH,
                SRCCOPY
            );

            // Fast horizontal wave glitch
            BitBlt(
                hdc,
                waveX, y,
                sw, sliceH,
                hdc,
                0, y,
                SRCCOPY
            );

            Sleep(5); // fast glitch speed
            ReleaseDC(0, hdc);
            t++;
        }
    }
}
namespace p2 {
    typedef union _TRGBQUAD {
        COLORREF rgb;
        struct {
            BYTE b;
            BYTE g;
            BYTE r;
            BYTE a;
        };
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        HDC hdc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(hdc);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;

        HBITMAP dib = CreateDIBSection(
            hdc,
            &bmi,
            DIB_RGB_COLORS,
            (void**)&pixels,
            NULL,
            0
        );

        SelectObject(memDC, dib);

        int ii = 0;

        while (true)
        {
            StretchBlt(memDC, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

#pragma omp parallel for
            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int i = y * ws + x;
                    int j = x ^ y;

                    int v =
                        (int)(
                            j + (j * sin(x / 16.0)) +
                            j + (j * sin(y / 8.0)) +
                            j + (j * sin((x + y) / 16.0)) +
                            j + (j * sin(sqrt((double)(x * x + y * y)) / 8.0))
                            ) / 4;

                    v = (v + w * ii * 2) >> 9;

                    pixels[i].r += 10 + v;
                    pixels[i].g += v;
                    pixels[i].b += 5;
                }
            }

            ii++;

            StretchBlt(hdc, 0, 0, w, h, memDC, 0, 0, ws, hs, SRCCOPY);
            Sleep(1);
        }

        DeleteObject(dib);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdc);

        return 0;
    }
}
namespace p3 {
    typedef struct { float h, s, v; } HSV;

    namespace Colors {
        RGBQUAD HSVtoRGB(HSV hsv) {
            int hi = (int)floor(hsv.h / 60.0) % 6;
            double f = hsv.h / 60.0 - floor(hsv.h / 60.0);
            double p = hsv.v * (1.0 - hsv.s);
            double q = hsv.v * (1.0 - f * hsv.s);
            double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);

            RGBQUAD rgb = {};
            switch (hi) {
            case 0: rgb.rgbRed = (BYTE)(hsv.v * 255); rgb.rgbGreen = (BYTE)(t * 255); rgb.rgbBlue = (BYTE)(p * 255); break;
            case 1: rgb.rgbRed = (BYTE)(q * 255); rgb.rgbGreen = (BYTE)(hsv.v * 255); rgb.rgbBlue = (BYTE)(p * 255); break;
            case 2: rgb.rgbRed = (BYTE)(p * 255); rgb.rgbGreen = (BYTE)(hsv.v * 255); rgb.rgbBlue = (BYTE)(t * 255); break;
            case 3: rgb.rgbRed = (BYTE)(p * 255); rgb.rgbGreen = (BYTE)(q * 255); rgb.rgbBlue = (BYTE)(hsv.v * 255); break;
            case 4: rgb.rgbRed = (BYTE)(t * 255); rgb.rgbGreen = (BYTE)(p * 255); rgb.rgbBlue = (BYTE)(hsv.v * 255); break;
            default: rgb.rgbRed = (BYTE)(hsv.v * 255); rgb.rgbGreen = (BYTE)(p * 255); rgb.rgbBlue = (BYTE)(q * 255); break;
            }
            rgb.rgbReserved = 0;
            return rgb;
        }

        HSV RGBtoHSV(RGBQUAD rgb) {
            HSV hsv;
            double r = rgb.rgbRed / 255.0;
            double g = rgb.rgbGreen / 255.0;
            double b = rgb.rgbBlue / 255.0;
            double cmax = max(max(r, g), b);
            double cmin = min(min(r, g), b);
            double delta = cmax - cmin;
            hsv.v = cmax;
            hsv.s = cmax == 0 ? 0 : delta / cmax;

            if (delta == 0) hsv.h = 0;
            else if (cmax == r) hsv.h = 60.0 * fmod((g - b) / delta, 6.0);
            else if (cmax == g) hsv.h = 60.0 * ((b - r) / delta + 2.0);
            else hsv.h = 60.0 * ((r - g) / delta + 4.0);

            if (hsv.h < 0) hsv.h += 360.0;
            return hsv;
        }
    }

    DWORD WINAPI GDI(LPVOID lpParam)
    {
        HDC dc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC dcCopy = CreateCompatibleDC(dc);
        int ws = w / 2, hs = h / 2;
        BITMAPINFO bmpi = {};
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = -hs; // top-down
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        int i = 0;
        srand((unsigned int)time(NULL));

        while (true) {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            int rw = rand() % ws;
            int rh = rand() % hs;
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int idx = y * ws + x;
                    int cx = abs(x - rw / 2), cy = abs(y - rh / 2);
                    double fx = 64 + 64 * cos(sqrt(cx * cx + cy * cy) / 4.0);
                    double fx2 = 128 + 128 * sin(sqrt(cx * cx + cy * cy) / 8.0);
                    double fx3 = fx + fx2;
                    HSV hsv = Colors::RGBtoHSV(rgbquad[idx]);
                    hsv.h = fmod(fx3 + i, 360.0);
                    rgbquad[idx] = Colors::HSVtoRGB(hsv);
                }
            }

            i++;
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
            Sleep(1); // fast screen update
        }

        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);
        return 0x00;
    }
    DWORD WINAPI GDI2(LPVOID lpParam)
    {
        HDC desk;
        int sw, sh;

        int offset = 0;
        int dir = 1;   // direction of shake

        while (1) {
            desk = GetDC(0);
            sw = GetSystemMetrics(SM_CXSCREEN);
            sh = GetSystemMetrics(SM_CYSCREEN);

            // Vertical shake logic
            offset += dir;
            if (offset > 20 || offset < -20)
                dir = -dir;

            // Stretch vertically (taller screen effect)
            StretchBlt(
                desk,
                0, offset - 20,      // destination X, Y (shake)
                sw, sh + 40,         // destination width, height (vertical stretch)
                desk,
                0, 0,                // source X, Y
                sw, sh,              // source width, height
                SRCCOPY
            );

            ReleaseDC(0, desk);
            Sleep(10); // control speed
        }
    }
}
namespace p4 {

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
            FLOAT r = rgb.rgbRed / 255.f;
            FLOAT g = rgb.rgbGreen / 255.f;
            FLOAT b = rgb.rgbBlue / 255.f;

            FLOAT maxc = max(r, max(g, b));
            FLOAT minc = min(r, min(g, b));
            FLOAT delta = maxc - minc;

            HSL hsl{};
            hsl.l = (maxc + minc) * 0.5f;

            if (delta != 0.f)
            {
                hsl.s = (hsl.l < 0.5f) ? (delta / (maxc + minc))
                    : (delta / (2.f - maxc - minc));

                FLOAT dr = (((maxc - r) / 6.f) + (delta * 0.5f)) / delta;
                FLOAT dg = (((maxc - g) / 6.f) + (delta * 0.5f)) / delta;
                FLOAT db = (((maxc - b) / 6.f) + (delta * 0.5f)) / delta;

                if (r == maxc)      hsl.h = db - dg;
                else if (g == maxc) hsl.h = (1.f / 3.f) + dr - db;
                else                hsl.h = (2.f / 3.f) + dg - dr;

                if (hsl.h < 0.f) hsl.h += 1.f;
                if (hsl.h > 1.f) hsl.h -= 1.f;
            }

            return hsl;
        }

        __forceinline RGBQUAD hsl2rgb(const HSL& hsl)
        {
            FLOAT r = hsl.l;
            FLOAT g = hsl.l;
            FLOAT b = hsl.l;

            if (hsl.s != 0.f)
            {
                FLOAT v = (hsl.l <= 0.5f)
                    ? (hsl.l * (1.f + hsl.s))
                    : (hsl.l + hsl.s - hsl.l * hsl.s);

                FLOAT m = hsl.l * 2.f - v;
                FLOAT sv = (v - m) / v;

                FLOAT h = hsl.h * 6.f;
                INT sextant = (INT)h;
                FLOAT fract = h - sextant;
                FLOAT vsf = v * sv * fract;

                FLOAT mid1 = m + vsf;
                FLOAT mid2 = v - vsf;

                switch (sextant)
                {
                case 0: r = v;    g = mid1; b = m;    break;
                case 1: r = mid2; g = v;    b = m;    break;
                case 2: r = m;    g = v;    b = mid1; break;
                case 3: r = m;    g = mid2; b = v;    break;
                case 4: r = mid1; g = m;    b = v;    break;
                default:r = v;    g = m;    b = mid2; break;
                }
            }

            RGBQUAD rgb;
            rgb.rgbRed = (BYTE)(r * 255.f);
            rgb.rgbGreen = (BYTE)(g * 255.f);
            rgb.rgbBlue = (BYTE)(b * 255.f);
            rgb.rgbReserved = 0;
            return rgb;
        }
    }
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        HDC hdc = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);

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
        HBITMAP bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&rgbq, NULL, 0);
        SelectObject(hdcMem, bmp);

        INT i = 0;

        while (1)
        {
            BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            FLOAT angle = i * 0.01f;
            FLOAT ca = cosf(angle);
            FLOAT sa = sinf(angle);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int index = y * w + x;

                    int cx = abs(x - (w >> 1));
                    int cy = abs(y - (h >> 1));

                    int zx = (int)(ca * cx - sa * cy);
                    int zy = (int)(sa * cx + ca * cy);

                    int fx = (int)(
                        (8 * i) + ((8 * i) * sin(zx + i / 32.0)) +
                        (4 * i) + ((4 * i) * sin(zy + i / 24.0))
                        );

                    HSL hsl = Colors::rgb2hsl(rgbq[index]);
                    hsl.h = fmodf((fx / 500.f) + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    rgbq[index] = Colors::hsl2rgb(hsl);
                }
            }

            i++;
            BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

            ReleaseDC(NULL, hdc);
            DeleteDC(hdc);
            hdc = GetDC(NULL);
        }

        return 0x00;
    }
}
namespace p5 {
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE r;
            BYTE g;
            BYTE b;
            BYTE a;
        };
    } TRGBQUAD, * PTRGBQUAD;

    DWORD WINAPI GDI(LPVOID lpParam)
    {

        HDC dc = GetDC(NULL);
        HDC dcMem = CreateCompatibleDC(dc);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(dcMem, bmp);

        int i = 0;
        int ii = 1;

        while (true)
        {
            StretchBlt(dcMem, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

#pragma omp parallel for
            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    FLOAT fx = (FLOAT)(sin(x / 500.f - y / (FLOAT)h * 0.1f) + i / 5.f);
                    FLOAT fx2 = (FLOAT)(cos(y / 500.f - x / (FLOAT)w * 0.1f) + i / 5.f);
                    FLOAT fx3 = (FLOAT)(sin(x / 500.f - y / (FLOAT)h * 0.1f) + i / 5.f);

                    FLOAT fx4 = (fx + fx2 + fx3);

                    int code = (int)(fx4 * ii);

                    pixels[index].rgborg += (code >> 2) << (code & 1023);
                }
            }

            StretchBlt(dc, 0, 0, w, h, dcMem, 0, 0, ws, hs, SRCCOPY);

            i++;
            ii += (i & 1);

            Sleep(1);
        }

        return 0;
    }
}
namespace p6 {
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE r;
            BYTE g;
            BYTE b;
            BYTE Reserved;
        };
    } TRGBQUAD, * PTRGBQUAD;

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    DWORD WINAPI GDI(LPVOID lpParam)
    {

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* trgbquad = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&trgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        int i = 0;
        double angle = 0.0;

        while (true) {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    int cx = abs(x - (ws / 2));
                    int cy = abs(y - (hs / 2));

                    int zx = static_cast<int>(cos(angle) * cx - sin(angle) * cy);
                    int zy = static_cast<int>(sin(angle) * cx + cos(angle) * cy);

                    int fx = (zx + i) + (zy + i);

                    // Example TRGBQUAD "shader effect"
                    trgbquad[index].rgborg += ((fx / 4) << ((fx / 1024) % 24)) & 0x00FFFFFF;
                }
            }

            i++;
            angle += 0.01;

            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
            Sleep(1);
        }

        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);
        DeleteObject(bmp);
        return 0;
    }
}
namespace p7 {
    typedef struct
    {
        FLOAT h;
        FLOAT s;
        FLOAT v;
    } HSV;

    namespace Colors
    {
        RGBQUAD HSVtoRGB(HSV hsv)
        {
            double r = 0, g = 0, b = 0;

            int hi = (int)(hsv.h / 60.0) % 6;
            double f = hsv.h / 60.0 - floor(hsv.h / 60.0);

            double p = hsv.v * (1.0 - hsv.s);
            double q = hsv.v * (1.0 - f * hsv.s);
            double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);

            switch (hi)
            {
            case 0: r = hsv.v; g = t; b = p; break;
            case 1: r = q; g = hsv.v; b = p; break;
            case 2: r = p; g = hsv.v; b = t; break;
            case 3: r = p; g = q; b = hsv.v; break;
            case 4: r = t; g = p; b = hsv.v; break;
            default: r = hsv.v; g = p; b = q; break;
            }

            return {
                (BYTE)(b * 255),
                (BYTE)(g * 255),
                (BYTE)(r * 255),
                0
            };
        }

        HSV RGBtoHSV(RGBQUAD rgb)
        {
            HSV hsv;

            double r = rgb.rgbRed / 255.0;
            double g = rgb.rgbGreen / 255.0;
            double b = rgb.rgbBlue / 255.0;

            double cmax = max(r, max(g, b));
            double cmin = min(r, min(g, b));
            double delta = cmax - cmin;

            hsv.v = cmax;
            hsv.s = (cmax == 0) ? 0 : delta / cmax;

            if (delta == 0)
                hsv.h = 0;
            else if (cmax == r)
                hsv.h = fmod((g - b) / delta, 6.0) * 60.0;
            else if (cmax == g)
                hsv.h = ((b - r) / delta + 2.0) * 60.0;
            else
                hsv.h = ((r - g) / delta + 4.0) * 60.0;

            if (hsv.h < 0) hsv.h += 360.0;
            return hsv;
        }
    }
    DWORD WINAPI GDI(LPVOID lpParam)
    {

        srand((unsigned)time(NULL));

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 2;
        int hs = h / 2;

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(dcCopy, bmp);

        int i = 0;

        while (true)
        {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    FLOAT fx = sin(x / 500.f - y / (FLOAT)h * 0.1f) + i / 5.f;
                    FLOAT fx2 = sin(y / 500.f - x / (FLOAT)w * 0.1f) + i / 5.f;
                    FLOAT fx3 = sin(x / 500.f - y / (FLOAT)h * 0.1f) + i / 5.f;

                    FLOAT fx4 = (fx + fx2 + fx3) * (fx + fx2 + fx3);

                    HSV hsv = Colors::RGBtoHSV(pixels[index]);

                    // Only hue is modified
                    hsv.h = fmod(fx4 * 40.0f + i, 360.0f);

                    pixels[index] = Colors::HSVtoRGB(hsv);
                }
            }

            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            i++;
            Sleep(5); // fast, smooth animation
        }

        return 0x00;
    }

    DWORD WINAPI GDI2(LPVOID lpParam)
    {

        HDC desk = GetDC(NULL);   // get once
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        double time = 0.0;

        while (true)
        {
            for (int y = 0; y < sh; y += 3)
            {
                int offset = (int)(sin(y * 0.05 + time) * 25);

                StretchBlt(
                    desk,
                    offset, y, sw, 3,   // dest
                    desk,
                    0, y, sw, 3,        // src
                    SRCCOPY
                );
            }

            time += 0.12;  // wave speed
            Sleep(5);      // keeps CPU low
        }

        ReleaseDC(NULL, desk);
        return 0;
    }
}
namespace p8 {
    // ================= TRGBQUAD =================
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE r;
            BYTE g;
            BYTE b;
            BYTE a;
        };
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 2;
        int hs = h / 2;

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;
        HBITMAP bmp = CreateDIBSection(
            dc, &bmi, DIB_RGB_COLORS,
            (void**)&pixels, NULL, 0
        );

        SelectObject(dcCopy, bmp);

        INT i = 0;
        DOUBLE angle = 0.0;

        while (true)
        {
            StretchBlt(
                dcCopy, 0, 0, ws, hs,
                dc, 0, 0, w, h,
                SRCCOPY
            );

            INT ii = i >> 2;

#pragma omp parallel for
            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    INT cx = x - (ws >> 1);
                    INT cy = y - (hs >> 1);

                    INT zx = (INT)(cos(angle) * cx - sin(angle) * cy);
                    INT zy = (INT)(sin(angle) * cx + cos(angle) * cy);

                    INT dt = 256 + i;

                    INT fx =
                        dt +
                        (INT)(dt * sin(i + x / 20.0)) +
                        dt +
                        (INT)(dt * cos(i + y / 20.0)) * ii +
                        zx + zy;

                    pixels[index].rgborg += (fx >> 4) << (fx >> 10);
                }
            }

            angle += 0.01;
            i++;

            StretchBlt(
                dc, 0, 0, w, h,
                dcCopy, 0, 0, ws, hs,
                SRCCOPY
            );

            Sleep(1);
        }

        return 0;
    }
}
namespace p9 {
    typedef struct
    {
        FLOAT h;
        FLOAT s;
        FLOAT v;
    } HSV;

    namespace Colors
    {
        RGBQUAD HSVtoRGB(HSV hsv)
        {
            int hi = (int)(hsv.h / 60.0f) % 6;
            float f = hsv.h / 60.0f - floor(hsv.h / 60.0f);
            float p = hsv.v * (1.0f - hsv.s);
            float q = hsv.v * (1.0f - f * hsv.s);
            float t = hsv.v * (1.0f - (1.0f - f) * hsv.s);

            switch (hi)
            {
            case 0: return { (BYTE)(hsv.v * 255), (BYTE)(t * 255), (BYTE)(p * 255), 0 };
            case 1: return { (BYTE)(q * 255), (BYTE)(hsv.v * 255), (BYTE)(p * 255), 0 };
            case 2: return { (BYTE)(p * 255), (BYTE)(hsv.v * 255), (BYTE)(t * 255), 0 };
            case 3: return { (BYTE)(p * 255), (BYTE)(q * 255), (BYTE)(hsv.v * 255), 0 };
            case 4: return { (BYTE)(t * 255), (BYTE)(p * 255), (BYTE)(hsv.v * 255), 0 };
            default:return { (BYTE)(hsv.v * 255), (BYTE)(p * 255), (BYTE)(q * 255), 0 };
            }
        }

        HSV RGBtoHSV(RGBQUAD rgb)
        {
            HSV hsv;
            float r = rgb.rgbRed / 255.0f;
            float g = rgb.rgbGreen / 255.0f;
            float b = rgb.rgbBlue / 255.0f;

            float cmax = max(r, max(g, b));
            float cmin = min(r, min(g, b));
            float delta = cmax - cmin;

            hsv.v = cmax;
            hsv.s = (cmax == 0.0f) ? 0.0f : delta / cmax;

            if (delta == 0.0f)
                hsv.h = 0.0f;
            else if (cmax == r)
                hsv.h = 60.0f * fmodf((g - b) / delta, 6.0f);
            else if (cmax == g)
                hsv.h = 60.0f * ((b - r) / delta + 2.0f);
            else
                hsv.h = 60.0f * ((r - g) / delta + 4.0f);

            if (hsv.h < 0.0f)
                hsv.h += 360.0f;

            return hsv;
        }
    }
    DWORD WINAPI GDI(LPVOID lpParam)
    {

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels;
        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(dcCopy, bmp);

        int i = 0;

        while (1)
        {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    // FAST fx wave
                    int fx = (int)(
                        sinf((x + i) * 0.05f) * 120 +
                        cosf((y + i) * 0.05f) * 120
                        );

                    HSV hsv = Colors::RGBtoHSV(pixels[index]);

                    // ONLY hue is changed
                    hsv.h = fmodf(hsv.h + fx + i * 2, 360.0f);

                    pixels[index] = Colors::HSVtoRGB(hsv);
                }
            }

            i++;

            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1); // FAST (no random delay)
        }

        return 0x00;
    }

    DWORD WINAPI GDI2(LPVOID lpParam)
    {

        srand((unsigned)time(nullptr));

        HDC desk = GetDC(nullptr); // Get desktop DC once
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        while (true)
        {
            int y = rand() % sh;          // random scanline
            int offset = (rand() % 40) - 20; // left/right shift

            BitBlt(
                desk,
                offset, y,               // destination
                sw, 1,                   // width, height (1px line)
                desk,
                0, y,                    // source
                SRCCOPY
            );

            // tiny delay keeps it fast but stable
            Sleep(1);
        }

        ReleaseDC(nullptr, desk);
        return 0;
    }
}
namespace p10 {
    // Custom RGBQUAD structure
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    DWORD WINAPI GDI(LPVOID lpParam)
    {
        // Screen width and height
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* rgbquad = nullptr;

        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        int i = 0;
        double angle = 0.0;

        while (true) {
            // Copy current screen to smaller buffer
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Shader loop (only green channel)
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    int cx = x - (ws / 2);
                    int cy = y - (hs / 2);

                    int zx = (int)(cos(angle) * cx - sin(angle) * cy);
                    int zy = (int)(sin(angle) * cx + cos(angle) * cy);

                    int fx = (zx - i) + (zy - i);

                    rgbquad[index].rgbGreen = (BYTE)((rgbquad[index].rgbGreen + fx) & 0xFF);
                }
            }

            i++;
            angle += 0.11;

            // Copy back to screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCERASE);

            Sleep(1); // smooth animation
        }

        // Cleanup (never reached in this infinite loop)
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);
        DeleteObject(bmp);

        return 0;
    }
    DWORD WINAPI theend(LPVOID lpParam)
    {

        srand((unsigned)time(0));

        HDC hdc;
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        for (;;)
        {
            hdc = GetDC(0);

            int rx = rand() % sw;
            int ry = rand() % sh;

            // KEEPING YOUR ORIGINAL CODE
            HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
            SelectObject(hdc, brush);

            // Vertical displacement glitch
            BitBlt(hdc, rx, 10, 100, sh, hdc, rx, 0, SRCCOPY);
            BitBlt(hdc, rx, -10, -100, sh, hdc, rx, 0, SRCCOPY);

            // KEEPING YOUR ORIGINAL CODE
            BitBlt(hdc, 10, ry, sw, 96, hdc, 0, ry, 0x2021e20);
            BitBlt(hdc, -10, ry, sw, -96, hdc, 0, ry, 0x2021e20);

            DeleteObject(brush);
            ReleaseDC(0, hdc);

            Sleep(1); // very fast glitch speed
        }
    }
    typedef struct {
        FLOAT h;
        FLOAT s;
        FLOAT v;
    } HSV;

    namespace Colors
    {
        RGBQUAD HSVtoRGB(HSV hsv)
        {
            int hi = (int)floor(hsv.h / 60.0) % 6;
            double f = hsv.h / 60.0 - floor(hsv.h / 60.0);
            double p = hsv.v * (1.0 - hsv.s);
            double q = hsv.v * (1.0 - f * hsv.s);
            double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);

            switch (hi)
            {
            case 0: return { (BYTE)(hsv.v * 255), (BYTE)(t * 255), (BYTE)(p * 255) };
            case 1: return { (BYTE)(q * 255), (BYTE)(hsv.v * 255), (BYTE)(p * 255) };
            case 2: return { (BYTE)(p * 255), (BYTE)(hsv.v * 255), (BYTE)(t * 255) };
            case 3: return { (BYTE)(p * 255), (BYTE)(q * 255), (BYTE)(hsv.v * 255) };
            case 4: return { (BYTE)(t * 255), (BYTE)(p * 255), (BYTE)(hsv.v * 255) };
            default: return { (BYTE)(hsv.v * 255), (BYTE)(p * 255), (BYTE)(q * 255) };
            }
        }

        HSV RGBtoHSV(RGBQUAD rgbquad)
        {
            HSV hsv;
            double rd = rgbquad.rgbRed / 255.0;
            double gd = rgbquad.rgbGreen / 255.0;
            double bd = rgbquad.rgbBlue / 255.0;

            double cmax = max(max(rd, gd), bd);
            double cmin = min(min(rd, gd), bd);
            double delta = cmax - cmin;

            hsv.v = cmax;
            hsv.s = (cmax > 0) ? delta / cmax : 0.0;

            if (delta == 0) { hsv.h = 0; }
            else if (cmax == rd) hsv.h = 60.0 * fmod((gd - bd) / delta, 6.0);
            else if (cmax == gd) hsv.h = 60.0 * ((bd - rd) / delta + 2.0);
            else hsv.h = 60.0 * ((rd - gd) / delta + 4.0);

            if (hsv.h < 0.0) hsv.h += 360.0;
            return hsv;
        }
    }

    // Global screen size
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    DWORD WINAPI theend2(LPVOID lpParam)
    {

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(bmpi.bmiHeader);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        SetStretchBltMode(dc, COLORONCOLOR);
        SetStretchBltMode(dcCopy, COLORONCOLOR);

        int i = 0;
        srand((unsigned int)time(NULL));

        while (true)
        {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    int Xii = x * x + i;
                    int Yii = y * y + i;

                    // fx formula
                    int fx = (int)((i ^ 4) + (i * 4) * cbrt((Yii + Xii - i / w * (i % w)) / 500));

                    HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

                    // Only modify hue
                    hsv.h = fmod(fx + i, 360.0);

                    rgbquad[index] = Colors::HSVtoRGB(hsv);
                }
            }

            i++;
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(rand() % 100); // Fast random-ish sleep
        }

        return 0x00;
    }
}