#pragma once
namespace gdis1 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get screen dimensions
        int screenW = GetSystemMetrics(SM_CXSCREEN);
        int screenH = GetSystemMetrics(SM_CYSCREEN);

        // Get the desktop device context
        HDC hScreen = GetDC(NULL);
        HDC hMem = CreateCompatibleDC(hScreen);
        HBITMAP hBmp = CreateCompatibleBitmap(hScreen, screenW, screenH);
        SelectObject(hMem, hBmp);

        srand((unsigned)time(NULL));

        while (1) {
            // Copy entire screen into memory DC
            BitBlt(hMem, 0, 0, screenW, screenH, hScreen, 0, 0, SRCCOPY);

            // Split-wave distortion
            for (int y = 0; y < screenH; y++) {
                int wave = (int)(sin(y / 30.0 + GetTickCount() / 400.0) * 20);
                BitBlt(hScreen, wave, y, screenW, 1, hMem, 0, y, SRCCOPY);
            }

            // Rainbow pixel overlay
            for (int i = 0; i < 500; i++) {
                int x = rand() % screenW;
                int y = rand() % screenH;
                COLORREF color = RGB(
                    (int)(127 + 127 * sin((x + GetTickCount()) / 100.0)),
                    (int)(127 + 127 * sin((y + GetTickCount()) / 150.0)),
                    (int)(127 + 127 * sin((x + y + GetTickCount()) / 200.0))
                );
                SetPixel(hScreen, x, y, color);
            }

            Sleep(1);
        }

        // Cleanup (unreachable, but good practice)
        DeleteObject(hBmp);
        DeleteDC(hMem);
        ReleaseDC(NULL, hScreen);
        return 0;
    }
}
namespace gdis2 {
    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sx = GetSystemMetrics(SM_CXSCREEN);
        int sy = GetSystemMetrics(SM_CYSCREEN);

        HDC hScreen = GetDC(0);
        HDC hMemDC = CreateCompatibleDC(hScreen);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sx;
        bmi.bmiHeader.biHeight = -sy; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* buf = nullptr;
        HBITMAP hBmp = CreateDIBSection(hMemDC, &bmi, DIB_RGB_COLORS, (void**)&buf, 0, 0);
        SelectObject(hMemDC, hBmp);

        double t = 0;
        while (1) {
            t += 0.02;
            for (int y = 0; y < sy; y++) {
                for (int x = 0; x < sx; x++) {
                    double nx = (x - sx / 2.0) / sy;
                    double ny = (y - sy / 2.0) / sy;

                    // Rotate
                    double r = sqrt(nx * nx + ny * ny);
                    double a = atan2(ny, nx) + t;

                    // Sierpinski fractal style pattern via bitwise mod
                    int fx = int((nx * 512 + 512) * 0.5);
                    int fy = int((ny * 512 + 512) * 0.5);
                    int f = ((fx & fy) ^ (int(t * 100))) & 255;

                    // Ripple effect on radius
                    double ripple = sin(r * 20.0 - t * 5.0) * 0.5 + 0.5;

                    buf[y * sx + x].r = (BYTE)(ripple * f);
                    buf[y * sx + x].g = (BYTE)((1.0 - ripple) * f);
                    buf[y * sx + x].b = (BYTE)((ripple * 0.5 + 0.5) * f);
                    buf[y * sx + x].a = 0;
                }
            }

            BitBlt(hScreen, 0, 0, sx, sy, hMemDC, 0, 0, SRCCOPY);
            Sleep(1);
        }

        DeleteObject(hBmp);
        DeleteDC(hMemDC);
        ReleaseDC(0, hScreen);
        return 0;
    }
}
namespace gdis3 {
    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sx = GetSystemMetrics(SM_CXSCREEN);
        int sy = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(0);
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc, sx, sy);
        SelectObject(memDC, bmp);

        TRGBQUAD* buf = new TRGBQUAD[sx * sy];
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sx;
        bmi.bmiHeader.biHeight = -sy;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        float angle = 0;

        while (1) {
            angle += 0.02f;
            float ca = cos(angle), sa = sin(angle);

            // Clear background
            for (int i = 0; i < sx * sy; i++)
                buf[i] = { 0, 0, 0, 0 };

            // Draw rotating cube
            float cube[8][3] = {
                {-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},
                {-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}
            };
            POINT pts[8];
            for (int i = 0; i < 8; i++) {
                float x = cube[i][0], y = cube[i][1], z = cube[i][2];
                float rx = x * ca + z * sa;
                float rz = z * ca - x * sa;
                float ry = y;
                int px = int((rx / (rz + 3)) * sy / 2 + sx / 2);
                int py = int((ry / (rz + 3)) * sy / 2 + sy / 2);
                pts[i] = { px, py };
            }

            int edges[12][2] = {
                {0,1},{1,2},{2,3},{3,0},
                {4,5},{5,6},{6,7},{7,4},
                {0,4},{1,5},{2,6},{3,7}
            };

            for (auto& e : edges) {
                int x1 = pts[e[0]].x, y1 = pts[e[0]].y;
                int x2 = pts[e[1]].x, y2 = pts[e[1]].y;
                int dx = abs(x2 - x1), dy = abs(y2 - y1);
                int sx1 = x1 < x2 ? 1 : -1, sy1 = y1 < y2 ? 1 : -1;
                int err = dx - dy;
                while (true) {
                    if (x1 >= 0 && x1 < sx && y1 >= 0 && y1 < sy) {
                        int idx = y1 * sx + x1;
                        buf[idx] = { 255, 255, 255, 0 };
                    }
                    if (x1 == x2 && y1 == y2) break;
                    int e2 = 2 * err;
                    if (e2 > -dy) { err -= dy; x1 += sx1; }
                    if (e2 < dx) { err += dx; y1 += sy1; }
                }
            }

            // Ripple effect
            for (int y = 0; y < sy; y++)
                for (int x = 0; x < sx; x++) {
                    int idx = y * sx + x;
                    float fx = x - sx / 2.0f, fy = y - sy / 2.0f;
                    float d = sqrtf(fx * fx + fy * fy);
                    BYTE v = BYTE(64 + 64 * sinf(d * 0.02f - angle * 3));
                    buf[idx].b = (buf[idx].b / 2) + v / 2;
                    buf[idx].r = (buf[idx].r / 2) + v / 2;
                }

            // Blit to screen
            StretchDIBits(memDC, 0, 0, sx, sy, 0, 0, sx, sy, buf, &bmi, DIB_RGB_COLORS, SRCCOPY);
            BitBlt(hdc, 0, 0, sx, sy, memDC, 0, 0, SRCCOPY);

            Sleep(1);
        }

        delete[] buf;
        DeleteObject(bmp);
        DeleteDC(memDC);
        ReleaseDC(0, hdc);
        return 0;
    }
}
namespace gdis4 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Fullscreen dimensions
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Get desktop DC
        HDC hdc = GetDC(NULL);

        double t = 0.0;

        while (1) {
            // Draw sine wave rainbow
            for (int x = 0; x < width; x++) {
                double angle = (x + t) * 0.05;
                int y = (int)(height / 2 + sin(angle) * height / 4);

                BYTE r = (BYTE)(127.5 * (sin(angle) + 1));
                BYTE g = (BYTE)(127.5 * (sin(angle + 2.0) + 1));
                BYTE b = (BYTE)(127.5 * (sin(angle + 4.0) + 1));

                SetPixel(hdc, x, y, RGB(r, g, b));
            }

            // Rotate/distort screen effect with PlgBlt
            POINT plg[3] = { {0,0}, {width - 1, 10}, {10, height - 1} };
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

            BitBlt(memDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
            PlgBlt(hdc, plg, memDC, 0, 0, width, height, NULL, 0, 0);

            SelectObject(memDC, oldBitmap);
            DeleteObject(hBitmap);
            DeleteDC(memDC);

            t += 1.0;
            Sleep(1);
        }

        ReleaseDC(NULL, hdc);
        return 0;
    }
}
namespace gdis5 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get full screen dimensions
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // Get screen DC
        HDC hdcScreen = GetDC(NULL);

        // Create memory DC and DIB section
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = screenWidth;
        bmi.bmiHeader.biHeight = -screenHeight; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits = nullptr;
        HBITMAP hbmMem = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hdcMem, hbmMem);

        TRGBQUAD* pixels = (TRGBQUAD*)pBits;

        float t = 0.0f;

        while (1) {
            // Precompute center
            float cx = screenWidth / 2.0f;
            float cy = screenHeight / 2.0f;

            // Generate fractal ripple pixels
            for (int y = 0; y < screenHeight; y++) {
                float fy = y - cy;
                for (int x = 0; x < screenWidth; x++) {
                    float fx = x - cx;
                    float dist = sqrtf(fx * fx + fy * fy);

                    // Ripple function
                    float value = sinf(dist * 0.05f - t) * 0.5f + 0.5f;

                    // Assign color
                    TRGBQUAD color;
                    color.rgbRed = (BYTE)(value * 255);
                    color.rgbGreen = (BYTE)((1 - value) * 255);
                    color.rgbBlue = (BYTE)((0.5f + 0.5f * sinf(t)) * 255);
                    color.rgbReserved = 0;

                    pixels[y * screenWidth + x] = color;
                }
            }

            // Draw to screen
            BitBlt(hdcScreen, 0, 0, screenWidth, screenHeight, hdcMem, 0, 0, SRCCOPY);

            t += 0.15f; // speed up animation
            Sleep(1);   // small delay
        }

        // Cleanup (never reached)
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

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

    // HSV to RGB conversion
    TRGBQUAD HSVtoRGB(float h, float s, float v) {
        float r, g, b;

        int i = int(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
        }

        TRGBQUAD rgb;
        rgb.rgbRed = (uint8_t)(r * 255);
        rgb.rgbGreen = (uint8_t)(g * 255);
        rgb.rgbBlue = (uint8_t)(b * 255);
        rgb.rgbReserved = 0;
        return rgb;
    }

    // Optional: RGB to HSV
    void RGBtoHSV(const TRGBQUAD& rgb, float& h, float& s, float& v) {
        float r = rgb.rgbRed / 255.f;
        float g = rgb.rgbGreen / 255.f;
        float b = rgb.rgbBlue / 255.f;

        float max = fmaxf(r, fmaxf(g, b));
        float min = fminf(r, fminf(g, b));
        v = max;

        float delta = max - min;
        s = (max == 0) ? 0 : delta / max;

        if (delta == 0) h = 0;
        else if (max == r) h = (g - b) / delta + (g < b ? 6 : 0);
        else if (max == g) h = (b - r) / delta + 2;
        else h = (r - g) / delta + 4;

        h /= 6;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Full screen dimensions
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        HDC hdcScreen = GetDC(NULL);

        // Create a compatible bitmap and DC
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits = nullptr;
        HBITMAP hbm = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hdcMem, hbm);

        TRGBQUAD* pixels = (TRGBQUAD*)pBits;

        int frame = 0;

        while (1) {
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    // MandelXOR ripple effect (simple variant)
                    float fx = (float)x / width - 0.5f;
                    float fy = (float)y / height - 0.5f;
                    float dist = sqrtf(fx * fx + fy * fy);
                    float hue = fmodf(dist * 10.0f - frame * 0.01f, 1.0f);
                    float sat = 1.0f;
                    float val = 1.0f - 0.5f * sinf(dist * 20.0f - frame * 0.05f);

                    pixels[y * width + x] = HSVtoRGB(hue, sat, val);
                }
            }

            // Blit to screen
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

            Sleep(1);
            ++frame;
        }

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdis7 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;

    // HSV to RGB conversion
    TRGBQUAD HSVtoRGB(float h, float s, float v) {
        float r = 0, g = 0, b = 0;
        int i = int(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        switch (i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        }

        TRGBQUAD color;
        color.rgbRed = BYTE(r * 255);
        color.rgbGreen = BYTE(g * 255);
        color.rgbBlue = BYTE(b * 255);
        color.rgbReserved = 0;
        return color;
    }

    // RGB to HSV conversion
    void RGBtoHSV(TRGBQUAD color, float& h, float& s, float& v) {
        float r = color.rgbRed / 255.0f;
        float g = color.rgbGreen / 255.0f;
        float b = color.rgbBlue / 255.0f;

        float cmax = fmaxf(r, fmaxf(g, b));
        float cmin = fminf(r, fminf(g, b));
        float delta = cmax - cmin;

        if (delta == 0) h = 0;
        else if (cmax == r) h = fmodf(((g - b) / delta), 6.0f) / 6.0f;
        else if (cmax == g) h = (((b - r) / delta) + 2.0f) / 6.0f;
        else h = (((r - g) / delta) + 4.0f) / 6.0f;

        s = (cmax == 0) ? 0 : delta / cmax;
        v = cmax;
    }

    // Mandelbrot iteration
    int mandelbrot(double cr, double ci, int max_iter) {
        double zr = 0, zi = 0;
        int i = 0;
        while (zr * zr + zi * zi < 4.0 && i < max_iter) {
            double tmp = zr * zr - zi * zi + cr;
            zi = 2.0 * zr * zi + ci;
            zr = tmp;
            i++;
        }
        return i;
    }

    // Julia / Johanbrot iteration
    int julia(double zr, double zi, double cr, double ci, int max_iter) {
        int i = 0;
        while (zr * zr + zi * zi < 4.0 && i < max_iter) {
            double tmp = zr * zr - zi * zi + cr;
            zi = 2.0 * zr * zi + ci;
            zr = tmp;
            i++;
        }
        return i;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Fullscreen dimensions
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Create DC and compatible bitmap
        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

        std::vector<TRGBQUAD> pixels(width * height);

        int max_iter = 500;
        double mandelbrotScale = 3.5;
        double mandelbrotX = -0.7, mandelbrotY = 0.0;

        double juliaScale = 2.0;
        double juliaX = 0.0, juliaY = 0.0;

        float t = 0.0f;

        while (true) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Mandelbrot
                    double cr = mandelbrotX + (x - width / 2.0) * mandelbrotScale / width;
                    double ci = mandelbrotY + (y - height / 2.0) * mandelbrotScale / height;
                    int mIter = mandelbrot(cr, ci, max_iter);

                    // Johanbrot (Julia set) animated
                    double zr = (x - width / 2.0) * juliaScale / width;
                    double zi = (y - height / 2.0) * juliaScale / height;
                    double jc = 0.7885 * cos(t);
                    double jd = 0.7885 * sin(t);
                    int jIter = julia(zr, zi, jc, jd, max_iter);

                    float hue = fmodf((mIter + jIter) / 512.0f + t, 1.0f);
                    pixels[y * width + x] = HSVtoRGB(hue, 1.0f, (mIter + jIter) < 2 * max_iter ? 1.0f : 0.0f);
                }
            }

            // Update bitmap
            BITMAPINFO bmi{};
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height; // top-down
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            SetDIBits(hdcMem, hBitmap, 0, height, pixels.data(), &bmi, DIB_RGB_COLORS);

            // Blit to screen
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

            t += 0.005f;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // Cleanup
        SelectObject(hdcMem, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return(0);
    }
}
namespace gdis8 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdc = GetDC(0);
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);
        HDC mem = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(mem, bmp);

        int t = 0;
        while (1) {
            // Copy screen
            BitBlt(mem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);

            // Swirl distort with PlgBlt
            POINT pts[3];
            double angle = t * 0.02;
            int cx = sw / 2;
            int cy = sh / 2;
            int r = (int)(100 * sin(t * 0.01)) + 200;

            pts[0].x = cx + (int)(r * cos(angle));
            pts[0].y = cy + (int)(r * sin(angle));
            pts[1].x = cx + (int)(r * cos(angle + 2.09));
            pts[1].y = cy + (int)(r * sin(angle + 2.09));
            pts[2].x = cx + (int)(r * cos(angle + 4.18));
            pts[2].y = cy + (int)(r * sin(angle + 4.18));

            PlgBlt(hdc, pts, mem, 0, 0, sw, sh, 0, 0, 0);

            // Rainbow unicorn sparkle with SetPixel
            for (int i = 0; i < 2000; i++) {
                int x = rand() % sw;
                int y = rand() % sh;
                COLORREF c = RGB(
                    (sin((x + t) * 0.02) + 1) * 127,
                    (sin((y + t) * 0.03) + 1) * 127,
                    (sin((x + y + t) * 0.04) + 1) * 127);
                SetPixel(hdc, x, y, c);
            }

            // Swirl Tidal Waves via BitBlt rotation
            int offset = (int)(20 * sin(t * 0.05));
            BitBlt(hdc, offset, offset, sw - offset * 2, sh - offset * 2, hdc, 0, 0, SRCCOPY);

            t++;
            Sleep(1);
        }

        DeleteObject(bmp);
        DeleteDC(mem);
        ReleaseDC(0, hdc);
        return 0;
    }
}
namespace gdis9 {
    // Custom RGB structure
    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;

    // HSV → RGB conversion
    TRGBQUAD HSVtoRGB(double h, double s, double v) {
        double r, g, b;
        int i = int(h * 6);
        double f = h * 6 - i;
        double p = v * (1 - s);
        double q = v * (1 - f * s);
        double t = v * (1 - (1 - f) * s);

        switch (i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        }

        TRGBQUAD c;
        c.r = (BYTE)(r * 255);
        c.g = (BYTE)(g * 255);
        c.b = (BYTE)(b * 255);
        c.a = 0;
        return c;
    }

    // RGB → HSV (not used here, but included per request)
    void RGBtoHSV(BYTE r, BYTE g, BYTE b, double& h, double& s, double& v) {
        double rf = r / 255.0, gf = g / 255.0, bf = b / 255.0;
        double max = fmax(rf, fmax(gf, bf));
        double min = fmin(rf, fmin(gf, bf));
        double d = max - min;

        v = max;
        s = max == 0 ? 0 : d / max;

        if (max == min)
            h = 0;
        else if (max == rf)
            h = fmod(((gf - bf) / d + (gf < bf ? 6 : 0)), 6) / 6.0;
        else if (max == gf)
            h = (((bf - rf) / d + 2) / 6.0);
        else
            h = (((rf - gf) / d + 4) / 6.0);
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(0);
        HDC memdc = CreateCompatibleDC(hdc);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sw;
        bmi.bmiHeader.biHeight = -sh; // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;
        HBITMAP hbm = CreateDIBSection(memdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(memdc, hbm);

        double t = 0;
        while (1) {
            for (int y = 0; y < sh; ++y) {
                for (int x = 0; x < sw; ++x) {
                    double nx = (double)x / sw - 0.5;
                    double ny = (double)y / sh - 0.5;
                    double dist = sqrt(nx * nx + ny * ny);
                    double angle = atan2(ny, nx);
                    double v = 0.5 + 0.5 * sin(10 * dist - t * 0.1 + sin(angle * 3 + t * 0.02));
                    double h = fmod(dist * 3 + t * 0.01, 1.0);
                    TRGBQUAD c = HSVtoRGB(h, 1.0, v);
                    pixels[y * sw + x] = c;
                }
            }

            BitBlt(hdc, 0, 0, sw, sh, memdc, 0, 0, SRCCOPY);
            t += 1.0;
            Sleep(1);
        }

        DeleteObject(hbm);
        DeleteDC(memdc);
        ReleaseDC(0, hdc);
        return 0;
    }
}
namespace gdis10 {
    // Typedef for pixel (like RGBQUAD)
    typedef struct TRGBQUAD {
        BYTE b, g, r, a;
    } TRGBQUAD;

    // HSV to RGB conversion
    void HSVtoRGB(double h, double s, double v, BYTE& r, BYTE& g, BYTE& b) {
        double c = v * s;
        double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
        double m = v - c;
        double r1, g1, b1;
        if (h < 60) { r1 = c; g1 = x; b1 = 0; }
        else if (h < 120) { r1 = x; g1 = c; b1 = 0; }
        else if (h < 180) { r1 = 0; g1 = c; b1 = x; }
        else if (h < 240) { r1 = 0; g1 = x; b1 = c; }
        else if (h < 300) { r1 = x; g1 = 0; b1 = c; }
        else { r1 = c; g1 = 0; b1 = x; }
        r = (BYTE)((r1 + m) * 255);
        g = (BYTE)((g1 + m) * 255);
        b = (BYTE)((b1 + m) * 255);
    }

    // RGB to HSV (not strictly needed but included)
    void RGBtoHSV(BYTE r, BYTE g, BYTE b, double& h, double& s, double& v) {
        double r1 = r / 255.0, g1 = g / 255.0, b1 = b / 255.0;
        double mx = max(r1, max(g1, b1));
        double mn = min(r1, min(g1, b1));
        double d = mx - mn;
        if (d == 0) h = 0;
        else if (mx == r1) h = 60 * fmod(((g1 - b1) / d), 6);
        else if (mx == g1) h = 60 * (((b1 - r1) / d) + 2);
        else h = 60 * (((r1 - g1) / d) + 4);
        if (h < 0) h += 360;
        s = (mx == 0) ? 0 : (d / mx);
        v = mx;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(hdc);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = NULL;
        HBITMAP bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(memDC, bmp);

        DWORD t = 0;
        while (1) {
            t++;
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    double H = fmod((x + y + t) * 0.1, 360.0);
                    double S = 1.0;
                    double V = 1.0;
                    BYTE R, G, B;
                    HSVtoRGB(H, S, V, R, G, B);
                    int i = y * w + x;
                    pixels[i].r = R ^ (x & 255);
                    pixels[i].g = G ^ (y & 255);
                    pixels[i].b = B ^ ((x ^ y) & 255);
                    pixels[i].a = 0;
                }
            }
            BitBlt(hdc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
            Sleep(1);
        }

        DeleteObject(bmp);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdc);
        return 0;
    }
}
namespace gdis11 {
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hScreen = GetDC(0);
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC mem = CreateCompatibleDC(hScreen);
        HBITMAP bmp = CreateCompatibleBitmap(hScreen, sw, sh);
        SelectObject(mem, bmp);

        while (1) {
            BitBlt(mem, 0, 0, sw, sh, hScreen, 0, 0, SRCCOPY);

            // Slight rotation (PlgBlt)
            POINT p[3];
            static double t = 0.0;
            double s = sin(t) * 50, c = cos(t) * 50;
            p[0].x = (LONG)(s);           p[0].y = (LONG)(c);
            p[1].x = sw - (LONG)(c);      p[1].y = (LONG)(s);
            p[2].x = (LONG)(c);           p[2].y = sh - (LONG)(s);
            PlgBlt(hScreen, p, mem, 0, 0, sw, sh, 0, 0, 0);

            // Rainbow SetPixel chaos
            for (int i = 0; i < 3000; i++) {
                int x = rand() % sw;
                int y = rand() % sh;
                COLORREF col = RGB(
                    (BYTE)(128 + 127 * sin(x * 0.02 + t)),
                    (BYTE)(128 + 127 * sin(y * 0.03 + t + 2)),
                    (BYTE)(128 + 127 * sin((x + y) * 0.01 + t + 4))
                );
                SetPixel(hScreen, x, y, col);
            }

            // BubbleWave via BitBlt offset distortions
            for (int y = 0; y < sh; y += 10) {
                int offset = (int)(10 * sin(y * 0.02 + t));
                BitBlt(hScreen, offset, y, sw, 10, hScreen, 0, y, SRCCOPY);
            }

            t += 0.05;
            Sleep(1);
        }

        DeleteDC(mem);
        DeleteObject(bmp);
        ReleaseDC(0, hScreen);
        return 0;
    }
}
namespace gdis12 {
    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;

    struct HSV {
        float h, s, v;
    };

    // Convert HSV → RGB
    TRGBQUAD HSVtoRGB(float h, float s, float v) {
        TRGBQUAD rgb;
        int i = int(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);
        float r, g, b;
        switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
        }
        rgb.r = (BYTE)(r * 255);
        rgb.g = (BYTE)(g * 255);
        rgb.b = (BYTE)(b * 255);
        rgb.a = 0;
        return rgb;
    }

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(0);
        HDC memDC = CreateCompatibleDC(hdc);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sw;
        bmi.bmiHeader.biHeight = -sh; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* buf = nullptr;
        HBITMAP hbm = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, (void**)&buf, 0, 0);
        SelectObject(memDC, hbm);

        double t = 0;
        while (1) {
            for (int y = 0; y < sh; y++) {
                for (int x = 0; x < sw; x++) {
                    float nx = (float)x / sw - 0.5f;
                    float ny = (float)y / sh - 0.5f;

                    float d = sqrtf(nx * nx + ny * ny);
                    float angle = atan2f(ny, nx);

                    float hue = fmodf((angle / 3.1415926f + 1.0f) * 0.5f + 0.5f * sinf(d * 12.0f - t * 3.0f), 1.0f);
                    float sat = 1.0f;
                    float val = 0.5f + 0.5f * sinf(d * 20.0f - t * 5.0f + (x ^ y) * 0.00005f);

                    buf[y * sw + x] = HSVtoRGB(hue, sat, val);
                }
            }

            BitBlt(hdc, 0, 0, sw, sh, memDC, 0, 0, SRCCOPY);
            t += 0.03;
            Sleep(1);
        }

        return 0;
    }
}
namespace gdis13 {
    typedef struct {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;

    // Convert HSV → RGB (0–255)
    TRGBQUAD HSVtoRGB(float h, float s, float v) {
        TRGBQUAD rgb = { 0, 0, 0, 0 };
        float c = v * s;
        float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
        float m = v - c;
        float r_, g_, b_;

        if (h < 60) { r_ = c; g_ = x; b_ = 0; }
        else if (h < 120) { r_ = x; g_ = c; b_ = 0; }
        else if (h < 180) { r_ = 0; g_ = c; b_ = x; }
        else if (h < 240) { r_ = 0; g_ = x; b_ = c; }
        else if (h < 300) { r_ = x; g_ = 0; b_ = c; }
        else { r_ = c; g_ = 0; b_ = x; }

        rgb.r = (BYTE)((r_ + m) * 255);
        rgb.g = (BYTE)((g_ + m) * 255);
        rgb.b = (BYTE)((b_ + m) * 255);
        rgb.a = 0;
        return rgb;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC hScreen = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, sw, sh);
        SelectObject(hMemDC, hBitmap);

        TRGBQUAD* buffer = new TRGBQUAD[sw * sh];
        srand((unsigned)time(0));

        float hue = 0.0f;

        while (1) {
            hue += 2.0f;
            if (hue > 360.0f) hue = 0.0f;

            // Generate rainbow glitch pixels
            for (int y = 0; y < sh; y++) {
                for (int x = 0; x < sw; x++) {
                    float h = fmodf(hue + (float)(x + y) * 0.1f, 360.0f);
                    TRGBQUAD color = HSVtoRGB(h, 1.0f, 1.0f);

                    // glitch: random flicker effect
                    if (rand() % 50 == 0) {
                        color.r = rand() % 256;
                        color.g = rand() % 256;
                        color.b = rand() % 256;
                    }

                    buffer[y * sw + x] = color;
                }
            }

            // Push to screen via GDI
            SetBitmapBits(hBitmap, sw * sh * sizeof(TRGBQUAD), buffer);
            BitBlt(hScreen, 0, 0, sw, sh, hMemDC, 0, 0, SRCCOPY);

            Sleep(1);
        }

        // Never reached
        delete[] buffer;
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreen);

        return 0;
    }
}