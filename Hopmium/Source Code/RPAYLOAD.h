#pragma once
namespace gdis1 {
    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;

    inline double clamp(double x, double a = 0.0, double b = 1.0) {
        return x < a ? a : x > b ? b : x;
    }

    // HSL → RGB
    TRGBQUAD HSLtoRGB(double h, double s, double l) {
        double r, g, b;
        auto hue2rgb = [&](double p, double q, double t) {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1.0 / 6) return p + (q - p) * 6 * t;
            if (t < 1.0 / 2) return q;
            if (t < 2.0 / 3) return p + (q - p) * (2.0 / 3 - t) * 6;
            return p;
            };
        if (s == 0) r = g = b = l;
        else {
            double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            double p = 2 * l - q;
            r = hue2rgb(p, q, h + 1.0 / 3);
            g = hue2rgb(p, q, h);
            b = hue2rgb(p, q, h - 1.0 / 3);
        }
        TRGBQUAD c = {
            (BYTE)(b * 255),
            (BYTE)(g * 255),
            (BYTE)(r * 255),
            0
        };
        return c;
    }

    // RGB → HSL
    void RGBtoHSL(TRGBQUAD c, double& h, double& s, double& l) {
        double r = c.r / 255.0, g = c.g / 255.0, b = c.b / 255.0;
        double mx = max(r, max(g, b)), mn = min(r, min(g, b));
        l = (mx + mn) / 2;
        if (mx == mn) { h = s = 0; return; }
        double d = mx - mn;
        s = l > 0.5 ? d / (2 - mx - mn) : d / (mx + mn);
        if (mx == r) h = (g - b) / d + (g < b ? 6 : 0);
        else if (mx == g) h = (b - r) / d + 2;
        else h = (r - g) / d + 4;
        h /= 6;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdc = GetDC(0);
        int sx = GetSystemMetrics(SM_CXSCREEN);
        int sy = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sx;
        bmi.bmiHeader.biHeight = -sy;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* buf;
        HBITMAP hbm = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&buf, 0, 0);
        HDC mem = CreateCompatibleDC(hdc);
        SelectObject(mem, hbm);

        double t = 0;
        while (1) {
            t += 0.03;
            for (int y = 0; y < sy; y++) {
                for (int x = 0; x < sx; x++) {
                    double nx = (x - sx / 2.0) / (sy / 2.0);
                    double ny = (y - sy / 2.0) / (sy / 2.0);
                    double dist = sqrt(nx * nx + ny * ny);
                    double angle = atan2(ny, nx) + t * 0.7;
                    double ripple = sin(20 * dist - t * 8) * 0.05;
                    double hue = fmod(angle / (2 * M_PI) + t * 0.1, 1.0);
                    double l = 0.5 + 0.5 * sin(10 * dist - t * 4);
                    double s = 1.0 - dist * 0.5 + ripple;
                    buf[y * sx + x] = HSLtoRGB(hue, clamp(s), clamp(l));
                }
            }
            BitBlt(hdc, 0, 0, sx, sy, mem, 0, 0, SRCCOPY);
            Sleep(1);
        }

        return 0;
    }
}
namespace gdis2 {

    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;

    typedef struct {
        double h, s, l;
    } THSL;

    TRGBQUAD HSLtoRGB(THSL hsl)
    {
        double c = (1.0 - fabs(2.0 * hsl.l - 1.0)) * hsl.s;
        double x = c * (1.0 - fabs(fmod(hsl.h / 60.0, 2) - 1.0));
        double m = hsl.l - c / 2.0;
        double r = 0, g = 0, b = 0;

        if (hsl.h < 60) { r = c; g = x; }
        else if (hsl.h < 120) { r = x; g = c; }
        else if (hsl.h < 180) { g = c; b = x; }
        else if (hsl.h < 240) { g = x; b = c; }
        else if (hsl.h < 300) { r = x; b = c; }
        else { r = c; b = x; }

        TRGBQUAD q;
        q.r = (BYTE)((r + m) * 255);
        q.g = (BYTE)((g + m) * 255);
        q.b = (BYTE)((b + m) * 255);
        q.a = 0;
        return q;
    }

    THSL RGBtoHSL(TRGBQUAD rgb)
    {
        double r = rgb.r / 255.0, g = rgb.g / 255.0, b = rgb.b / 255.0;
        double max = fmax(fmax(r, g), b);
        double min = fmin(fmin(r, g), b);
        double h = 0, s, l = (max + min) / 2.0;
        double d = max - min;
        s = (d == 0) ? 0 : d / (1 - fabs(2 * l - 1));
        if (d != 0) {
            if (max == r) h = 60 * fmod(((g - b) / d), 6);
            else if (max == g) h = 60 * (((b - r) / d) + 2);
            else h = 60 * (((r - g) / d) + 4);
        }
        if (h < 0) h += 360;
        THSL hs; hs.h = h; hs.s = s; hs.l = l;
        return hs;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(0);
        HDC mem = CreateCompatibleDC(hdc);

        TRGBQUAD* pixels;
        BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), sw, -sh, 1, 32, BI_RGB };
        HBITMAP dib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(mem, dib);

        int frame = 0;

        while (1)
        {
            double t = frame * 0.04;
            double cx = sw / 2.0;
            double cy = sh / 2.0;

            for (int y = 0; y < sh; y++)
            {
                for (int x = 0; x < sw; x++)
                {
                    double nx = (x - cx) / (sh / 2.0);
                    double ny = (y - cy) / (sh / 2.0);

                    // Rotating 3D-like swirl
                    double ang = atan2(ny, nx) + t * 0.7;
                    double dist = sqrt(nx * nx + ny * ny);

                    // Sine wave modulation for fractal layers
                    double s1 = sin(8.0 * dist - t * 1.5);
                    double s2 = sin(5.0 * dist + ang * 3.0 + t);
                    double s3 = cos(4.0 * dist * sin(t * 0.5));

                    double mix = (s1 + s2 + s3) / 3.0;
                    double hue = fmod((ang * 180.0 / 3.14159) * 3 + frame * 2.0, 360.0);
                    double light = 0.5 + 0.5 * mix;

                    THSL hsl;
                    hsl.h = hue;
                    hsl.s = 1.0;
                    hsl.l = light * 0.7;

                    TRGBQUAD color = HSLtoRGB(hsl);
                    pixels[y * sw + x] = color;
                }
            }

            // Visual feedback effect (capture + invert + rotate)
            BitBlt(mem, (int)(sin(t) * 10), (int)(cos(t) * 10), sw, sh, mem, 0, 0, SRCINVERT);
            BitBlt(hdc, 0, 0, sw, sh, mem, 0, 0, NOTSRCCOPY);

            Sleep(1);
            frame++;
        }

        DeleteObject(dib);
        DeleteDC(mem);
        ReleaseDC(0, hdc);
        return 0;
    }
}
namespace gdis3 {
    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;

    typedef struct {
        double h, s, v;
    } THSV;

    TRGBQUAD HSVtoRGB(THSV hsv)
    {
        double hh = fmod(hsv.h, 360.0) / 60.0;
        double c = hsv.v * hsv.s;
        double x = c * (1.0 - fabs(fmod(hh, 2.0) - 1.0));
        double m = hsv.v - c;
        double r, g, b;
        int i = (int)hh;

        if (i == 0) { r = c; g = x; b = 0; }
        else if (i == 1) { r = x; g = c; b = 0; }
        else if (i == 2) { r = 0; g = c; b = x; }
        else if (i == 3) { r = 0; g = x; b = c; }
        else if (i == 4) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        TRGBQUAD q;
        q.r = (BYTE)((r + m) * 255);
        q.g = (BYTE)((g + m) * 255);
        q.b = (BYTE)((b + m) * 255);
        q.a = 0;
        return q;
    }

    THSV RGBtoHSV(TRGBQUAD rgb)
    {
        double r = rgb.r / 255.0, g = rgb.g / 255.0, b = rgb.b / 255.0;
        double max = fmax(fmax(r, g), b);
        double min = fmin(fmin(r, g), b);
        double d = max - min;
        double h = 0;

        if (d == 0) h = 0;
        else if (max == r) h = 60 * fmod(((g - b) / d), 6);
        else if (max == g) h = 60 * (((b - r) / d) + 2);
        else h = 60 * (((r - g) / d) + 4);

        if (h < 0) h += 360;
        double s = (max == 0) ? 0 : d / max;
        double v = max;

        THSV hsv;
        hsv.h = h;
        hsv.s = s;
        hsv.v = v;
        return hsv;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);
        HDC hdc = GetDC(0);
        HDC mem = CreateCompatibleDC(hdc);

        TRGBQUAD* pixels;
        BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), sw, -sh, 1, 32, BI_RGB };
        HBITMAP dib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(mem, dib);

        int frame = 0;

        while (1)
        {
            double t = frame * 0.03;
            double cx = sw / 2.0;
            double cy = sh / 2.0;

            for (int y = 0; y < sh; y++)
            {
                for (int x = 0; x < sw; x++)
                {
                    double nx = (x - cx) / (sh / 2.0);
                    double ny = (y - cy) / (sh / 2.0);

                    double dist = sqrt(nx * nx + ny * ny);
                    double ang = atan2(ny, nx);

                    // Cosine ripples (up) + Tangent waves (down)
                    double ripple = cos(8.0 * dist - t * 2.0) * 0.5 + 0.5;
                    double bubble = fabs(tan(ang + t * 0.7)) * 0.05;

                    double brightness = fmod(ripple + bubble, 1.0);
                    double hue = fmod(ang * 180.0 / 3.14159 + frame * 2.0, 360.0);

                    THSV hsv;
                    hsv.h = hue;
                    hsv.s = 1.0;
                    hsv.v = brightness;

                    TRGBQUAD color = HSVtoRGB(hsv);
                    pixels[y * sw + x] = color;
                }
            }

            // Capture + ripple feedback
            BitBlt(mem, (int)(sin(t * 0.7) * 10), (int)(cos(t * 0.5) * 10), sw, sh, mem, 0, 0, SRCCOPY);
            BitBlt(hdc, 0, 0, sw, sh, mem, 0, 0, SRCCOPY);

            Sleep(1);
            frame++;
        }

        DeleteObject(dib);
        DeleteDC(mem);
        ReleaseDC(0, hdc);
        return 0;
    }
}
namespace gdis4 {
    typedef struct {
        BYTE b, g, r, a;
    } TRGBQUAD;

    typedef struct {
        double h, s, v;
    } THSV;

    TRGBQUAD HSVtoRGB(THSV hsv)
    {
        double hh = fmod(hsv.h, 360.0) / 60.0;
        double c = hsv.v * hsv.s;
        double x = c * (1.0 - fabs(fmod(hh, 2.0) - 1.0));
        double m = hsv.v - c;
        double r = 0, g = 0, b = 0;
        int i = (int)hh;

        if (i == 0) { r = c; g = x; }
        else if (i == 1) { r = x; g = c; }
        else if (i == 2) { g = c; b = x; }
        else if (i == 3) { g = x; b = c; }
        else if (i == 4) { r = x; b = c; }
        else { r = c; b = x; }

        TRGBQUAD q;
        q.r = (BYTE)((r + m) * 255);
        q.g = (BYTE)((g + m) * 255);
        q.b = (BYTE)((b + m) * 255);
        q.a = 0;
        return q;
    }

    THSV RGBtoHSV(TRGBQUAD rgb)
    {
        double r = rgb.r / 255.0, g = rgb.g / 255.0, b = rgb.b / 255.0;
        double max = fmax(fmax(r, g), b);
        double min = fmin(fmin(r, g), b);
        double d = max - min;
        double h = 0;

        if (d == 0) h = 0;
        else if (max == r) h = 60 * fmod(((g - b) / d), 6);
        else if (max == g) h = 60 * (((b - r) / d) + 2);
        else h = 60 * (((r - g) / d) + 4);

        if (h < 0) h += 360;
        double s = (max == 0) ? 0 : d / max;
        double v = max;

        THSV hsv;
        hsv.h = h;
        hsv.s = s;
        hsv.v = v;
        return hsv;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);

        HDC hdc = GetDC(0);
        HDC mem = CreateCompatibleDC(hdc);

        TRGBQUAD* pixels;
        BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), sw, -sh, 1, 32, BI_RGB };
        HBITMAP dib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
        SelectObject(mem, dib);

        int frame = 0;

        while (1)
        {
            double t = frame * 0.03;
            double cx = sw / 2.0;
            double cy = sh / 2.0;

            for (int y = 0; y < sh; y++)
            {
                for (int x = 0; x < sw; x++)
                {
                    double nx = (x - cx) / (sh / 2.0);
                    double ny = (y - cy) / (sh / 2.0);

                    // CosineWaves (up)
                    double waveUp = cos((nx * 5.0 + t) * 3.0) * 0.5 + 0.5;
                    // TangentWaves (down)
                    double waveDown = fabs(tan((ny * 3.0 - t) * 1.2)) * 0.2;

                    double combined = fmod(waveUp + waveDown, 1.0);
                    double hue = fmod((atan2(ny, nx) * 180.0 / 3.14159) + frame * 2.0, 360.0);

                    THSV hsv;
                    hsv.h = hue;
                    hsv.s = 1.0;
                    hsv.v = combined;

                    TRGBQUAD color = HSVtoRGB(hsv);
                    pixels[y * sw + x] = color;
                }
            }

            // Feedback capture with SRCERASE blending
            BitBlt(mem, (int)(sin(t) * 15), (int)(cos(t * 0.5) * 15), sw, sh, mem, 0, 0, SRCERASE);
            BitBlt(hdc, 0, 0, sw, sh, mem, 0, 0, SRCCOPY);

            Sleep(1);
            frame++;
        }

        DeleteObject(dib);
        DeleteDC(mem);
        ReleaseDC(0, hdc);
        return 0;
    }
}
namespace gdis5 {
    typedef struct _TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;

    TRGBQUAD HSVtoRGB(double h, double s, double v) {
        double r = 0, g = 0, b = 0;
        int i = (int)(h * 6);
        double f = h * 6 - i;
        double p = v * (1 - s);
        double q = v * (1 - f * s);
        double t = v * (1 - (1 - f) * s);
        switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
        }
        TRGBQUAD c = { (BYTE)(b * 255), (BYTE)(g * 255), (BYTE)(r * 255), 0 };
        return c;
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
        bmi.bmiHeader.biHeight = -H;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* buf;
        HBITMAP hBmp = CreateDIBSection(hScreen, &bmi, DIB_RGB_COLORS, (void**)&buf, NULL, 0);
        SelectObject(hMem, hBmp);

        double t = 0;
        while (1) {
            t += 0.01;
            for (int y = 0; y < H; y++) {
                double wy = cos((y / 50.0) + t * 1.3);
                for (int x = 0; x < W; x++) {
                    double wx = tan((x / 100.0) - t * 0.7);
                    double hue = fmod((wx + wy) * 0.1 + t * 0.05, 1.0);
                    TRGBQUAD c = HSVtoRGB(hue, 1.0, 1.0);
                    buf[y * W + x] = c;
                }
            }
            BitBlt(hScreen, 0, 0, W, H, hMem, 0, 0, SRCCOPY);
            Sleep(1);
        }

        DeleteObject(hBmp);
        DeleteDC(hMem);
        ReleaseDC(NULL, hScreen);
        return 0;
    }
}
namespace gdis6 {
    typedef struct _TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;

    TRGBQUAD HSVtoRGB(double h, double s, double v) {
        double r = 0, g = 0, b = 0;
        int i = (int)(h * 6);
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
        TRGBQUAD c = { (BYTE)(b * 255), (BYTE)(g * 255), (BYTE)(r * 255), 0 };
        return c;
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

        TRGBQUAD* buf;
        HBITMAP hBmp = CreateDIBSection(hScreen, &bmi, DIB_RGB_COLORS, (void**)&buf, NULL, 0);
        SelectObject(hMem, hBmp);

        double t = 0;
        while (1) {
            t += 0.02; // animation speed

            for (int y = 0; y < H; y++) {
                double wy = cos((y / 40.0) + t * 1.5); // wave up
                double glitchShift = (rand() % 20 - 10); // random horizontal glitch
                for (int x = 0; x < W; x++) {
                    double wx = tan((x / 120.0) - t * 0.7); // wave down
                    double hue = fmod((wx + wy + glitchShift * 0.01 + t * 0.05), 1.0);
                    TRGBQUAD c = HSVtoRGB(hue, 1.0, 1.0);
                    buf[y * W + x] = c;
                }
            }

            BitBlt(hScreen, 0, 0, W, H, hMem, 0, 0, SRCCOPY);
            Sleep(1);
        }

        DeleteObject(hBmp);
        DeleteDC(hMem);
        ReleaseDC(NULL, hScreen);
        return 0;
    }
}
