#pragma once
namespace gdis1 {
    typedef struct TRGBQUAD {
        BYTE b, g, r, a;
    } TRGBQUAD;

    // ---------------- HSV → RGB ----------------
    static TRGBQUAD HSVtoRGB(float h, float s, float v) {
        float c = v * s;
        float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
        float m = v - c;

        float r, g, b;
        if (h < 60) { r = c; g = x; b = 0; }
        else if (h < 120) { r = x; g = c; b = 0; }
        else if (h < 180) { r = 0; g = c; b = x; }
        else if (h < 240) { r = 0; g = x; b = c; }
        else if (h < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        TRGBQUAD out;
        out.r = (BYTE)((r + m) * 255);
        out.g = (BYTE)((g + m) * 255);
        out.b = (BYTE)((b + m) * 255);
        out.a = 255;
        return out;
    }

    // --------------- OPTIONAL RGB → HSV ----------------
    static void RGBtoHSV(BYTE R, BYTE G, BYTE B, float& h, float& s, float& v) {
        float r = R / 255.0f;
        float g = G / 255.0f;
        float b = B / 255.0f;

        float cmax = max(r, max(g, b));
        float cmin = min(r, min(g, b));
        float diff = cmax - cmin;

        if (diff == 0) h = 0;
        else if (cmax == r) h = fmodf(60 * ((g - b) / diff) + 360, 360);
        else if (cmax == g) h = 60 * ((b - r) / diff + 2);
        else                h = 60 * ((r - g) / diff + 4);

        s = (cmax == 0 ? 0 : diff / cmax);
        v = cmax;
    }

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int W = GetSystemMetrics(SM_CXSCREEN);
        int H = GetSystemMetrics(SM_CYSCREEN);

        HDC hScreen = GetDC(NULL);
        HDC hMem = CreateCompatibleDC(hScreen);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = W;
        bmi.bmiHeader.biHeight = -H; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;
        HBITMAP hBmp = CreateDIBSection(hScreen, &bmi, DIB_RGB_COLORS,
            (void**)&pixels, NULL, 0);
        SelectObject(hMem, hBmp);

        float t = 0.0f;

        while (1) {
            t += 0.03f;

            for (int y = 0; y < H; y++) {
                for (int x = 0; x < W; x++) {
                    // Plasma & ripple formulas
                    float v1 = sinf(x * 0.01f + t);
                    float v2 = sinf(y * 0.015f - t * 1.5f);
                    float d = sqrtf((x - W / 2) * (x - W / 2) + (y - H / 2) * (y - H / 2));
                    float v3 = sinf(d * 0.02f - t * 2.0f);

                    float v = (v1 + v2 + v3) / 3.0f;

                    // Hue cycling
                    float hue = fmodf((v * 180.0f + t * 60.0f), 360.0f);
                    if (hue < 0) hue += 360;

                    pixels[y * W + x] = HSVtoRGB(hue, 1.0f, 1.0f);
                }
            }

            // Draw result to screen
            BitBlt(hScreen, 0, 0, W, H, hMem, 0, 0, SRCCOPY);
            Sleep(1);
        }

        // unreachable, but clean up if needed
        DeleteObject(hBmp);
        DeleteDC(hMem);
        ReleaseDC(NULL, hScreen);
        return 0;
    }
}
namespace gdis2 {
#define WIDTH  GetSystemMetrics(SM_CXSCREEN)
#define HEIGHT GetSystemMetrics(SM_CYSCREEN)

    // Typedef for RGB quad
    typedef struct TRGBQUAD {
        uint8_t rgbBlue;
        uint8_t rgbGreen;
        uint8_t rgbRed;
        uint8_t rgbReserved;
    } TRGBQUAD;

    // HSV structure
    typedef struct THSV {
        float h; // 0..360
        float s; // 0..1
        float v; // 0..1
    } THSV;

    // Convert HSV to RGB
    TRGBQUAD HSVtoRGB(const THSV& hsv) {
        float h = hsv.h, s = hsv.s, v = hsv.v;
        float r = 0, g = 0, b = 0;

        int i = int(h / 60.0f) % 6;
        float f = h / 60.0f - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        }

        TRGBQUAD rgb;
        rgb.rgbRed = (uint8_t)(r * 255);
        rgb.rgbGreen = (uint8_t)(g * 255);
        rgb.rgbBlue = (uint8_t)(b * 255);
        rgb.rgbReserved = 0;
        return rgb;
    }

    // Convert RGB to HSV
    THSV RGBtoHSV(const TRGBQUAD& rgb) {
        float r = rgb.rgbRed / 255.f;
        float g = rgb.rgbGreen / 255.f;
        float b = rgb.rgbBlue / 255.f;
        float maxc = fmaxf(r, fmaxf(g, b));
        float minc = fminf(r, fminf(g, b));
        float delta = maxc - minc;

        THSV hsv;
        hsv.v = maxc;
        hsv.s = (maxc == 0) ? 0 : delta / maxc;

        if (delta == 0) {
            hsv.h = 0;
        }
        else if (maxc == r) {
            hsv.h = 60 * fmodf(((g - b) / delta), 6);
        }
        else if (maxc == g) {
            hsv.h = 60 * (((b - r) / delta) + 2);
        }
        else {
            hsv.h = 60 * (((r - g) / delta) + 4);
        }
        if (hsv.h < 0) hsv.h += 360;
        return hsv;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(NULL);

        // Create a compatible DC and bitmap
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, WIDTH, HEIGHT);
        SelectObject(hdcMem, hBitmap);

        TRGBQUAD* pixels = new TRGBQUAD[WIDTH * HEIGHT];

        float zoom = 1.0f;
        float angle = 0.0f;

        while (1) {
            // Fractal XOR + HSV color calculation
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    float nx = (x - WIDTH / 2.0f) / (WIDTH / 2.0f) * zoom;
                    float ny = (y - HEIGHT / 2.0f) / (HEIGHT / 2.0f) * zoom;

                    // Rotation
                    float rx = nx * cos(angle) - ny * sin(angle);
                    float ry = nx * sin(angle) + ny * cos(angle);

                    // XOR fractal
                    int ix = (int)(fabsf(rx * 10)) % 256;
                    int iy = (int)(fabsf(ry * 10)) % 256;
                    int val = (ix ^ iy) % 256;

                    THSV hsv = { (float)(val), 1.0f, 1.0f };
                    pixels[y * WIDTH + x] = HSVtoRGB(hsv);
                }
            }

            // Copy pixels to bitmap
            BITMAPINFO bmi = { 0 };
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = WIDTH;
            bmi.bmiHeader.biHeight = -HEIGHT; // top-down
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            SetDIBits(hdcMem, hBitmap, 0, HEIGHT, pixels, &bmi, DIB_RGB_COLORS);

            // XOR blit to screen
            BitBlt(hdcScreen, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, SRCERASE);

            angle += 0.01f;
            zoom += 0.999f; // zoom out slowly

            Sleep(1);
        }

        delete[] pixels;
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 0;
    }
}
namespace gdis3 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        srand((unsigned int)time(0));

        // Get full screen size
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(NULL); // Get desktop DC

        while (1) {
            // Random ellipse
            int x = rand() % screenWidth;
            int y = rand() % screenHeight;
            int w = 10 + rand() % 100;
            int h = 10 + rand() % 100;

            COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);

            HBRUSH brush = CreateSolidBrush(color);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
            Ellipse(hdc, x, y, x + w, y + h);
            SelectObject(hdc, oldBrush);
            DeleteObject(brush);

            // Pixel-level wave effect
            int waveHeight = 5 + rand() % 20;
            for (int i = 0; i < 500; i++) {
                int px = rand() % screenWidth;
                int py = rand() % screenHeight;
                COLORREF c = RGB(
                    (sin(px * 0.05) * 127 + 128) / 2 + rand() % 50,
                    (cos(py * 0.05) * 127 + 128) / 2 + rand() % 50,
                    (sin((px + py) * 0.03) * 127 + 128) / 2 + rand() % 50
                );
                SetPixel(hdc, px, py, c);
            }

            // BitBlt glitch wave
            int bx = rand() % screenWidth;
            int by = rand() % screenHeight;
            int bw = 50 + rand() % 200;
            int bh = 20 + rand() % 100;
            BitBlt(hdc, bx, by, bw, bh, hdc, bx + (rand() % 20 - 10), by + (rand() % 10 - 5), SRCINVERT);

            Sleep(1);
        }

        ReleaseDC(NULL, hdc);
        return 0;
    }
    DWORD WINAPI RGDI2(LPVOID lpParam)
    {
        // Get full screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Get DC for the entire screen
        HDC hdc = GetDC(NULL);

        // For sine waves and random chaos
        float t = 0;

        while (1) {
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    // Rainbow "colossal cube" style color
                    int r = (int)((sin((x + t) / 20.0f) + 1.0f) * 127.0f);
                    int g = (int)((sin((y + t) / 25.0f) + 1.0f) * 127.0f);
                    int b = (int)((sin((x + y + t) / 30.0f) + 1.0f) * 127.0f);

                    // Bullet spam random chaos
                    if (rand() % 50 == 0) r = g = b = rand() % 256;

                    SetPixel(hdc, x, y, RGB(r, g, b));
                }
            }

            // BitBlt chaos: copy some screen parts randomly
            int sx = rand() % w;
            int sy = rand() % h;
            int sw = rand() % (w / 4);
            int sh = rand() % (h / 4);
            BitBlt(hdc, rand() % (w - sw), rand() % (h - sh), sw, sh, hdc, sx, sy, NOTSRCCOPY);

            t += 0.1f;           // advance time for sine wave
            Sleep(1);             // tiny delay to avoid locking CPU 100%
        }

        ReleaseDC(NULL, hdc);
        return 0;
    }
}
namespace gdis4 {
    typedef struct TRGBQUAD {
        BYTE b, g, r, a;
    } TRGBQUAD;

    //-------------------------------------------------------------
    // HSV → RGB  (0–360, 0–1, 0–1)
    //-------------------------------------------------------------
    TRGBQUAD HSVtoRGB(float h, float s, float v)
    {
        float c = v * s;
        float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
        float m = v - c;

        float r = 0, g = 0, b = 0;
        if (h < 60) { r = c; g = x; b = 0; }
        else if (h < 120) { r = x; g = c; b = 0; }
        else if (h < 180) { r = 0; g = c; b = x; }
        else if (h < 240) { r = 0; g = x; b = c; }
        else if (h < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        TRGBQUAD q;
        q.r = (BYTE)((r + m) * 255);
        q.g = (BYTE)((g + m) * 255);
        q.b = (BYTE)((b + m) * 255);
        q.a = 255;
        return q;
    }

    //-------------------------------------------------------------
    // RGB → HSV (unused but requested)
    //-------------------------------------------------------------
    void RGBtoHSV(BYTE R, BYTE G, BYTE B, float& H, float& S, float& V)
    {
        float r = R / 255.f, g = G / 255.f, b = B / 255.f;
        float mx = max(r, max(g, b));
        float mn = min(r, min(g, b));
        float d = mx - mn;

        // Hue
        if (d == 0) H = 0;
        else if (mx == r) H = 60 * fmodf(((g - b) / d), 6);
        else if (mx == g) H = 60 * (((b - r) / d) + 2);
        else              H = 60 * (((r - g) / d) + 4);

        if (H < 0) H += 360;

        // Sat + Value
        S = (mx == 0 ? 0 : d / mx);
        V = mx;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int W = GetSystemMetrics(SM_CXSCREEN);
        int H = GetSystemMetrics(SM_CYSCREEN);

        // Create 32-bit DIB buffer
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = W;
        bmi.bmiHeader.biHeight = -H;  // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;
        HBITMAP hBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);

        HDC screen = GetDC(NULL);
        HDC memdc = CreateCompatibleDC(screen);
        SelectObject(memdc, hBmp);

        DWORD t = 0;

        while (1)
        {
            // SUPER-FAST RAINBOW HSV WAVE
            for (int y = 0; y < H; y++)
            {
                for (int x = 0; x < W; x++)
                {
                    float h = fmodf((x + t) * 0.5f + (y * 0.2f), 360.0f);
                    float s = 1.0f;
                    float v = 1.0f;

                    // Ripple distortion
                    float ripple = sinf((x + y + t) * 0.02f) * 30.0f;
                    h = fmodf(h + ripple, 360.f);

                    pixels[y * W + x] = HSVtoRGB(h, s, v);
                }
            }

            BitBlt(screen, 0, 0, W, H, memdc, 0, 0, SRCCOPY);

            t++;
            Sleep(1);
        }

        DeleteDC(memdc);
        ReleaseDC(NULL, screen);
        DeleteObject(hBmp);

        return 0;
    }
}
namespace gdis5 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        srand((unsigned)time(0));

        // Get full screen dimensions
        int screenX = GetSystemMetrics(SM_CXSCREEN);
        int screenY = GetSystemMetrics(SM_CYSCREEN);

        // Get DC for the entire screen
        HDC hScreenDC = GetDC(NULL);

        // Create a compatible DC and bitmap
        HDC hMemDC = CreateCompatibleDC(hScreenDC);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenX, screenY);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

        // Initialize bounce rectangle
        int rectW = 200, rectH = 100;
        int rectX = rand() % (screenX - rectW);
        int rectY = rand() % (screenY - rectH);
        int dx = 5, dy = 3;

        // Initialize sine wave variables
        double phase = 0.0;

        while (true) {
            // --- Bounce Rectangle with NOTSRCCOPY ---
            BitBlt(hScreenDC, rectX, rectY, rectW, rectH, hScreenDC, rectX, rectY, NOTSRCCOPY);

            rectX += dx;
            rectY += dy;

            if (rectX <= 0 || rectX + rectW >= screenX) dx = -dx;
            if (rectY <= 0 || rectY + rectH >= screenY) dy = -dy;

            // --- Big Rainbow Circle using SetPixel ---
            int centerX = screenX / 2;
            int centerY = screenY / 2;
            int radius = 150;

            for (int y = -radius; y <= radius; y++) {
                for (int x = -radius; x <= radius; x++) {
                    if (x * x + y * y <= radius * radius) {
                        COLORREF color = RGB(
                            (x + radius) % 256,
                            (y + radius) % 256,
                            (x + y + radius) % 256
                        );
                        SetPixel(hScreenDC, centerX + x, centerY + y, color);
                    }
                }
            }

            // --- Sine Wave BitBlt effect ---
            int waveHeight = 50;
            for (int y = 0; y < screenY; y++) {
                int offset = (int)(waveHeight * sin(y * 0.05 + phase));
                BitBlt(hScreenDC, offset, y, screenX, 1, hScreenDC, 0, y, SRCCOPY);
            }
            phase += 0.1;

            Sleep(1);
        }

        // Cleanup (never reached in this demo)
        SelectObject(hMemDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);

        return 0;
    }
}
namespace gdis6 {
    typedef struct TRGBQUAD {
        uint8_t rgbBlue;
        uint8_t rgbGreen;
        uint8_t rgbRed;
        uint8_t rgbReserved;
    } TRGBQUAD;

    // Convert HSV to RGB (values: H=0-360, S=0-1, V=0-1)
    TRGBQUAD HSVtoRGB(float H, float S, float V) {
        float C = V * S;
        float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
        float m = V - C;
        float r, g, b;

        if (H < 60) { r = C; g = X; b = 0; }
        else if (H < 120) { r = X; g = C; b = 0; }
        else if (H < 180) { r = 0; g = C; b = X; }
        else if (H < 240) { r = 0; g = X; b = C; }
        else if (H < 300) { r = X; g = 0; b = C; }
        else { r = C; g = 0; b = X; }

        TRGBQUAD color;
        color.rgbRed = (uint8_t)((r + m) * 255);
        color.rgbGreen = (uint8_t)((g + m) * 255);
        color.rgbBlue = (uint8_t)((b + m) * 255);
        color.rgbReserved = 0;
        return color;
    }

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        HDC hScreen = GetDC(NULL); // entire screen
        HDC hMem = CreateCompatibleDC(hScreen);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits;
        HBITMAP hBmp = CreateDIBSection(hMem, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hMem, hBmp);

        TRGBQUAD* pixels = (TRGBQUAD*)pBits;
        int frame = 0;

        while (1) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float angle = atan2f((float)(y - height / 2), (float)(x - width / 2));
                    float dist = sqrtf((float)((x - width / 2) * (x - width / 2) + (y - height / 2) * (y - height / 2)));
                    float hue = fmodf((angle * 180.0f / 3.14159f + frame + dist / 5.0f), 360.0f);
                    pixels[y * width + x] = HSVtoRGB(hue, 1.0f, 1.0f);
                }
            }

            BitBlt(hScreen, 0, 0, width, height, hMem, 0, 0, SRCERASE); // GDI fast blit
            frame += 2;
            Sleep(1);
        }

        DeleteObject(hBmp);
        DeleteDC(hMem);
        ReleaseDC(NULL, hScreen);
        return 0;
    }
}
namespace gdis7 {

    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;

    // HSV ↔ RGB conversions
    TRGBQUAD HSVtoRGB(float h, float s, float v) {
        TRGBQUAD rgb;
        float c = v * s;
        float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
        float m = v - c;
        float r = 0, g = 0, b = 0;
        if (h < 60)      r = c, g = x, b = 0;
        else if (h < 120)  r = x, g = c, b = 0;
        else if (h < 180)  r = 0, g = c, b = x;
        else if (h < 240)  r = 0, g = x, b = c;
        else if (h < 300)  r = x, g = 0, b = c;
        else             r = c, g = 0, b = x;
        rgb.r = (BYTE)((r + m) * 255);
        rgb.g = (BYTE)((g + m) * 255);
        rgb.b = (BYTE)((b + m) * 255);
        rgb.a = 0;
        return rgb;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        HDC hdcScreen = GetDC(NULL);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pixels;
        HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcMem, hBitmap);

        TRGBQUAD* buf = (TRGBQUAD*)pixels;

        int t = 0;
        while (1) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // XOR + plasma + rotation + fractalish formula
                    float fx = (x - width / 2.0f) / 50.0f;
                    float fy = (y - height / 2.0f) / 50.0f;
                    float h = fmodf(360.0f * (sin(fx * fx - fy * fy + t * 0.01f) + 1) / 2, 360.0f);
                    float s = 1.0f;
                    float v = 0.5f + 0.5f * sinf(fx * fx + fy * fy + t * 0.02f);
                    buf[y * width + x] = HSVtoRGB(h, s, v);
                }
            }
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
            t++;
            Sleep(1);
        }

        DeleteDC(hdcMem);
        DeleteObject(hBitmap);
        ReleaseDC(NULL, hdcScreen);
        return 0;
    }
}
namespace gdis8 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get full screen size
        int screenX = GetSystemMetrics(SM_CXSCREEN);
        int screenY = GetSystemMetrics(SM_CYSCREEN);

        // Get desktop device context
        HDC hdc = GetDC(NULL);

        // Seed random
        srand((unsigned int)time(0));

        // Variables for BitBlt pixelated cubes
        const int cubeSize = 20;

        while (1) {
            // Draw a rainbow circle at random position
            int cx = rand() % screenX;
            int cy = rand() % screenY;
            int radius = 50 + rand() % 100;

            for (int y = -radius; y <= radius; y++) {
                for (int x = -radius; x <= radius; x++) {
                    int px = cx + x;
                    int py = cy + y;
                    if (px < 0 || py < 0 || px >= screenX || py >= screenY)
                        continue;

                    if (x * x + y * y <= radius * radius) {
                        COLORREF color = RGB(
                            (x + radius) * 255 / (2 * radius),
                            (y + radius) * 255 / (2 * radius),
                            255 - ((x + radius) * 255 / (2 * radius))
                        );
                        SetPixel(hdc, px, py, color);
                    }
                }
            }

            // Draw pixelated cubes using BitBlt
            for (int i = 0; i < 10; i++) {
                int cubeX = rand() % (screenX - cubeSize);
                int cubeY = rand() % (screenY - cubeSize);

                HDC memDC = CreateCompatibleDC(hdc);
                HBITMAP memBitmap = CreateCompatibleBitmap(hdc, cubeSize, cubeSize);
                HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

                // Fill with random pixels
                for (int py = 0; py < cubeSize; py++) {
                    for (int px = 0; px < cubeSize; px++) {
                        COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);
                        SetPixel(memDC, px, py, color);
                    }
                }

                // Copy to screen
                BitBlt(hdc, cubeX, cubeY, cubeSize, cubeSize, memDC, 0, 0, SRCCOPY);

                SelectObject(memDC, oldBitmap);
                DeleteObject(memBitmap);
                DeleteDC(memDC);
            }

            Sleep(1);
        }

        ReleaseDC(NULL, hdc);
        return 0;
    }
    DWORD WINAPI RGDI2(LPVOID lpParam)
    {
        HDC screen = GetDC(nullptr);             // Full desktop DC
        int W = GetSystemMetrics(SM_CXSCREEN);
        int H = GetSystemMetrics(SM_CYSCREEN);

        // Compatible buffer for PlgBlt / BitBlt effects
        HDC buffer = CreateCompatibleDC(screen);
        HBITMAP bmp = CreateCompatibleBitmap(screen, W, H);
        SelectObject(buffer, bmp);

        double t = 0.0;

        while (1)
        {
            // Copy current desktop frame into buffer
            BitBlt(buffer, 0, 0, W, H, screen, 0, 0, NOTSRCERASE);

            // === 1. SUPER FAST WAVE BITBLT ===
            int wave = int(10 * sin(t * 2.0));
            BitBlt(screen, wave, 0, W, H, screen, 0, 0, SRCPAINT);
            BitBlt(screen, 0, wave, W, H, screen, 0, 0, SRCAND);

            // === 2. RAINBOW SINE SETPIXEL EFFECT ===
            for (int x = 0; x < W; x += 7)
            {
                double s = sin((x * 0.01) + t * 3.0);
                int y = int((H / 2) + s * (H / 3));

                COLORREF color = RGB(
                    int(127 + 127 * sin(t + x * 0.05)),
                    int(127 + 127 * sin(t + x * 0.07 + 2)),
                    int(127 + 127 * sin(t + x * 0.09 + 4))
                );

                SetPixel(screen, x, y, color);
            }

            // === 3. SPRING-ROTATE PLGBLT ===
            POINT tri[3];
            double a = t * 0.8;

            int cx = W / 2;
            int cy = H / 2;
            double r = 40 * sin(t * 2.5);   // wobble radius

            tri[0].x = cx + int(r * cos(a));
            tri[0].y = cy + int(r * sin(a));

            tri[1].x = W + int(r * sin(a * 1.3));
            tri[1].y = 0 + int(r * cos(a * 1.3));

            tri[2].x = 0 + int(r * sin(a * 0.8));
            tri[2].y = H + int(r * cos(a * 0.8));

            PlgBlt(screen, tri, buffer, 0, 0, W, H, nullptr, 0, 0);

            t += 0.02;
            Sleep(1);
        }

        return 0;
    }
}
namespace gdis9 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get desktop DC
        HDC hdc = GetDC(NULL);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Backbuffer
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(memDC, bmp);

        // Pixel buffer
        TRGBQUAD* buf = new TRGBQUAD[w * h];

        float t = 0.0f;

        while (1) {
            t += 0.03f;
            float cx = w * (0.5f + 0.25f * sinf(t));
            float cy = h * (0.5f + 0.25f * cosf(t));

            // Shader: radial "eye"
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    float dx = x - cx;
                    float dy = y - cy;
                    float dist = sqrtf(dx * dx + dy * dy);

                    float iris = sinf(dist * 0.02f - t) * 0.5f + 0.5f;

                    TRGBQUAD px;
                    px.rgbRed = (BYTE)(200 * iris);
                    px.rgbGreen = (BYTE)(40);
                    px.rgbBlue = (BYTE)(255 - 200 * iris);
                    px.rgbReserved = 0;

                    buf[y * w + x] = px;
                }
            }

            // Copy buffer → GDI bitmap
            SetBitmapBits(bmp, w * h * sizeof(TRGBQUAD), buf);

            // Blit to desktop
            BitBlt(
                hdc, 0, 0, w, h,
                memDC, 0, 0,
                SRCCOPY
            );

            Sleep(1);
        }

        delete[] buf;
        DeleteObject(bmp);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdc);
        return 0;
    }
}
namespace gdis10 {
    typedef struct TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sx = GetSystemMetrics(SM_CXSCREEN);
        int sy = GetSystemMetrics(SM_CYSCREEN);

        HDC screen = GetDC(NULL);

        // Create memory buffer (lava shader texture)
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sx;
        bmi.bmiHeader.biHeight = -sy;          // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* buf = nullptr;
        HDC memdc = CreateCompatibleDC(screen);
        HBITMAP hbmp = CreateDIBSection(memdc, &bmi, DIB_RGB_COLORS,
            (void**)&buf, NULL, 0);

        SelectObject(memdc, hbmp);

        DWORD t = 0;

        while (1) {
            // --- Generate a fake “red lava volcano” noise field -------------
            for (int y = 0; y < sy; y++) {
                for (int x = 0; x < sx; x++) {

                    // Some wavy red noise
                    BYTE r = (BYTE)(
                        (
                            (int)(128 + 127 * sin((x + t) * 0.01)) ^
                            (int)(128 + 127 * sin((y + t) * 0.02))
                            ) & 0xFF
                        );

                    // Lava: strong red, weak green/blue
                    buf[y * sx + x].r = r;
                    buf[y * sx + x].g = r / 6;
                    buf[y * sx + x].b = 0;
                    buf[y * sx + x].a = 0;
                }
            }

            // --- SRCERASE onto the real desktop ---------------------------
            BitBlt(screen, 0, 0, sx, sy, memdc, 0, 0, SRCERASE);

            t++;
            Sleep(1);
        }

        // never reached
        DeleteObject(hbmp);
        DeleteDC(memdc);
        ReleaseDC(NULL, screen);

        return 0;
    }
}
namespace gdis11 {
    typedef struct tagTRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        srand((unsigned)time(0));

        // Fullscreen dimensions
        int W = GetSystemMetrics(SM_CXSCREEN);
        int H = GetSystemMetrics(SM_CYSCREEN);

        // Desktop DC (no window needed)
        HDC hDesk = GetDC(NULL);

        // Create back-buffer
        HDC hMem = CreateCompatibleDC(hDesk);
        HBITMAP hBmp = CreateCompatibleBitmap(hDesk, W, H);
        SelectObject(hMem, hBmp);

        // Working buffer
        TRGBQUAD* buf = new TRGBQUAD[W * H];

        while (1) {
            // Generate glitch noise (TV static)
            for (int i = 0; i < W * H; i++) {
                buf[i].rgbRed = rand() % 256;
                buf[i].rgbGreen = rand() % 256;
                buf[i].rgbBlue = rand() % 256;
                buf[i].rgbReserved = 0;
            }

            // Push buffer into the bitmap
            SetBitmapBits(hBmp, W * H * sizeof(TRGBQUAD), buf);

            // Glitch effects using BitBlt + SRCERASE
            // 1) Draw static
            BitBlt(hDesk, 0, 0, W, H, hMem, 0, 0, SRCERASE);

            // 2) Shift random strips (digital glitch)
            int stripH = 5 + rand() % 40;
            int y = rand() % (H - stripH);
            int shift = (rand() % 200) - 100;

            BitBlt(hDesk, shift, y, W, stripH, hDesk, 0, y, SRCAND);

            // 3) Some horizontal tearing
            int y2 = rand() % H;
            BitBlt(hDesk, 0, y2, W, 2, hDesk, 0, y2, SRCPAINT);

            Sleep(1);
        }

        // Cleanup (never reached)
        delete[] buf;
        DeleteObject(hBmp);
        DeleteDC(hMem);
        ReleaseDC(NULL, hDesk);

        return 0;
    }
}