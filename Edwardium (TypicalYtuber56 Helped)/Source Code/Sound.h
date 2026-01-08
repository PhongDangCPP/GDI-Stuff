#pragma once
namespace bytebeat {
    VOID WINAPI sfx1() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11100, 11100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                ((t * t) ^ (t & 33 | t >> 8) ^ (t & 14 | t >> 9 | t & 76) ^ (t | 187) ^ t * (t >> 8 & 838 + t >> 13) & 644)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx2() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11100, 11100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t * sin(((t >> 7 | t | t >> (t >> 15)) + (t >> 8 & 5))) * 19)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx3() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22100, 22100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[22100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t * ((t >> (t & 8192 ? 3 : 2) >> 2 | t + t) & t >> 10 & t + t >> 7))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx4() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22100, 22100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[22100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t >> 4 | t * (t >> 5 & t >> 7) | t * (t >> 4 & t >> 5 | t >> 9 & t >> 1) * t * (t + t >> t & t >> 8))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx5() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22100, 22100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[22100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (2 * (t + t >> 5 + t) - (t ^ t >> 5) + t * (t >> 14 & 14))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx6() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11100, 11100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (5 * (t + (t ^ t >> 7) ^ 5) | 19 * (t * t >> 43 | 15 * t >> 4) & (t * t >> 172) / 2 * t * ((t >> 9 | t >> 13) & 15) & 129 | t * ((t >> 9 | 3 < t) & 25 & t >> 10))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx7() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11100, 11100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (9 * (t + (t ^ t >> 6) ^ 5) | 19 * (t * t >> 43 | 15 * t >> 4) & (t * t >> 172) / 2 * t * ((t >> 9 | t >> 11) & 15) & 129 | t * ((t >> 9 | 3 < t) & 25 & t >> 10))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx8() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11100, 11100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (10 * (t & 9 * t | t >> 5 | (t & 32768 ? -6 * t / 7 : (t & 65536 ? -9 * t & 666 : -9 * (t & 55)) / 11)))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx9() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22100, 22100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[22100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (66 * (t & 7 * t | t >> 6 | (t & 32768 ? -6 * t / 7 : (t & 65536 ? -9 * t & 100 : -9 * (t & 150)) / 11)))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx10() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11100, 11100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                ((t * "57"[(t >> 14) % 2] >> 2 & 148) + (t * "1234"[(t >> 10) % 4] * "57"[(t >> 10) % 2] & 95) + 96 * (1 + (1 & t >> "999"[(t >> 12) % 2])) + 70 * sin(4E3 / (t & 4095)))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx11() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11100, 11100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (128 * sin((t * (t >> 5 | t >> 8)) / 40.75) + 128)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }

}