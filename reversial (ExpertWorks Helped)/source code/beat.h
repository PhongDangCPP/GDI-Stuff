#pragma once
VOID WINAPI sfx1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[8100 * 40] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            (5 * sin(t * t >> 9 & t >> 10 | t >> 11 & 24 ^ t >> 10 & 15 & t >> 15) * 12)
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

    BYTE buffer[8100 * 25] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            (sin(t * (1 + (t >> 10) * (43 + 2 * (t >> 15 - (t >> 16) % 13) % 8) % 8) * (1 + (t >> 14) % 4)) * 17)
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16100, 16100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[16100 * 45] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            (430 * sin(5 * t >> 11 | 5 * t >> 1) * 17)
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16100, 16100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[16100 * 30] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            (t >> (t * t >> 1) * t / ((t >> 13 & 3) + 4) & -t >> 11)
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16100, 16100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[16100 * 35] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            sin(t * (t ^ t + (t >> 15 | 1) + (t - 1280 ^ t) >> 10)) * 17
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[32000 * 25] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            ((sin((t >> 1) * (15 & 0x6568a90 >> (t >> 8 & 28)) | t >> 1 >> (t >> 11) ^ t >> 12) + (t >> 4 & t & 24)) * 127)
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16100, 16100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[16100 * 45] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            ((t & t >> 12) * (t & t >> 4 | t + t >> 8 | t - t >> 16) - 1)
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[8100 * 35] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            (t * sin((1 + (t >> 10) * (43 + 2 * (t >> 15 - (t >> 16) % 13) % 8) % 8) * (1 + (t >> 14) % 4)) * 66)
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[8100 * 35] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            (t * sin((1 + (t >> 8) * (43 + 2 * (t >> 15 - (t >> 16) % 13) % 8) % 8) * (1 + (t >> 14) % 4)) * 66)
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sfx10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    BYTE buffer[8100 * 50] = {};  // Changed from char[12100*30] to BYTE[8100*30]

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = (BYTE)(
            (t * sin((1 + (t >> 6) * (43 + 2 * (t >> 11 - (t >> 14) % 13) % 8) % 8) * (1 + (t >> 11) % 4)) * 66)
            );

    WAVEHDR header = { (LPSTR)buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}