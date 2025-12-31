#pragma once
namespace bytebeat {
    VOID WINAPI sfx1() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16100, 16100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[16100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (2 * (t % 128 | t % 130))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx2() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[8100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t << 2 * t >> 4 * t << 4 + t >> 8 | t << 1 & -t >> 4)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx3() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[8100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t * (t >> 5 & t) + (t >> 5) + t * (t >> 14 & 14))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx4() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[32000 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t - (t >> 8 | t >> 9) & t >> 6 & t * (t & t >> 13 | t + t >> 6))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx5() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[24000 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                ((t >> 1) * (15 & 0x234568a0 >> (t >> 6 & 28)) | t + t >> 1 >> (t >> 11) ^ t >> 12) + (t + t >> 4 & t >> t & 24)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx6() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[24000 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                ((((t >> 10 & 44) % 32 >> 1) + ((t >> 9 & 44) % 6 >> 1)) * (32768 > t % 65536 ? 1 : 4 / 5) * t | t >> 3) * (t | t >> 8 | t >> 6)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx7() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11045, 11045, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11045 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t * (t & 16384 ? 7 : 5) * (6 - (5 & t >> 10) + (3 & t >> 11)) >> (3 & +t >> (t & 4096 ? 2 : 11)))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx8() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11045, 11045, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11045 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t * (t >> 9 | t | t >> 9 | t >> 13) + t * t)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx9() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11045, 11045, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[11045 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                ((t & 5 * t >> 1 | t >> 4) + (t >> 8 + t | t >> 4 * t << 7) ^ t >> 12)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx10() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[32000 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (11 + (t * (1 | (t >> 10 - (t >> 17) % 4) % 8) & (8 << (t >> 13) % 4) * (1 | (t >> 15) % 8)) * (t >> 10))
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx11() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[24000 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t * 7 : t >> 16) + (1 & t >> 14)) >> (3 & t >> (t & 2048 ? t * 2 : t * 10)) | t * t)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
    VOID WINAPI sfx12() {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        BYTE buffer[8100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

        for (DWORD t = 0; t < sizeof(buffer); ++t)
            buffer[t] = (BYTE)(
                (t * 244) >> t / ((t >> 14 & 3) + 4)
                );

        WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
    }
}