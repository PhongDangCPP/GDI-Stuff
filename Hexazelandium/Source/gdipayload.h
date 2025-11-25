#pragma once
#include "resource.h"

VOID WINAPI sfx1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8100 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(
            (150 ^ (t & t + t / 25) - t * (t + t >> 16) & 67)
            );

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8100 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(
            (t * (t / 50 ^ t / 90) + t >> 4)
            );

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 6000, 6000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[6000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(
            (t * 37 | t + (t ^ t >> 11) - t * ((t % 24 ? t >> 2 : t >> 6) & t >> 11) ^ t << 1 & (t & 598 ? t * t >> 4 : t * t >> 1))
            );

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    DWORD c = 0;
    for (DWORD t = 0; t < sizeof(buffer); t++) {
        double wave = Math::SineWave(1, c * 0.0015, (c & 32767), wfx.nSamplesPerSec * 4);
        buffer[t] = (int(wave * 15) & 65535);
        c++;
    }
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    DWORD dx = 0, c = 0;
    for (DWORD t = 0; t < sizeof(buffer); t++) {
        if (c == 100000) {
            c = 0;
        }
        DWORD offset = c + 20000;
        double freq = (10 * sizeof(buffer)) / offset;
        buffer[t] = Math::SineWave(freq, 2, offset, 4000);
        buffer[t] *= 2;
        c++;
    }
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(TriangleWave(t, t * 5, t * 2500) + SquareWave(t * t >> 6, t & 5, t * 200000000));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx7() {
    SeedXorshift32(__rdtsc());
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(TriangleWave(t, t + 99, t * 575) - SquareWave(t, t + t, t * 404000));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx8() {
    SeedXorshift32(__rdtsc());
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(SquareWave(t, t, t * 16000000) - TriangleWave(t, t + 7, t * xorshift32() % 444));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(TriangleWave(t + 550, t, t * 150) + t + 30 * 50);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t) {
        DWORD freq = Math::SineWave(10, t % 900, 1, 1100);
        buffer[t] = t ^ (freq % 4);
    }
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32100, 32100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32100 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(
            (2 * sin(t + (t + t * t >> 6) - t * (t >> 9 & (t % 16 ? t >> t : t * 6) & t >> 9)))
            );

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    DWORD c = 0;
    for (DWORD t = 0; t < sizeof(buffer); t++) {
        double wave = Math::SineWave(1, c * 0.0025, (c & 32768), wfx.nSamplesPerSec * 2);
        buffer[t] = (int(wave * 15) & 65535);
        c++;
    }
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
// simple C‑string arrays (NO std::string)
const char* g_title = "Hexazelandium.exe";
const char* g_text = "Made By PhongDangCPP/P. ĐĂNG & N17Pro3426";

const char* g_fonts[] = {
    "Arial",
    "Consolas",
    "Courier New",
    "Impact",
    "Times New Roman",
    "Comic Sans MS",
    "Verdana"
};
const int g_fontCount = sizeof(g_fonts) / sizeof(g_fonts[0]);

HFONT MakeRandomFont(int size) {
    int idx = rand() % g_fontCount;

    return CreateFontA(
        size, 0, 0, 0,
        FW_BOLD,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        g_fonts[idx]
    );
}

DWORD WINAPI tetz(LPVOID lpParam)
{
    srand((unsigned)time(0));

    // fullscreen drawing DC (safe, legal)
    HDC hdc = GetDC(NULL);
    if (!hdc) return 0;

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    while (1) {
        int x = rand() % sw;
        int y = rand() % sh;

        int size = 60 + (rand() % 220);   // giant font sizes

        HFONT font = MakeRandomFont(size);
        HFONT old = (HFONT)SelectObject(hdc, font);

        COLORREF c = RGB(rand() % 256, rand() % 256, rand() % 256);
        SetTextColor(hdc, c);

        // transparent background — no black/white box
        SetBkMode(hdc, TRANSPARENT);

        // giant title
        TextOutA(hdc, x, y, g_title, lstrlenA(g_title));

        // second text
        TextOutA(hdc, x + 50, y + 80, g_text, lstrlenA(g_text));

        SelectObject(hdc, old);
        DeleteObject(font);

        Sleep(1);
    }

    ReleaseDC(NULL, hdc);
    return 0;

}
DWORD WINAPI tetz2(LPVOID lpParam)
{
    srand((unsigned int)time(NULL));

    HDC hdc = GetDC(NULL); // Get the screen DC
    if (!hdc) return 1;

    // Get screen dimensions
    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);

    // Text info
    const char* text = "Hexazelandium.exe";

    // Initial position
    int x = rand() % screenX;
    int y = rand() % screenY;
    int dx = 5 + rand() % 10;
    int dy = 5 + rand() % 10;

    while (1)
    {
        // Random text color
        COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);

        // Random font size
        int fontSize = 48 + rand() % 72; // 48-120
        HFONT hFont = CreateFontA(
            fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY, VARIABLE_PITCH, "Arial");

        HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
        SetTextColor(hdc, color);
        SetBkMode(hdc, TRANSPARENT);

        // Draw text
        TextOutA(hdc, x, y, text, (int)strlen(text));

        // Move text
        x += dx;
        y += dy;

        // Bounce off screen edges
        SIZE sz;
        GetTextExtentPoint32A(hdc, text, (int)strlen(text), &sz);
        if (x < 0 || x + sz.cx > screenX) dx = -dx;
        if (y < 0 || y + sz.cy > screenY) dy = -dy;

        // Delete font to avoid GDI leak
        SelectObject(hdc, oldFont);
        DeleteObject(hFont);

        Sleep(50); // control speed

        // Optional: clear previous text by drawing over it in background color
        // But you said no black/white background, so we can just leave trails
    }

    ReleaseDC(NULL, hdc);
    return 0;
}
DWORD WINAPI abll(LPVOID lpParam) {
    POINT cursor;
    int signX = 1;
    int signY = 1;
    int signX1 = 1;
    int signY1 = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    while (1) {
        HDC hdc = GetDC(HWND_DESKTOP);
        int icon_x = GetSystemMetrics(SM_CXICON);
        int icon_y = GetSystemMetrics(SM_CYICON);
        GetCursorPos(&cursor);
        //int X = cursor.x + rand() % 3 - 1;
        //int Y = cursor.y + rand() % 3 - 1;
        //int top_x = 0 + cursor.x;
        //int top_y = 0 + cursor.y;
        //int size = rand() % 900 +100;
        //int bottom_x = 300 + cursor.x;
        //int bottom_y = 300 + cursor.y;
        x += incrementor * signX;
        y += incrementor * signY;
        SetCursorPos(x, y);
        DrawIconEx(hdc, cursor.x - icon_x, cursor.y - icon_y, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2)), 3 * GetSystemMetrics(SM_CXICON), 3 * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        //DrawIcon(hdc, cursor.x - icon_x, cursor.y - icon_y, LoadCursor(0, IDC_ARROW));
        if (y >= GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }

        if (x >= GetSystemMetrics(SM_CXSCREEN))
        {
            signX = -1;
        }

        if (y == 0)
        {
            signY = 1;
        }

        if (x == 0)
        {
            signX = 1;
        }
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return(1);
}

namespace gdi1 {
    struct HSL {
        float h; // Hue [0,1]
        float s; // Saturation [0,1]
        float l; // Lightness [0,1]
    };

    namespace Colors {

        HSL rgb2hsl(RGBQUAD rgb) {
            HSL hsl;
            float r = rgb.rgbRed / 255.f;
            float g = rgb.rgbGreen / 255.f;
            float b = rgb.rgbBlue / 255.f;

            float minVal = min(min(r, g), b);
            float maxVal = max(max(r, g), b);
            float delta = maxVal - minVal;

            hsl.l = (maxVal + minVal) / 2.f;
            hsl.s = 0.f;
            hsl.h = 0.f;

            if (delta != 0.f) {
                hsl.s = (hsl.l < 0.5f) ? (delta / (maxVal + minVal)) : (delta / (2.f - maxVal - minVal));

                if (r == maxVal) hsl.h = (g - b) / delta;
                else if (g == maxVal) hsl.h = 2.f + (b - r) / delta;
                else hsl.h = 4.f + (r - g) / delta;

                hsl.h /= 6.f;
                if (hsl.h < 0.f) hsl.h += 1.f;
            }

            return hsl;
        }

        RGBQUAD hsl2rgb(HSL hsl) {
            float r, g, b;
            if (hsl.s == 0.f) {
                r = g = b = hsl.l; // gray
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

                float q = (hsl.l < 0.5f) ? (hsl.l * (1.f + hsl.s)) : (hsl.l + hsl.s - hsl.l * hsl.s);
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
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(NULL);
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pixels = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcMem, hBitmap);

        RGBQUAD* buffer = (RGBQUAD*)pixels;
        int i = 0;

        while (true) {
            // Capture screen fast
            BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

            // Apply HSL shader
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int idx = y * width + x;
                    HSL hsl = Colors::rgb2hsl(buffer[idx]);
                    hsl.h = fmodf(hsl.h + (i * 0.002f) + (y / (float)height * 0.1f), 1.f);
                    buffer[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

            i++;
            Sleep(1); // prevent 100% CPU
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        DeleteObject(hBitmap);

        return 0;
    }
}
namespace gdi2 {
#include <omp.h>   // OpenMP for parallel loops
#include <thread>  // optional for Sleep

    // Modern struct instead of union
    typedef struct _TRGBQUAD {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE reserved;
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        const int w = GetSystemMetrics(SM_CXSCREEN);
        const int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down DIB

        PTRGBQUAD pixels = nullptr;
        HBITMAP hbm = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)&pixels, nullptr, 0);
        SelectObject(hdcMem, hbm);

        int frame = 0;
        for (;;) {
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            // OpenMP parallel loop for fast pixel processing
#pragma omp parallel for
            for (int i = 0; i < w * h; i++) {
                int x = i % w;
                int y = i / w;

                // Hypnotic XOR red rotate effect
                pixels[i].r += (pixels[i].r + pixels[i].g + pixels[i].b / 2);
                pixels[i].g += (pixels[i].g + pixels[i].b / 2); // optional fade effect
                pixels[i].b += (pixels[i].b / 2); // optional fade effect
                pixels[i].b ^= x ^ y; // optional fade effect
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

            frame++;
            Sleep(1); // minimal delay to control CPU usage
        }

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdi3 {

    // Define RGB quad structure
    typedef struct _TRGBQUAD {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE reserved; // unused
    } TRGBQUAD, * PTRGBQUAD;

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        // Screen dimensions
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        // Create memory DC compatible with screen
        HDC hdcScreen = GetDC(nullptr);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        // Setup DIB section
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        TRGBQUAD* pixels = nullptr;
        HBITMAP hbm = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&pixels, nullptr, 0);
        SelectObject(hdcMem, hbm);

        for (;;) {
            HDC hdcScreenLoop = GetDC(nullptr);

            // Copy screen into memory DC
            BitBlt(hdcMem, 0, 0, w, h, hdcScreenLoop, 0, 0, SRCCOPY);

            // Parallel "shader" using OpenMP
#pragma omp parallel for
            for (int i = 0; i < w * h; i++) {
                int x = i % w;
                int y = i / w;

                pixels[i].r = (BYTE)((pixels[i].r + (x ^ y)) % 64);
                pixels[i].g = (BYTE)((pixels[i].g + (x ^ y)) % 32);
                pixels[i].b = (BYTE)((pixels[i].b + (x ^ y)) % 16);
            }

            // Draw memory DC back to screen
            BitBlt(hdcScreenLoop, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

            // Release screen DC
            ReleaseDC(nullptr, hdcScreenLoop);

            // Small delay
            Sleep(1);
        }

        // Cleanup (never reached in infinite loop)
        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(nullptr, hdcScreen);

        return 0;
    }
}
namespace gdi4 {
    typedef struct {
        float h;
        float s;
        float l;
    } HSL;

    HSL rgb2hsl(RGBQUAD rgb) {
        HSL hsl;
        float r = rgb.rgbRed / 255.f;
        float g = rgb.rgbGreen / 255.f;
        float b = rgb.rgbBlue / 255.f;

        float rgbMin = min(r, min(g, b));
        float rgbMax = max(r, max(g, b));
        float delta = rgbMax - rgbMin;

        hsl.l = (rgbMax + rgbMin) / 2.f;
        if (delta == 0.f) {
            hsl.h = 0.f;
            hsl.s = 0.f;
            return hsl;
        }

        hsl.s = (hsl.l < 0.5f) ? delta / (rgbMax + rgbMin) : delta / (2.f - rgbMax - rgbMin);

        if (r == rgbMax) hsl.h = (g - b) / delta;
        else if (g == rgbMax) hsl.h = 2.f + (b - r) / delta;
        else hsl.h = 4.f + (r - g) / delta;

        hsl.h /= 6.f;
        if (hsl.h < 0.f) hsl.h += 1.f;

        return hsl;
    }

    RGBQUAD hsl2rgb(HSL hsl) {
        float r, g, b;
        float h = hsl.h, s = hsl.s, l = hsl.l;

        if (s == 0.f) {
            r = g = b = l;
        }
        else {
            auto hue2rgb = [](float p, float q, float t) {
                if (t < 0.f) t += 1.f;
                if (t > 1.f) t -= 1.f;
                if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
                if (t < 1.f / 2.f) return q;
                if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
                return p;
                };

            float q = (l < 0.5f) ? l * (1.f + s) : l + s - l * s;
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
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = nullptr;
        HBITMAP hbm = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        SelectObject(hdcMem, hbm);

        int t = 0;
        while (true) {
            // Capture screen
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    int idx = y * w + x;
                    RGBQUAD col = pixels[idx];

                    HSL hsl = rgb2hsl(col);

                    // XOR Fractal Rotate Eye Shader
                    float fx = (float)(x ^ y) + t;
                    hsl.h = fmod(fx / 500.f + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    pixels[idx] = hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            t++;
        }

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 0;
    }
}
namespace gdi5 {

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

            hsl.l = (max + min) * 0.5f;

            if (delta == 0.f)
            {
                hsl.h = 0.f;
                hsl.s = 0.f;
            }
            else
            {
                hsl.s = (hsl.l < 0.5f) ? (delta / (max + min)) : (delta / (2.f - max - min));
                if (r == max) hsl.h = (g - b) / delta + (g < b ? 6.f : 0.f);
                else if (g == max) hsl.h = (b - r) / delta + 2.f;
                else hsl.h = (r - g) / delta + 4.f;
                hsl.h /= 6.f;
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
                auto hue2rgb = [](float p, float q, float t) -> float {
                    if (t < 0.f) t += 1.f;
                    if (t > 1.f) t -= 1.f;
                    if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
                    if (t < 1.f / 2.f) return q;
                    if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
                    return p;
                    };

                float q = (hsl.l < 0.5f) ? hsl.l * (1.f + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
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
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
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

        int t = 0;

        while (1)
        {
            // Capture current screen into memory DC
            BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int index = y * w + x;
                    RGBQUAD rgbOrig = pixels[index];

                    HSL hsl = Colors::rgb2hsl(rgbOrig);

                    // Old-school plasma effect
                    float plasma = (sinf((x + t) * 0.03f) + cosf((y + t) * 0.03f)) * 0.5f + 0.5f;
                    hsl.h = fmodf(plasma + y / (float)h * 0.1f, 1.f);

                    pixels[index] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            t++;
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);
        DeleteObject(bmp);
        return 0;
    }
}
namespace gdi6 {
    typedef struct {
        FLOAT h;
        FLOAT s;
        FLOAT l;
    } HSL;

    namespace Colors {

        HSL rgb2hsl(RGBQUAD rgb) {
            HSL hsl{};
            FLOAT r = rgb.rgbRed / 255.f;
            FLOAT g = rgb.rgbGreen / 255.f;
            FLOAT b = rgb.rgbBlue / 255.f;

            FLOAT mn = min(r, min(g, b));
            FLOAT mx = max(r, max(g, b));
            FLOAT d = mx - mn;

            hsl.l = (mx + mn) * 0.5f;

            if (d == 0.f) {
                hsl.h = hsl.s = 0.f;
                return hsl;
            }

            hsl.s = (hsl.l < .5f) ? d / (mx + mn) : d / (2.f - mx - mn);

            FLOAT dr = (((mx - r) / 6.f) + d / 2.f) / d;
            FLOAT dg = (((mx - g) / 6.f) + d / 2.f) / d;
            FLOAT db = (((mx - b) / 6.f) + d / 2.f) / d;

            if (mx == r)      hsl.h = db - dg;
            else if (mx == g) hsl.h = (1.f / 3.f) + dr - db;
            else              hsl.h = (2.f / 3.f) + dg - dr;

            if (hsl.h < 0.f) hsl.h += 1.f;
            if (hsl.h > 1.f) hsl.h -= 1.f;

            return hsl;
        }

        RGBQUAD hsl2rgb(HSL hsl) {
            RGBQUAD rgb{};
            if (hsl.s == 0.f) {
                BYTE v = (BYTE)(hsl.l * 255.f);
                rgb.rgbRed = rgb.rgbGreen = rgb.rgbBlue = v;
                return rgb;
            }

            FLOAT v = (hsl.l <= .5f) ?
                (hsl.l * (1.f + hsl.s)) :
                (hsl.l + hsl.s - hsl.l * hsl.s);

            FLOAT m = 2.f * hsl.l - v;
            FLOAT h6 = hsl.h * 6.f;
            INT   s = (INT)h6;
            FLOAT f = h6 - s;

            FLOAT p = v * (1.f - hsl.s);
            FLOAT q = v * (1.f - hsl.s * f);
            FLOAT t = v * (1.f - hsl.s * (1.f - f));

            FLOAT r, g, b;

            switch (s % 6) {
            case 0: r = v; g = t; b = m; break;
            case 1: r = q; g = v; b = m; break;
            case 2: r = m; g = v; b = t; break;
            case 3: r = m; g = q; b = v; break;
            case 4: r = t; g = m; b = v; break;
            default:r = v; g = m; b = q; break;
            }

            rgb.rgbRed = (BYTE)(r * 255.f);
            rgb.rgbGreen = (BYTE)(g * 255.f);
            rgb.rgbBlue = (BYTE)(b * 255.f);
            return rgb;
        }
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdc = GetDC(NULL);
        HDC mem = CreateCompatibleDC(hdc);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(bmi);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = h;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* px = NULL;

        HBITMAP bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&px, NULL, 0);
        SelectObject(mem, bmp);

        INT t = 0;

        while (1) {

            BitBlt(mem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {

                    int idx = y * w + x;
                    RGBQUAD c = px[idx];

                    HSL hsl = Colors::rgb2hsl(c);

                    // ====== YOUR REQUESTED HUE/SAT FORCING ======
                    hsl.h = fmod((239 + t) / 500.f + hsl.h + 0.02f, 1.f);
                    hsl.s = 1.f;
                    hsl.h = .5f;
                    // ============================================

                    // ===== Mandelbrot Rotating Eye =====
                    float zx = (x - w / 2.f) / (w / 4.f);
                    float zy = (y - h / 2.f) / (h / 4.f);

                    float ang = t * 0.01f;
                    float rx = zx * cos(ang) - zy * sin(ang);
                    float ry = zx * sin(ang) + zy * cos(ang);

                    float cx = sin(t * 0.01f) * 0.3f;
                    float cy = cos(t * 0.01f) * 0.3f;

                    int iter = 0, maxIter = 50;
                    float a = rx, b = ry;

                    while (a * a + b * b <= 4.f && iter < maxIter) {
                        float na = a * a - b * b + cx;
                        b = 2.f * a * b + cy;
                        a = na;
                        iter++;
                    }

                    if (iter < maxIter)
                        hsl.l = (float)iter / maxIter;
                    // ============================================

                    px[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdc, 0, 0, w, h, mem, 0, 0, SRCCOPY);

            t++;
        }

        return 0;
    }
}
namespace gdi7 {
    typedef struct { float h, s, l; } HSL;

    // ---------- HSL <-> RGB ----------
    inline HSL rgb2hsl(const RGBQUAD& c)
    {
        float r = c.rgbRed / 255.f;
        float g = c.rgbGreen / 255.f;
        float b = c.rgbBlue / 255.f;

        float mn = min(r, min(g, b));
        float mx = max(r, max(g, b));
        float d = mx - mn;

        HSL o{};
        o.l = (mx + mn) * 0.5f;

        if (d == 0.f) { o.h = 0.f; o.s = 0.f; return o; }

        o.s = (o.l < 0.5f)
            ? d / (mx + mn)
            : d / (2.f - mx - mn);

        float dr = (((mx - r) / 6.f) + (d / 2.f)) / d;
        float dg = (((mx - g) / 6.f) + (d / 2.f)) / d;
        float db = (((mx - b) / 6.f) + (d / 2.f)) / d;

        if (r == mx)      o.h = db - dg;
        else if (g == mx) o.h = (1.f / 3.f) + dr - db;
        else              o.h = (2.f / 3.f) + dg - dr;

        if (o.h < 0.f) o.h += 1.f;
        if (o.h > 1.f) o.h -= 1.f;

        return o;
    }

    inline RGBQUAD hsl2rgb(const HSL& o)
    {
        RGBQUAD out{};
        float h = o.h * 6.f;
        int   s = (int)h;
        float f = h - s;

        float v = (o.l <= 0.5f) ? (o.l * (1.f + o.s)) : (o.l + o.s - o.l * o.s);

        if (v == 0.f)
            return out;

        float m = o.l + o.l - v;
        float sv = (v - m) / v;
        float vsf = v * sv * f;

        float mid1 = m + vsf;
        float mid2 = v - vsf;

        float r, g, b;
        switch (s)
        {
        default:
        case 0: r = v;    g = mid1; b = m;    break;
        case 1: r = mid2; g = v;    b = m;    break;
        case 2: r = m;    g = v;    b = mid1; break;
        case 3: r = m;    g = mid2; b = v;    break;
        case 4: r = mid1; g = m;    b = v;    break;
        case 5: r = v;    g = m;    b = mid2; break;
        }

        out.rgbRed = (BYTE)(r * 255.f);
        out.rgbGreen = (BYTE)(g * 255.f);
        out.rgbBlue = (BYTE)(b * 255.f);
        return out;
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC scr = GetDC(0);
        int W = GetSystemMetrics(SM_CXSCREEN);
        int H = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bi{};
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = W;
        bi.bmiHeader.biHeight = -H;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* px = 0;
        HBITMAP bmp = CreateDIBSection(scr, &bi, DIB_RGB_COLORS, (void**)&px, 0, 0);

        HDC mem = CreateCompatibleDC(scr);
        SelectObject(mem, bmp);

        int t = 0;

        while (1)
        {
            BitBlt(mem, 0, 0, W, H, scr, 0, 0, SRCCOPY);

            float it = (float)t * 0.01f;

            for (int y = 0; y < H; ++y)
            {
                float ny = (float)y / H;

                for (int x = 0; x < W; ++x)
                {
                    int idx = y * W + x;
                    RGBQUAD c = px[idx];

                    HSL hsl = rgb2hsl(c);

                    // Ripple fractal shader
                    float dx = (float)x - (W * 0.5f);
                    float dy = (float)y - (H * 0.5f);
                    float d = sqrtf(dx * dx + dy * dy);

                    float ripple = sinf(d * 0.02f - it * 4.f) * 0.02f;

                    // final HSL
                    hsl.h = fmodf((float)x / 500.f + hsl.h + 0.02f + ripple, 1.f);
                    if (hsl.h < 0.f) hsl.h += 1.f;

                    hsl.s = 1.f;
                    hsl.l = 0.5f;

                    px[idx] = hsl2rgb(hsl);
                }
            }

            BitBlt(scr, 0, 0, W, H, mem, 0, 0, SRCCOPY);

            t++;
            Sleep(1);
        }

        return 0;
    }
}

namespace gdi8
{
    // ==========================================================
    //  Pixel structure (TRGBQUAD)
    // ==========================================================
    typedef struct _TRGBQUAD {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        INT w = GetSystemMetrics(SM_CXSCREEN);
        INT h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;     // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        PTRGBQUAD rgb = nullptr;

        HBITMAP hbm = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS,
            (void**)&rgb, NULL, 0);

        SelectObject(hdcMem, hbm);

        double t = 0.0;

        for (;;)
        {
            HDC hdc = GetDC(0);
            BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
            ReleaseDC(0, hdc);

            // ==========================================================
            //   Water swirl + sine perturbation shader
            //   OMP-parallelized for speed
            // ==========================================================
#pragma omp parallel for schedule(static)
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int i = y * w + x;

                    // Current pixel
                    TRGBQUAD px = rgb[i];

                    // ------------------------------------------------------
                    //   Water ripple / swirl displacement (simple but fast)
                    // ------------------------------------------------------
                    float fx = (float)x - w * 0.5f;
                    float fy = (float)y - h * 0.5f;

                    float dist = sqrtf(fx * fx + fy * fy);
                    float angle = dist * 0.005f + (float)t * 0.03f;

                    fx = x + sinf(angle) * 3.0f;
                    fy = y + cosf(angle) * 3.0f;

                    // Clamp to screen
                    int sx = (int)(fx < 0 ? 0 : (fx >= w ? w - 1 : fx));
                    int sy = (int)(fy < 0 ? 0 : (fy >= h ? h - 1 : fy));

                    TRGBQUAD samp = rgb[sy * w + sx];

                    // Fade original slightly (feedback blur)
                    px.r = (px.r + samp.r) >> 1;
                    px.g = (px.g + samp.g) >> 1;
                    px.b = (px.b + samp.b) >> 1;

                    rgb[i] = px;
                }
            }

            // Output shader result
            hdc = GetDC(0);
            BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            ReleaseDC(0, hdc);

            Sleep(1);
            t += 0.1;
        }

        return 0;
    }
}
namespace gdi9
{
    typedef struct {
        float h;
        float s;
        float l;
    } HSL;

    namespace Colors {

        HSL rgb2hsl(RGBQUAD c)
        {
            float r = c.rgbRed / 255.f;
            float g = c.rgbGreen / 255.f;
            float b = c.rgbBlue / 255.f;

            float mn = min(r, min(g, b));
            float mx = max(r, max(g, b));
            float d = mx - mn;

            HSL hsl;
            hsl.l = (mx + mn) * 0.5f;
            hsl.s = 0.f;
            hsl.h = 0.f;

            if (d != 0.f)
            {
                hsl.s = (hsl.l < 0.5f) ? (d / (mx + mn)) : (d / (2.f - mx - mn));
                float dr = (((mx - r) / 6.f) + d * .5f) / d;
                float dg = (((mx - g) / 6.f) + d * .5f) / d;
                float db = (((mx - b) / 6.f) + d * .5f) / d;

                if (mx == r)      hsl.h = db - dg;
                else if (mx == g) hsl.h = (1.f / 3.f) + dr - db;
                else              hsl.h = (2.f / 3.f) + dg - dr;

                if (hsl.h < 0.f) hsl.h += 1.f;
                if (hsl.h > 1.f) hsl.h -= 1.f;
            }

            return hsl;
        }

        RGBQUAD hsl2rgb(HSL hsl)
        {
            float h = hsl.h * 6.f;
            float s = hsl.s;
            float l = hsl.l;

            float v = (l <= .5f) ? (l * (1.f + s)) : (l + s - l * s);

            float r = l, g = l, b = l;

            if (v > 0.f)
            {
                float m = l + l - v;
                float sv = (v - m) / v;

                int sext = (int)h;
                float f = h - sext;
                float vsf = v * sv * f;
                float mid1 = m + vsf;
                float mid2 = v - vsf;

                switch (sext)
                {
                case 0: r = v;   g = mid1; b = m;    break;
                case 1: r = mid2; g = v;   b = m;    break;
                case 2: r = m;   g = v;   b = mid1; break;
                case 3: r = m;   g = mid2; b = v;    break;
                case 4: r = mid1; g = m;   b = v;    break;
                case 5: r = v;   g = m;   b = mid2; break;
                }
            }

            RGBQUAD out;
            out.rgbRed = (BYTE)(r * 255.f);
            out.rgbGreen = (BYTE)(g * 255.f);
            out.rgbBlue = (BYTE)(b * 255.f);
            out.rgbReserved = 0;
            return out;
        }
    }
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdc = GetDC(NULL);
        HDC mem = CreateCompatibleDC(hdc);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bi = { 0 };
        bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
        bi.bmiHeader.biWidth = w;
        bi.bmiHeader.biHeight = -h;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbq = nullptr;
        HBITMAP bmp = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, (void**)&rgbq, NULL, 0);
        SelectObject(mem, bmp);

        int i = 0;

        while (1)
        {
            BitBlt(mem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

            for (int y = 0; y < h; y++)
            {
                float wave = (sinf((y + i * 2) * 0.01f) * 0.05f);

                for (int x = 0; x < w; x++)
                {
                    int idx = y * w + x;

                    HSL hsl = Colors::rgb2hsl(rgbq[idx]);

                    hsl.h = fmodf((i / 500.f) + hsl.h + 0.02f + wave, 1.f);
                    hsl.s = 1.f;
                    hsl.h = fmodf(hsl.h + .5f, 1.f);

                    rgbq[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdc, 0, 0, w, h, mem, 0, 0, SRCCOPY);
            i++;
        }

        return 0;
    }
}
namespace gdi10
{
    typedef struct _TRGBQUAD {
        union {
            COLORREF rgb;
            struct {
                BYTE r;
                BYTE g;
                BYTE b;
                BYTE a;   // reserved
            };
        };
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {
        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;     // top-down DIB for normal orientation
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        PTRGBQUAD pixels = nullptr;

        HBITMAP hbm = CreateDIBSection(
            hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0
        );

        SelectObject(hdcMem, hbm);

        double t = 0;

        for (;;)
        {
            // capture screen → hdcMem
            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            // shader pass
            double freq = 0.015;
            double amp = 120.0;

            for (int y = 0; y < h; ++y)
            {
                double wave = sin((y * freq) + t) + tan((y * freq * 0.3) + t * 0.5);

                int shift = int(wave * amp);

                int line = y * w;

                for (int x = 0; x < w; ++x)
                {
                    int i = line + x;
                    int s = line + ((x + shift + w) % w);

                    // crushed tangent blending
                    BYTE r = pixels[s].r;
                    BYTE g = pixels[s].g;
                    BYTE b = pixels[s].b;

                    r = BYTE((r + abs(shift)) % 256);
                    g = BYTE((g + (shift >> 1)) % 256);
                    b = BYTE((b ^ (shift * 7)) & 255);

                    pixels[i].r = r;
                    pixels[i].g = g;
                    pixels[i].b = b;
                }
            }

            // write back to screen
            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

            // animated time
            t += 0.12;

            Sleep(1);
        }

        // never reached
        return 0;
    }
}
namespace gdi11
{
    typedef struct _TRGBQUAD {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } TRGBQUAD, * PTRGBQUAD;
    DWORD WINAPI RGDI(LPVOID lpParam)
    {

        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        int total = w * h;

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;               // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        PTRGBQUAD pixels = nullptr;

        HBITMAP hbm = CreateDIBSection(
            hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0
        );

        SelectObject(hdcMem, hbm);

        // Main shader loop
        for (;;) {

            BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

            PTRGBQUAD px = pixels;

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++, px++) {

                    uint8_t n = (uint8_t)((x ^ y) & 0xFF);

                    // Loud, crushed RGB XOR effect
                    px->r += n;
                    px->g += (n * 3);
                    px->b += (n * 7);
                }
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

            Sleep(1);  // ~1ms delay
        }

        // unreachable, but correct cleanup:
        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);

        return 0;
    }
}
namespace gdi12
{
    typedef struct {
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

            float rgbMin = min(min(r, g), b);
            float rgbMax = max(max(r, g), b);
            float delta = rgbMax - rgbMin;

            hsl.l = (rgbMax + rgbMin) * 0.5f;

            if (delta == 0.f)
            {
                hsl.h = 0.f;
                hsl.s = 0.f;
            }
            else
            {
                hsl.s = (hsl.l < 0.5f) ? (delta / (rgbMax + rgbMin)) : (delta / (2.f - rgbMax - rgbMin));

                if (r == rgbMax) hsl.h = (g - b) / delta;
                else if (g == rgbMax) hsl.h = 2.f + (b - r) / delta;
                else hsl.h = 4.f + (r - g) / delta;

                hsl.h /= 6.f;
                if (hsl.h < 0.f) hsl.h += 1.f;
            }

            return hsl;
        }

        RGBQUAD hsl2rgb(HSL hsl)
        {
            float r, g, b;
            float h = hsl.h;
            float s = hsl.s;
            float l = hsl.l;

            if (s == 0.f)
            {
                r = g = b = l;
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
    }

    DWORD WINAPI RGDI(LPVOID lpParam)
    {
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
        HBITMAP bmp = CreateDIBSection(hdcScreen, &bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcMem, bmp);

        int i = 0;
        while (true)
        {
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int idx = y * w + x;
                    HSL hsl;
                    hsl.h = 0.5f;
                    hsl.s = 1.f;

                    float fx = 239 + i;
                    hsl.l = 0.5f + 0.5f * sinf(x * 0.02f + i * 0.04f) * cosf(y * 0.02f + i * 0.03f); // Plasma ripple
                    hsl.h = fmodf(fx / 500.f + hsl.h + 0.02f, 1.f);

                    pixels[idx] = Colors::hsl2rgb(hsl);
                }
            }

            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            i++;
            Sleep(10); // ~100 FPS throttle
        }

        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 0;
    }
}