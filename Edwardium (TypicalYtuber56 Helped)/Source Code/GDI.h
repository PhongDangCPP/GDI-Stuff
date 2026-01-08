#pragma once
// credits for ChatGPT Ai For A Make GDI
typedef DWORD payload;

payload WINAPI EPICMAN(LPVOID lpParam)
{

    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    srand((unsigned)time(NULL));

    int radius = 200;
    double angle = 0.0;

    while (true)
    {
        int cx = rand() % sw;
        int cy = rand() % sh;

        for (int i = 0; i < 60; i++)
        {
            angle += 0.3;

            int x = cx + (int)(cos(angle) * radius);
            int y = cy + (int)(sin(angle) * radius);

            int size = 30 + rand() % 120;

            HRGN rgn = CreateEllipticRgn(
                x - size,
                y - size,
                x + size,
                y + size
            );

            InvertRgn(hdc, rgn);
            DeleteObject(rgn);

            Sleep(15);
        }

        // Random hard flash
        HRGN flash = CreateEllipticRgn(
            rand() % sw,
            rand() % sh,
            rand() % sw + 300,
            rand() % sh + 300
        );

        InvertRgn(hdc, flash);
        DeleteObject(flash);
        Sleep(40);
    }

    ReleaseDC(NULL, hdc);
    return 0;

}

payload WINAPI abll(LPVOID lpParam) {
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;

    while (1) {
        HDC hdc = GetDC(HWND_DESKTOP);
        int icon_x = GetSystemMetrics(SM_CXICON);
        int icon_y = GetSystemMetrics(SM_CYICON);

        x += incrementor * signX;
        y += incrementor * signY;

        DrawIconEx(
            hdc,
            x - icon_x,
            y - icon_y,
            LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2)),
            3 * GetSystemMetrics(SM_CXICON),
            3 * GetSystemMetrics(SM_CYICON),
            0,
            NULL,
            DI_NORMAL
        );

        if (y >= GetSystemMetrics(SM_CYSCREEN)) {
            signY = -1;
        }

        if (x >= GetSystemMetrics(SM_CXSCREEN)) {
            signX = -1;
        }

        if (y <= 0) {
            signY = 1;
        }

        if (x <= 0) {
            signX = 1;
        }

        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 1;
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
            float r = rgb.rgbRed / 255.0f;
            float g = rgb.rgbGreen / 255.0f;
            float b = rgb.rgbBlue / 255.0f;

            float minv = min(r, min(g, b));
            float maxv = max(r, max(g, b));
            float delta = maxv - minv;

            HSL hsl{};
            hsl.l = (maxv + minv) * 0.5f;

            if (delta > 0.0f)
            {
                hsl.s = (hsl.l < 0.5f) ? (delta / (maxv + minv)) :
                    (delta / (2.0f - maxv - minv));

                float dr = (((maxv - r) / 6.0f) + (delta * 0.5f)) / delta;
                float dg = (((maxv - g) / 6.0f) + (delta * 0.5f)) / delta;
                float db = (((maxv - b) / 6.0f) + (delta * 0.5f)) / delta;

                if (r == maxv)      hsl.h = db - dg;
                else if (g == maxv) hsl.h = (1.0f / 3.0f) + dr - db;
                else                hsl.h = (2.0f / 3.0f) + dg - dr;

                if (hsl.h < 0.0f) hsl.h += 1.0f;
                if (hsl.h > 1.0f) hsl.h -= 1.0f;
            }

            return hsl;
        }

        __forceinline RGBQUAD hsl2rgb(const HSL& hsl)
        {
            float r = hsl.l, g = hsl.l, b = hsl.l;

            float v = (hsl.l <= 0.5f) ? (hsl.l * (1.0f + hsl.s))
                : (hsl.l + hsl.s - hsl.l * hsl.s);

            if (v > 0.0f)
            {
                float m = hsl.l * 2.0f - v;
                float sv = (v - m) / v;

                float h = hsl.h * 6.0f;
                int sextant = (int)h;
                float fract = h - sextant;

                float vsf = v * sv * fract;
                float mid1 = m + vsf;
                float mid2 = v - vsf;

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

            RGBQUAD rgb{};
            rgb.rgbRed = (BYTE)(r * 255.0f);
            rgb.rgbGreen = (BYTE)(g * 255.0f);
            rgb.rgbBlue = (BYTE)(b * 255.0f);
            return rgb;
        }
    }
    payload WINAPI GDI(LPVOID lpParam)
    {

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(hdc);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbq = 0;
        HBITMAP dib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&rgbq, 0, 0);
        SelectObject(memDC, dib);

        int tick = 0;

        while (1)
        {
            hdc = GetDC(0);

            BitBlt(memDC, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < h; ++y)
            {
                int row = y * w;
                for (int x = 0; x < w; ++x)
                {
                    int i = row + x;

                    int average = (int)roundf(
                        (rgbq[i].rgbBlue +
                            rgbq[i].rgbRed +
                            rgbq[i].rgbGreen) / 8.0f);

                    int fx = average + average;

                    HSL hsl = Colors::rgb2hsl(rgbq[i]);
                    hsl.h = fmodf((fx / 500.0f) + hsl.h + 0.02f, 1.0f);
                    hsl.s = 1.0f;
                    hsl.h += 0.5f;
                    if (hsl.h > 1.0f) hsl.h -= 1.0f;

                    rgbq[i] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

            // reversed tunnel
            StretchBlt(
                hdc,
                -20, -20,
                w + 40, h + 40,
                hdc,
                0, 0,
                w, h,
                SRCCOPY
            );

            tick++;

            // keep gOING
            ReleaseDC(NULL, hdc);
            DeleteDC(hdc);
        }

        return 0x00;
    }
}
namespace p2 {
    // Defining the TRGBQUAD structure
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE r;
            BYTE g;
            BYTE b;
            BYTE Reserved;
        };
    } TRGBQUAD, * NTRGBQUAD;

    // Global screen width and height
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    // Hue-related color variables
    int red = 0, green = 0, blue = 0;
    bool ifcolorblue = false, ifblue = false;

    // Hue function to change colors over time
    COLORREF Hue(int length) {
        if (red != length) {
            red++;
            if (ifblue == true) {
                return RGB(red, 0, length);
            }
            else {
                return RGB(red, 0, 0);
            }
        }
        else {
            if (green != length) {
                green++;
                return RGB(length, green, 0);
            }
            else {
                if (blue != length) {
                    blue++;
                    return RGB(0, length, blue);
                }
                else {
                    red = 0; green = 0; blue = 0;
                    ifblue = true;
                }
            }
        }
        return RGB(0, 0, 0); // Default return if no condition is met
    }
    payload WINAPI GDI(LPVOID lpParam)
    {

        // Set up the device context and compatible DC
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        // Half screen width and height for scaling
        int ws = w / 2;
        int hs = h / 2;

        // Create a BITMAPINFO structure to describe the DIB section
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        // Create the bitmap and select it into the DC
        TRGBQUAD* rgbquad = NULL;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        // Animation variables
        INT i = 0;
        DOUBLE angle = 0.0;

        // Random seed for variability in sleep times
        srand(time(NULL));

        // Main animation loop
        while (1) {
            // Stretch the original screen image into the smaller bitmap section
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Apply color transformations to each pixel
            for (int x = 0; x < ws; x++) {
                for (int y = 0; y < hs; y++) {
                    int index = y * ws + x;
                    // Apply hue effect based on pixel position and animation counter
                    rgbquad[index].rgborg = ((x ^ y) & i) - (i * Hue(239));
                }
            }

            // Increment the counter and adjust the angle for animation
            i++;
            angle += 0.01f;

            // Stretch the transformed bitmap back to the original screen area
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            // Add a random sleep time for variability in animation speed
            Sleep(rand() % 10);

            // Randomly reset the animation (optional)
            if (rand() % 25 == 0) {
                // Add any reset logic here, if necessary
            }
        }

        // Cleanup
        DeleteObject(bmp);
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);

        return 0;
    }
}
namespace p3 {

    // Custom RGB union
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE b;
            BYTE g;
            BYTE r;
            BYTE Reserved;
        };
    } TRGBQUAD, * PTRGBQUAD;
    payload WINAPI GDI(LPVOID lpParam)
    {

        srand((unsigned int)time(NULL));

        // Get screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        int ws = w / 2;
        int hs = h / 2;

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        // Setup bitmap info
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = -hs; // top-down
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(dcCopy, bmp);

        while (true) {
            if ((rand() % 4) == 0) RedrawWindow(0, 0, 0, 133);
            // Downscale screen to small buffer
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Pixel shader
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    if ((rand() % 4) == 0) {
                        pixels[index].r = (pixels[index].r >= 255) ? pixels[index].r : pixels[index].r;
                        pixels[index].g = pixels[index].g + (pixels[index].g / 10);
                        pixels[index].b = pixels[index].b - (pixels[index].b / 5);
                    }
                    else {
                        pixels[index].r = (pixels[index].r >= 255) ? pixels[index].r + 5 : pixels[index].r;
                        pixels[index].g = pixels[index].g * (pixels[index].g / 10);
                        pixels[index].b = pixels[index].b + (pixels[index].b / 5);
                    }
                }
            }

            // Upscale back to screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1);
        }

        DeleteObject(bmp);
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);

        return 0;
    }
}
namespace p4 {
    typedef union _TRGBQUAD {
        COLORREF rgborg;
        struct {
            BYTE b;
            BYTE g;
            BYTE r;
            BYTE Reserved;
        };
    } _FUCKQUAD, TRGBQUAD, * NTRGBQUAD;
    payload WINAPI GDI(LPVOID lpParam)
    {

        HDC dc = GetDC(NULL);                 // screen DC
        HDC dcCopy = CreateCompatibleDC(dc);  // memory DC

        // Get screen dimensions
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 2;
        int hs = h / 2;

        // Create DIB section
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = -hs;  // negative for top-down
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* trgb = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&trgb, NULL, 0);
        SelectObject(dcCopy, bmp);

        int i = 0;
        double angle = 0.0;

        while (true) {
            // Capture screen into half-size buffer
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Shader effect on pixels
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    int dt = 128 + i;
                    int fx = dt + (int)(dt * sin(i + x / 16.0)) + dt + (int)(dt * sin(i + y / 16.0));

                    trgb[index].rgborg += fx + 100; // manipulate color in one shot
                }
            }

            i++;
            angle += 0.01;

            // Stretch back to screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1); // minimal delay for animation
        }

        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);
        DeleteObject(bmp);

        return 0;
    }
}
namespace p5 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    payload WINAPI GDI(LPVOID lpParam)
    {

        // Get the screen dimensions
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Get device contexts
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        // Setup bitmap info
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = -hs; // top-down
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;

        // Create DIB section
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(dcCopy, bmp);

        while (true) {
            // Copy screen to smaller DC
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Apply grayscale shader
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;
                    TRGBQUAD& px = pixels[index];
                    // Luminance grayscale formula: 0.299*R + 0.587*G + 0.114*B
                    BYTE gray = (BYTE)((px.rgbRed * 299 + px.rgbGreen * 587 + px.rgbBlue * 114) / 1000);
                    px.rgbRed *= gray;
                    px.rgbGreen *= gray;
                    px.rgbBlue *= gray;
                }
            }

            // Stretch the modified bitmap back to the screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1);
        }

        DeleteObject(bmp);
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);

        return 0;
    }
}
namespace p6 {
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

            float maxc = max(max(r, g), b);
            float minc = min(min(r, g), b);
            float delta = maxc - minc;

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

        RGBQUAD hsl2rgb(HSL hsl)
        {
            RGBQUAD rgb;
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

            rgb.rgbRed = (BYTE)(r * 255.f);
            rgb.rgbGreen = (BYTE)(g * 255.f);
            rgb.rgbBlue = (BYTE)(b * 255.f);
            rgb.rgbReserved = 0;
            return rgb;
        }
    }
    payload WINAPI GDI(LPVOID lpParam)
    {

        // Fullscreen HDC
        HDC hdcScreen = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = -h; // top-down DIB
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);

        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcMem, hBitmap);

        int frame = 0;
        while (true)
        {
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            float angle = frame * 0.02f;
            float cosA = cosf(angle);
            float sinA = sinf(angle);
            float cx = w / 2.f;
            float cy = h / 2.f;

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int idx = y * w + x;

                    // Center rotation
                    float dx = x - cx;
                    float dy = y - cy;
                    int rx = (int)(cx + dx * cosA - dy * sinA);
                    int ry = (int)(cy + dx * sinA + dy * cosA);
                    if (rx < 0) rx = 0; if (rx >= w) rx = w - 1;
                    if (ry < 0) ry = 0; if (ry >= h) ry = h - 1;
                    int rIdx = ry * w + rx;

                    HSL hsl = Colors::rgb2hsl(pixels[rIdx]);
                    hsl.h = fmodf(hsl.h + frame * 0.002f + (float)y / h * 0.1f, 1.f);
                    pixels[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            frame++;
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
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

            float maxVal = max(max(r, g), b);
            float minVal = min(min(r, g), b);
            float delta = maxVal - minVal;

            hsl.l = (maxVal + minVal) / 2.f;
            hsl.s = 0.f;
            hsl.h = 0.f;

            if (delta != 0.f)
            {
                hsl.s = hsl.l < 0.5f ? delta / (maxVal + minVal) : delta / (2.f - maxVal - minVal);

                if (r == maxVal) hsl.h = (g - b) / delta;
                else if (g == maxVal) hsl.h = 2.f + (b - r) / delta;
                else hsl.h = 4.f + (r - g) / delta;

                hsl.h /= 6.f;
                if (hsl.h < 0.f) hsl.h += 1.f;
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
                auto hue2rgb = [](float p, float q, float t)
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

        void* pBits = nullptr;
        HBITMAP hBmp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcMem, hBmp);

        RGBQUAD* pixels = (RGBQUAD*)pBits;
        int frame = 0;

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

                    float fx = 239 + frame;
                    hsl.h = fmod(fx / 500.f + hsl.h + 0.02f + sinf(frame * 0.01f) * 0.1f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    pixels[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            frame++;
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        DeleteObject(hBmp);

        return 0;
    }
}
namespace p8 {
    typedef struct
    {
        FLOAT h; // animation only
        FLOAT s; // keep
        FLOAT l; // keep
    } HSL;

    namespace Colors
    {
        HSL rgb2hsl(RGBQUAD rgb)
        {
            HSL hsl;
            BYTE r = rgb.rgbRed;
            BYTE g = rgb.rgbGreen;
            BYTE b = rgb.rgbBlue;

            FLOAT _r = r / 255.f;
            FLOAT _g = g / 255.f;
            FLOAT _b = b / 255.f;

            FLOAT rgbMin = min(min(_r, _g), _b);
            FLOAT rgbMax = max(max(_r, _g), _b);

            FLOAT fDelta = rgbMax - rgbMin;
            hsl.l = (rgbMax + rgbMin) / 2.f;
            hsl.s = (fDelta == 0.f) ? 0.f : fDelta / (1.f - fabsf(2.f * hsl.l - 1.f));

            hsl.h = 0.f; // we only animate h later
            return hsl;
        }

        RGBQUAD hsl2rgb(HSL hsl)
        {
            RGBQUAD rgb = { 0 };
            FLOAT c = (1.f - fabsf(2.f * hsl.l - 1.f)) * hsl.s;
            FLOAT x = c * (1.f - fabsf(fmodf(hsl.h * 6.f, 2.f) - 1.f));
            FLOAT m = hsl.l - c / 2.f;

            FLOAT r1 = 0.f, g1 = 0.f, b1 = 0.f;

            if (hsl.h < 1.f / 6.f) { r1 = c; g1 = x; b1 = 0; }
            else if (hsl.h < 2.f / 6.f) { r1 = x; g1 = c; b1 = 0; }
            else if (hsl.h < 3.f / 6.f) { r1 = 0; g1 = c; b1 = x; }
            else if (hsl.h < 4.f / 6.f) { r1 = 0; g1 = x; b1 = c; }
            else if (hsl.h < 5.f / 6.f) { r1 = x; g1 = 0; b1 = c; }
            else { r1 = c; g1 = 0; b1 = x; }

            rgb.rgbRed = (BYTE)((r1 + m) * 255.f);
            rgb.rgbGreen = (BYTE)((g1 + m) * 255.f);
            rgb.rgbBlue = (BYTE)((b1 + m) * 255.f);

            return rgb;
        }
    }

    payload WINAPI GDI(LPVOID lpParam)
    {

        // Fullscreen HDC
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = -h; // top-down
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, bmp);

        FLOAT i = 0.f;

        while (true)
        {
            // Copy screen to memory
            BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int idx = y * w + x;
                    HSL hsl = Colors::rgb2hsl(pixels[idx]);

                    // Animate hue using sine, cosine, tangent
                    FLOAT fx = (x + i) * 0.002f;
                    FLOAT fy = (y + i) * 0.002f;

                    hsl.h = fmodf(
                        (sinf(fx) + cosf(fy) + tanf(fx + fy)) * 0.5f + 0.5f, 1.f
                    );

                    pixels[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            i += 1.f;
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);
        return 0;
    }
}
namespace p9 {

    // Custom RGB structure
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

        // Get screen DC
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        // Half size for offscreen buffer
        int ws = w / 2;
        int hs = h / 2;

        // Create DIB section
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = -hs; // top-down
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* rgbquad = nullptr;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        double angle = 0.0;

        while (1) {
            // Copy screen to offscreen buffer (half size)
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Apply shader
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    int sepiaRed = round(.666 * rgbquad[index].r + .666 * rgbquad[index].g + .666 * rgbquad[index].b);
                    int sepiaGreen = round(.666 * rgbquad[index].r + .666 * rgbquad[index].g + .666 * rgbquad[index].b);
                    int sepiaBlue = round(.666 * rgbquad[index].r + .666 * rgbquad[index].g + .666 * rgbquad[index].b);

                    if (sepiaBlue > 255) sepiaBlue = 255;
                    if (sepiaRed > 255) sepiaRed = 127;
                    if (sepiaGreen > 255) sepiaGreen = 64;

                    rgbquad[index].r += sepiaRed + 128;
                    rgbquad[index].g += sepiaGreen - 64;
                    rgbquad[index].b += 5;
                }
            }

            // Draw back to screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            angle += 0.01;
            Sleep(1); // fast animation
        }

        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);
        DeleteObject(bmp);

        return 0;
    }
}
namespace p10 {
    typedef struct {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE a; // keep for alignment
    } TRGBQUAD;
    payload WINAPI GDI(LPVOID lpParam)
    {

        // Get full screen dimensions
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

        TRGBQUAD* pixels = NULL;
        HBITMAP bmp = CreateDIBSection(dc, (BITMAPINFO*)&bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(dcCopy, bmp);

        double angle = 0.0;

        while (1) {
            // Copy full screen to smaller bitmap
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            // Apply shader effect per pixel
            for (int y = 0; y < hs; y++) {
                for (int x = 0; x < ws; x++) {
                    int index = y * ws + x;

                    BYTE r = GetRValue(RGB(pixels[index].r, pixels[index].g, pixels[index].b));
                    BYTE g = GetGValue(RGB(pixels[index].r, pixels[index].g, pixels[index].b));
                    BYTE b = GetBValue(RGB(pixels[index].r, pixels[index].g, pixels[index].b));

                    // Example shader: luminance + sine/cosine/tangent wave
                    double lum = (r + g + b) / 3.0;
                    pixels[index].r += (BYTE)min(255, max(0, lum + 50 * sin(angle + x * 0.05)));
                    pixels[index].g += (BYTE)min(255, max(0, lum + 50 * cos(angle + y * 0.05)));
                    pixels[index].b += (BYTE)min(255, max(0, lum + 50 * tan(fmod(angle + (x + y) * 0.01, 3.1415 / 2))));

                    //pixels[index].a = 0; // keep alpha zero
                }
            }

            // Stretch back to full screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            angle += 0.05;
            Sleep(1);
        }

        DeleteObject(bmp);
        DeleteDC(dcCopy);
        ReleaseDC(NULL, dc);
        return 0;
    }
}
namespace p11 {

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846

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
            float r = rgb.rgbRed / 255.0f;
            float g = rgb.rgbGreen / 255.0f;
            float b = rgb.rgbBlue / 255.0f;

            float maxc = max(r, max(g, b));
            float minc = min(r, min(g, b));
            float delta = maxc - minc;

            HSL hsl{};
            hsl.l = (maxc + minc) * 0.5f;

            if (delta > 0.00001f)
            {
                hsl.s = delta / (1.0f - fabsf(2.0f * hsl.l - 1.0f));

                if (maxc == r)
                    hsl.h = fmodf((g - b) / delta, 6.0f);
                else if (maxc == g)
                    hsl.h = ((b - r) / delta) + 2.0f;
                else
                    hsl.h = ((r - g) / delta) + 4.0f;

                hsl.h /= 6.0f;
                if (hsl.h < 0.0f) hsl.h += 1.0f;
            }
            return hsl;
        }

        __forceinline RGBQUAD hsl2rgb(const HSL& hsl)
        {
            float h = hsl.h * 6.0f;
            float c = (1.0f - fabsf(2.0f * hsl.l - 1.0f)) * hsl.s;
            float x = c * (1.0f - fabsf(fmodf(h, 2.0f) - 1.0f));
            float m = hsl.l - c * 0.5f;

            float r = 0, g = 0, b = 0;

            switch ((int)h)
            {
            case 0: r = c; g = x; break;
            case 1: r = x; g = c; break;
            case 2: g = c; b = x; break;
            case 3: g = x; b = c; break;
            case 4: r = x; b = c; break;
            case 5: r = c; b = x; break;
            }

            RGBQUAD rgb{};
            rgb.rgbRed = (BYTE)((r + m) * 255.0f);
            rgb.rgbGreen = (BYTE)((g + m) * 255.0f);
            rgb.rgbBlue = (BYTE)((b + m) * 255.0f);
            return rgb;
        }
    }

    payload WINAPI GDI(LPVOID lpParam)
    {

        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(NULL);
        HDC mem = CreateCompatibleDC(hdc);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sw;
        bmi.bmiHeader.biHeight = -sh;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbq = nullptr;
        HBITMAP dib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&rgbq, 0, 0);
        SelectObject(mem, dib);

        float t = 0.0f;

        while (1)
        {
            BitBlt(mem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < sh; y++)
            {
                for (int x = 0; x < sw; x++)
                {
                    int i = y * sw + x;

                    HSL hsl = Colors::rgb2hsl(rgbq[i]);

                    float sx = sinf(x * 0.01f + t);
                    float cy = cosf(y * 0.01f - t);
                    float swirl = atan2f(y - sh * 0.5f, x - sw * 0.5f) / (2.0f * M_PI);

                    hsl.h = fmodf(hsl.h + sx * 0.05f + cy * 0.05f + swirl, 1.0f);
                    if (hsl.h < 0.0f) hsl.h += 1.0f;

                    rgbq[i] = Colors::hsl2rgb(hsl);
                }
            }

            int xSize = sw / 10;
            int ySize = 9;

            for (int i = 0; i < sw; i++)
            {
                int wave = (int)(sinf(i / (float)xSize * M_PI + t) * ySize);
                BitBlt(mem, i, 0, 1, sh, mem, i, wave, SRCCOPY);
            }

            BLENDFUNCTION blend{ AC_SRC_OVER, 0, 180, 0 };
            AlphaBlend(hdc, 0, 0, sw, sh, mem, 0, 0, sw, sh, blend);

            t += 0.03f;
        }

        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);

        return 0x00;
    }
}

