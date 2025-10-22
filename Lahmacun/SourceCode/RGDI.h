#pragma once
namespace gdis1 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get screen resolution
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Get desktop device context
        HDC screenDC = GetDC(NULL);

        // Create memory DC and DIB section for direct pixel access
        HDC memDC = CreateCompatibleDC(screenDC);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pixels = NULL;
        HBITMAP hDIB = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
        SelectObject(memDC, hDIB);

        DWORD t = 0;
        while (1) {
            TRGBQUAD* p = (TRGBQUAD*)pixels;

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    float dx = x - w / 2.0f;
                    float dy = y - h / 2.0f;
                    float dist = sqrtf(dx * dx + dy * dy);
                    float angle = atan2f(dy, dx);

                    float wave = sinf(dist / 20.0f - t * 0.05f + angle * 3.0f) * 0.5f + 0.5f;

                    BYTE r = (BYTE)(wave * 255);
                    BYTE g = (BYTE)(sinf(angle + t * 0.02f) * 127 + 128);
                    BYTE b = (BYTE)(cosf(dist / 30.0f + t * 0.02f) * 127 + 128);

                    int i = y * w + x;
                    p[i].rgbRed = r;
                    p[i].rgbGreen = g;
                    p[i].rgbBlue = b;
                    p[i].rgbReserved = 0;
                }
            }

            // Draw to screen
            BitBlt(screenDC, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

            Sleep(1);
            t++;
        }

        // Cleanup (unreachable, but good practice)
        DeleteObject(hDIB);
        DeleteDC(memDC);
        ReleaseDC(NULL, screenDC);

        return 0;
    }
}
namespace gdis2 {
    typedef struct {
        unsigned char b, g, r, a;
    } TRGBQUAD;

    // HSL structure
    struct THSL {
        float h, s, l;
    };

    // HSL to RGB conversion
    TRGBQUAD HSLtoRGB(THSL hsl) {
        float c = (1 - fabs(2 * hsl.l - 1)) * hsl.s;
        float x = c * (1 - fabs(fmod(hsl.h / 60.0, 2) - 1));
        float m = hsl.l - c / 2;
        float r = 0, g = 0, b = 0;

        if (hsl.h < 60) { r = c; g = x; b = 0; }
        else if (hsl.h < 120) { r = x; g = c; b = 0; }
        else if (hsl.h < 180) { r = 0; g = c; b = x; }
        else if (hsl.h < 240) { r = 0; g = x; b = c; }
        else if (hsl.h < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        TRGBQUAD rgb;
        rgb.r = (unsigned char)((r + m) * 255);
        rgb.g = (unsigned char)((g + m) * 255);
        rgb.b = (unsigned char)((b + m) * 255);
        rgb.a = 0;
        return rgb;
    }

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC screen = GetDC(0);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        int centerX = w / 2;
        int centerY = h / 2;

        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = 0;
        HDC memDC = CreateCompatibleDC(screen);
        HBITMAP bmp = CreateDIBSection(screen, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(memDC, bmp);

        int t = 0;

        while (1) {
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    int dx = x - centerX;
                    int dy = y - centerY;
                    float dist = sqrtf((float)(dx * dx + dy * dy));
                    float angle = atan2f((float)dy, (float)dx);

                    float ripple = sinf(dist / 10.0f - t * 0.1f);
                    float brightness = (ripple + 1.0f) / 2.0f;

                    THSL hsl;
                    hsl.h = fmodf(t + dist / 2.0f, 360.0f);
                    hsl.s = 1.0f;
                    hsl.l = brightness * 0.5f + 0.25f;

                    pixels[y * w + x] = HSLtoRGB(hsl);
                }
            }

            BitBlt(screen, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
            t++;
            Sleep(1);
        }

        return 0;
    }
}
namespace gdis3 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;

    // Converts HSV (0-360,0-1,0-1) to RGB (0-255)
    void HSVtoRGB(double h, double s, double v, TRGBQUAD* rgb) {
        double c = v * s;
        double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
        double m = v - c;
        double r = 0, g = 0, b = 0;

        if (h < 60) { r = c; g = x; b = 0; }
        else if (h < 120) { r = x; g = c; b = 0; }
        else if (h < 180) { r = 0; g = c; b = x; }
        else if (h < 240) { r = 0; g = x; b = c; }
        else if (h < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        rgb->rgbRed = (BYTE)((r + m) * 255);
        rgb->rgbGreen = (BYTE)((g + m) * 255);
        rgb->rgbBlue = (BYTE)((b + m) * 255);
        rgb->rgbReserved = 0;
    }

    // Converts RGB (0-255) to HSV (0-360,0-1,0-1)
    void RGBtoHSV(TRGBQUAD* rgb, double* h, double* s, double* v) {
        double r = rgb->rgbRed / 255.0;
        double g = rgb->rgbGreen / 255.0;
        double b = rgb->rgbBlue / 255.0;

        double max = r > g ? (r > b ? r : b) : (g > b ? g : b);
        double min = r < g ? (r < b ? r : b) : (g < b ? g : b);
        double delta = max - min;

        *v = max;

        if (max == 0) {
            *s = 0;
            *h = 0;
            return;
        }

        *s = delta / max;

        if (delta == 0) {
            *h = 0;
        }
        else if (max == r) {
            *h = 60 * fmod(((g - b) / delta), 6);
        }
        else if (max == g) {
            *h = 60 * (((b - r) / delta) + 2);
        }
        else {
            *h = 60 * (((r - g) / delta) + 4);
        }

        if (*h < 0) *h += 360;
    }

    // Rotates a 3D point (x,y,z) by angle around Y axis (for spin)
    void rotateY(double x, double y, double z, double angle, double* rx, double* ry, double* rz) {
        double cosA = cos(angle);
        double sinA = sin(angle);
        *rx = x * cosA + z * sinA;
        *ry = y;
        *rz = -x * sinA + z * cosA;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Screen dimensions (fullscreen)
        int screenX = GetSystemMetrics(SM_CXSCREEN);
        int screenY = GetSystemMetrics(SM_CYSCREEN);

        // Get device contexts
        HDC hdcScreen = GetDC(NULL); // entire screen DC
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        // Create a bitmap to draw pixels on
        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = screenX;
        bmi.bmiHeader.biHeight = -screenY; // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* bits = NULL;
        HBITMAP hbm = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);

        if (!hbm) {
            ReleaseDC(NULL, hdcScreen);
            DeleteDC(hdcMem);
            return 1;
        }

        SelectObject(hdcMem, hbm);

        TRGBQUAD* pixels = (TRGBQUAD*)bits;

        double time = 0.0;
        double angle = 0.0;

        // Animation loop
        while (1) {
            for (int y = 0; y < screenY; y++) {
                for (int x = 0; x < screenX; x++) {
                    // Normalize to [-1,1]
                    double nx = 2.0 * x / screenX - 1.0;
                    double ny = 2.0 * y / screenY - 1.0;

                    // Define 3D cube coords with z fixed or varying
                    double z = 0.5 * sin(time + nx * 3.14) * cos(time + ny * 3.14);

                    // Rotate around Y
                    double rx, ry, rz;
                    rotateY(nx, ny, z, angle, &rx, &ry, &rz);

                    // Create plasma effect with ripples and cube rotation
                    double value = sin(10 * rx + time) + cos(10 * ry + time) + sin(10 * rz + time);

                    // Normalize value to [0,1]
                    value = (value + 3) / 6.0;

                    // Map value to HSV hue rotating over time
                    double hue = fmod((value * 360.0 + time * 60.0), 360.0);
                    double saturation = 1.0;
                    double brightness = 1.0;

                    TRGBQUAD color;
                    HSVtoRGB(hue, saturation, brightness, &color);

                    pixels[y * screenX + x] = color;
                }
            }

            // Copy from memDC to screenDC
            BitBlt(hdcScreen, 0, 0, screenX, screenY, hdcMem, 0, 0, SRCCOPY);

            Sleep(1);

            time += 0.01;
            angle += 0.02;
            if (angle > 6.2831853) angle -= 6.2831853; // keep angle in [0,2PI]
        }

        // Cleanup (never reached in this loop, but good practice)
        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdis4 {
    typedef struct TRGBQUAD {
        unsigned char rgbBlue;
        unsigned char rgbGreen;
        unsigned char rgbRed;
        unsigned char rgbReserved;
    } TRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get full screen size
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Get desktop DC (entire screen)
        HDC hdcScreen = GetDC(NULL);

        // Create compatible DC and bitmap
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down bitmap
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits = NULL;

        // Create DIB section to get pointer to pixel bits
        HBITMAP hbm = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);

        if (hbm == NULL || pBits == NULL) {
            ReleaseDC(NULL, hdcScreen);
            DeleteDC(hdcMem);
            return -1;
        }

        SelectObject(hdcMem, hbm);

        TRGBQUAD* pixels = (TRGBQUAD*)pBits;

        int maxIter = 100; // Mandelbrot max iterations

        double cx, cy;
        int i, j, iter;

        double time = 0.0;

        while (1) {
            // Increase time for ripple effect
            time += 0.03;

            // Loop through all pixels
            for (j = 0; j < height; j++) {
                for (i = 0; i < width; i++) {

                    // Normalize coordinates for fractal (centered)
                    double x0 = (double)(i - width / 2) / (width / 4);
                    double y0 = (double)(j - height / 2) / (height / 4);

                    // Mandelbrot iteration
                    double x = 0.0;
                    double y = 0.0;
                    iter = 0;

                    while (x * x + y * y <= 4 && iter < maxIter) {
                        double xtemp = x * x - y * y + x0;
                        y = 2 * x * y + y0;
                        x = xtemp;
                        iter++;
                    }

                    // Penrose-like ripple overlay using sin waves
                    double ripple = sin((double)i * 0.05 + time) + cos((double)j * 0.05 + time * 1.2);

                    // Map iteration to color components
                    unsigned char r = (unsigned char)((iter * 5 + ripple * 30));
                    unsigned char g = (unsigned char)((iter * 2 + ripple * 50));
                    unsigned char b = (unsigned char)((iter * 10 + ripple * 20));

                    // Clamp colors (simple)
                    if (r > 255) r = 255;
                    if (g > 255) g = 255;
                    if (b > 255) b = 255;

                    // Write pixel
                    TRGBQUAD* p = pixels + j * width + i;
                    p->rgbRed = r;
                    p->rgbGreen = g;
                    p->rgbBlue = b;
                    p->rgbReserved = 0;
                }
            }

            // Copy the memory DC to the screen DC (fullscreen)
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

            Sleep(1);
        }

        // Cleanup (never reached here)
        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdis5 {
    typedef struct TRGBQUAD {
        unsigned char rgbBlue;
        unsigned char rgbGreen;
        unsigned char rgbRed;
        unsigned char rgbReserved;
    } TRGBQUAD;

    // Clamp integer helper
    int ClampInt(int val, int minVal, int maxVal) {
        if (val < minVal) return minVal;
        if (val > maxVal) return maxVal;
        return val;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get full screen dimensions
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Get desktop device context (entire screen)
        HDC hdcScreen = GetDC(NULL);

        // Create compatible memory DC
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        // Prepare BITMAPINFO for 32bpp top-down DIBSection
        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;  // Negative for top-down bitmap
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits = NULL;

        // Create DIBSection for pixel buffer
        HBITMAP hbm = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        if (hbm == NULL || pBits == NULL) {
            ReleaseDC(NULL, hdcScreen);
            DeleteDC(hdcMem);
            return -1;
        }

        // Select bitmap into memory DC
        SelectObject(hdcMem, hbm);

        // Cast pixel buffer to TRGBQUAD array
        TRGBQUAD* pixels = (TRGBQUAD*)pBits;

        // Mandelbrot parameters
        int maxIter = 80;

        // Time variable for animation
        double time = 0.0;

        int i, j, iter;

        while (1) {
            time += 0.05;

            for (j = 0; j < height; j++) {
                for (i = 0; i < width; i++) {
                    // Map pixel coordinate to normalized complex plane [-2..2]
                    double cx = (double)(i - width / 2) / (width / 4);
                    double cy = (double)(j - height / 2) / (height / 4);

                    // Mandelbrot iteration
                    double x = 0.0;
                    double y = 0.0;
                    iter = 0;

                    while ((x * x + y * y <= 4.0) && (iter < maxIter)) {
                        double xtemp = x * x - y * y + cx;
                        y = 2.0 * x * y + cy;
                        x = xtemp;
                        iter++;
                    }

                    // Ripple effect with sine waves and Penrose style geometry
                    double ripple = sin(i * 0.04 + time) + cos(j * 0.06 + time * 1.1);

                    // XOR pattern on pixel positions
                    int xorValue = (i ^ j) & 0xFF;

                    // Combine fractal iteration with ripple and XOR pattern
                    int r = ClampInt((int)(iter * 3 + ripple * 40 + xorValue), 0, 255);
                    int g = ClampInt((int)(iter * 2 + ripple * 60 + (xorValue / 2)), 0, 255);
                    int b = ClampInt((int)(iter * 5 + ripple * 30 + xorValue / 3), 0, 255);

                    // Write pixel to buffer
                    TRGBQUAD* p = pixels + j * width + i;
                    p->rgbRed = (unsigned char)r;
                    p->rgbGreen = (unsigned char)g;
                    p->rgbBlue = (unsigned char)b;
                    p->rgbReserved = 0;
                }
            }

            // BitBlt with SRCERASE raster op to screen DC
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCERASE);

            Sleep(1);
        }

        // Cleanup - unreachable in this infinite loop example
        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdis6 {
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get screen dimensions
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Prepare BITMAPINFO for DIB section
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        // Pointer to pixel data
        TRGBQUAD* pixels = nullptr;

        // Create a memory device context and DIB section
        HDC hScreenDC = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hScreenDC);
        HBITMAP hBitmap = CreateDIBSection(hScreenDC, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(hMemDC, hBitmap);

        // Time counter for animation
        int t = 0;

        while (1) {
            // Loop through each pixel and apply a ripple effect
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float dx = x - width / 2.0f;
                    float dy = y - height / 2.0f;
                    float dist = sqrtf(dx * dx + dy * dy);
                    float wave = sinf(dist / 10.0f - t * 0.1f);

                    BYTE val = (BYTE)(wave * 127.0f + 128.0f);

                    int index = y * width + x;
                    pixels[index].rgbRed = val;
                    pixels[index].rgbGreen = val / 2;
                    pixels[index].rgbBlue = 255 - val;
                    pixels[index].rgbReserved = 0;
                }
            }

            // Blit buffer to screen
            BitBlt(hScreenDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);

            // Delay and update time
            Sleep(1);
            t++;
        }

        // Cleanup (never reached due to infinite loop)
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);

        return 0;
    }
}
namespace gdis7 {
    // Define TRGBQUAD manually (32-bit pixel)
    typedef struct TRGBQUAD {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;

    // Clamp helper
    int clamp(int val, int min, int max) {
        return (val < min) ? min : (val > max ? max : val);
    }

    // Convert HSL to RGB
    TRGBQUAD HSLtoRGB(double h, double s, double l) {
        double c = (1.0 - fabs(2.0 * l - 1.0)) * s;
        double x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1.0));
        double m = l - c / 2.0;

        double r = 0.0;
        double g = 0.0;
        double b = 0.0;

        if (h >= 0 && h < 60) {
            r = c; g = x; b = 0;
        }
        else if (h < 120) {
            r = x; g = c; b = 0;
        }
        else if (h < 180) {
            r = 0; g = c; b = x;
        }
        else if (h < 240) {
            r = 0; g = x; b = c;
        }
        else if (h < 300) {
            r = x; g = 0; b = c;
        }
        else {
            r = c; g = 0; b = x;
        }

        TRGBQUAD color;
        color.rgbRed = (BYTE)(clamp((int)((r + m) * 255), 0, 255));
        color.rgbGreen = (BYTE)(clamp((int)((g + m) * 255), 0, 255));
        color.rgbBlue = (BYTE)(clamp((int)((b + m) * 255), 0, 255));
        color.rgbReserved = 0;
        return color;
    }

    // Convert RGB to HSL
    void RGBtoHSL(TRGBQUAD c, double* h, double* s, double* l) {
        double r = c.rgbRed / 255.0;
        double g = c.rgbGreen / 255.0;
        double b = c.rgbBlue / 255.0;

        double max = (r > g) ? r : g;
        if (b > max) max = b;
        double min = (r < g) ? r : g;
        if (b < min) min = b;

        *l = (max + min) / 2.0;

        if (max == min) {
            *s = 0.0;
            *h = 0.0;
        }
        else {
            double d = max - min;
            *s = (*l > 0.5) ? d / (2.0 - max - min) : d / (max + min);

            if (max == r) {
                *h = 60.0 * fmod(((g - b) / d), 6.0);
            }
            else if (max == g) {
                *h = 60.0 * (((b - r) / d) + 2.0);
            }
            else {
                *h = 60.0 * (((r - g) / d) + 4.0);
            }

            if (*h < 0) *h += 360.0;
        }
    }

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Screen size
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Prepare DIB Section
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // Top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = 0;

        // Create GDI surfaces
        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(hdcMem, hBitmap);

        // Center point
        int cx = width / 2;
        int cy = height / 2;

        // Animation frame counter
        int t = 0;

        while (1) {
            for (int y = 0; y < height; y++) {
                int dy = y - cy;
                for (int x = 0; x < width; x++) {
                    int dx = x - cx;
                    int index = y * width + x;

                    // Distance and angle
                    double dist = sqrt((double)(dx * dx + dy * dy));
                    double angle = atan2((double)dy, (double)dx);

                    // XOR ripple pattern + animation
                    double wave = dist + 50.0 * sin(dist / 20.0 - t * 0.1);
                    BYTE intensity = (BYTE)((((int)wave) ^ t) & 0xFF);

                    // Map to HSL
                    double h = fmod(((double)intensity * 2.0 + t), 360.0);  // hue shifts over time
                    double s = 1.0;
                    double l = 0.5;

                    TRGBQUAD color = HSLtoRGB(h, s, l);
                    pixels[index] = color;
                }
            }

            // Blit to screen
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
            Sleep(1);
            t++;
        }

        // Cleanup (never reached)
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdis8 {
#define WIDTH  GetSystemMetrics(SM_CXSCREEN)
#define HEIGHT GetSystemMetrics(SM_CYSCREEN)

    typedef struct TRGBQUAD {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char a;
    } TRGBQUAD;

    typedef struct THSL {
        float h;
        float s;
        float l;
    } THSL;

    // RGB to HSL
    int RGBtoHSL(TRGBQUAD rgb, THSL* hsl) {
        float r = rgb.r / 255.0f;
        float g = rgb.g / 255.0f;
        float b = rgb.b / 255.0f;
        float max = r;
        if (g > max) max = g;
        if (b > max) max = b;
        float min = r;
        if (g < min) min = g;
        if (b < min) min = b;

        hsl->l = (max + min) / 2.0f;
        if (max == min) {
            hsl->h = 0;
            hsl->s = 0;
        }
        else {
            float d = max - min;
            hsl->s = hsl->l > 0.5f ? d / (2.0f - max - min) : d / (max + min);
            if (max == r)
                hsl->h = (g - b) / d + (g < b ? 6.0f : 0.0f);
            else if (max == g)
                hsl->h = (b - r) / d + 2.0f;
            else
                hsl->h = (r - g) / d + 4.0f;
            hsl->h /= 6.0f;
        }
        return 1;
    }

    // HSL to RGB
    int HSLtoRGB(THSL hsl, TRGBQUAD* rgb) {
        float r, g, b;

        if (hsl.s == 0) {
            r = g = b = hsl.l;
        }
        else {
            float q = hsl.l < 0.5f ? hsl.l * (1.0f + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
            float p = 2.0f * hsl.l - q;

            float hk = hsl.h;
            float t[3] = { hk + 1.0f / 3.0f, hk, hk - 1.0f / 3.0f };
            float c[3];
            int i;
            for (i = 0; i < 3; i++) {
                if (t[i] < 0) t[i] += 1;
                if (t[i] > 1) t[i] -= 1;
                if (t[i] < 1.0f / 6.0f)
                    c[i] = p + (q - p) * 6.0f * t[i];
                else if (t[i] < 1.0f / 2.0f)
                    c[i] = q;
                else if (t[i] < 2.0f / 3.0f)
                    c[i] = p + (q - p) * (2.0f / 3.0f - t[i]) * 6.0f;
                else
                    c[i] = p;
            }

            r = c[0]; g = c[1]; b = c[2];
        }

        rgb->r = (unsigned char)(r * 255);
        rgb->g = (unsigned char)(g * 255);
        rgb->b = (unsigned char)(b * 255);
        rgb->a = 0;
        return 1;
    }

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC screen = GetDC(0);
        HDC memDC = CreateCompatibleDC(screen);

        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = WIDTH;
        bmi.bmiHeader.biHeight = -HEIGHT;  // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        unsigned int* pixels = 0;
        HBITMAP hBitmap = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(memDC, hBitmap);

        int t = 0;
        while (1) {
            int x, y;
            for (y = 0; y < HEIGHT; y++) {
                for (x = 0; x < WIDTH; x++) {
                    THSL hsl;
                    TRGBQUAD color;
                    hsl.h = (float)(sin(x * 0.01f + t * 0.01f) + sin(y * 0.02f + t * 0.01f)) * 0.5f;
                    hsl.h = hsl.h - (int)hsl.h;  // wrap
                    if (hsl.h < 0) hsl.h += 1.0f;
                    hsl.s = 1.0f;
                    hsl.l = 0.5f;
                    HSLtoRGB(hsl, &color);
                    pixels[y * WIDTH + x] = (color.r) | (color.g << 8) | (color.b << 16);
                }
            }

            BitBlt(screen, 0, 0, WIDTH, HEIGHT, memDC, 0, 0, SRCERASE);
            Sleep(1);
            t++;
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

    // Screen dimensions (get dynamically)
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Get the screen DC (fullscreen)
        HDC hdcScreen = GetDC(NULL);
        if (!hdcScreen) return -1;

        // Create compatible memory DC
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        // Create DIB section for pixel buffer
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // negative for top-down bitmap
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32; // 4 bytes per pixel (TRGBQUAD)
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pvBits = nullptr;
        HBITMAP hbm = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
        if (!hbm) {
            ReleaseDC(NULL, hdcScreen);
            DeleteDC(hdcMem);
            return -1;
        }

        SelectObject(hdcMem, hbm);

        TRGBQUAD* pixels = (TRGBQUAD*)pvBits;

        // Variables for fractal animation
        int frame = 0;

        while (1) {
            frame++;

            // XOR fractal shader effect
            // For each pixel, color = (x ^ y ^ frame) pattern
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    BYTE val = (BYTE)((x ^ y ^ frame) & 0xFF);
                    TRGBQUAD* px = &pixels[y * width + x];
                    px->rgbRed = val;
                    px->rgbGreen = val;
                    px->rgbBlue = val;
                    px->rgbReserved = 0;
                }
            }

            // Use BitBlt with SRCERASE to draw memory DC to screen DC
            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCERASE);

            Sleep(1);
        }

        // Cleanup (never reached due to infinite loop)
        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdis10 {
    typedef struct {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;

    double Clamp(double x, double a = 0.0, double b = 1.0) {
        return x < a ? a : (x > b ? b : x);
    }

    void HSLtoRGB(double h, double s, double l, TRGBQUAD& rgb) {
        double c = (1.0 - fabs(2.0 * l - 1.0)) * s;
        double x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1));
        double m = l - c / 2.0;

        double r = 0, g = 0, b = 0;
        if (h < 60) { r = c; g = x; b = 0; }
        else if (h < 120) { r = x; g = c; b = 0; }
        else if (h < 180) { r = 0; g = c; b = x; }
        else if (h < 240) { r = 0; g = x; b = c; }
        else if (h < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        rgb.rgbRed = (BYTE)((r + m) * 255);
        rgb.rgbGreen = (BYTE)((g + m) * 255);
        rgb.rgbBlue = (BYTE)((b + m) * 255);
        rgb.rgbReserved = 0;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int screenX = GetSystemMetrics(SM_CXSCREEN);
        int screenY = GetSystemMetrics(SM_CYSCREEN);

        HDC hScreen = GetDC(0);
        HDC hMemDC = CreateCompatibleDC(hScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenX, screenY);
        SelectObject(hMemDC, hBitmap);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = screenX;
        bmi.bmiHeader.biHeight = -screenY;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels;
        HBITMAP hDIB = CreateDIBSection(hMemDC, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(hMemDC, hDIB);

        int frame = 0;

        while (1) {
            for (int y = 0; y < screenY; ++y) {
                for (int x = 0; x < screenX; ++x) {
                    double hue = fmod((x + y + frame) * 0.5, 360.0);
                    double sat = 1.0;
                    double lum = 0.5 + 0.25 * sin((x + frame) * 0.01);
                    TRGBQUAD color;
                    HSLtoRGB(hue, sat, lum, color);
                    pixels[y * screenX + x] = color;
                }
            }

            BitBlt(hScreen, 0, 0, screenX, screenY, hMemDC, 0, 0, SRCERASE);
            Sleep(1);
            frame++;
        }

        // Cleanup (never reached)
        DeleteObject(hDIB);
        DeleteDC(hMemDC);
        ReleaseDC(0, hScreen);

        return 0;
    }
}
