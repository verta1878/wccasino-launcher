// ============================================================================
//  WCCASINO_LAUNCHER.CPP
//
//  A thin single-instance guard for the original wccasino.exe. This does
//  NOT modify wccasino.exe in any way — it's an external wrapper that
//  holds a named mutex for as long as the real program is running, and
//  refuses to launch a second copy while one's already active. This is
//  the safe way to add the missing "detect it's already loaded" check:
//  wccasino.exe is VB4 p-code (proprietary bytecode with no available
//  decompiler/patcher), so hex-editing it directly risks corrupting
//  offsets/jump tables I can't actually read — not an acceptable risk on
//  a binary you rely on. This wrapper sidesteps that entirely.
//
//  DEPLOYMENT:
//    1. Rename the real program:      wccasino.exe  ->  wccasino_real.exe
//    2. Build this file as:           wccasino_launcher.exe
//    3. Run INSTALL_WCCASINO_LAUNCHER.BAT to place it as wccasino.exe
//       (see docs/NOTES.md for details) — nothing else needs to change.
//    4. The launcher passes through any command-line arguments it
//       receives to wccasino_real.exe unchanged, and runs it in the same
//       working directory, so it's a transparent drop-in.
//
//  If a second launch is attempted while the first is still running, the
//  user sees a message box and nothing further happens — the real
//  program is never started a second time.
//
//  Licensed under the GNU General Public License v3.0 or later — see
//  LICENSE in the repository root.
// ============================================================================

#include <windows.h>
#include <string.h>
#include <stdio.h>

#define MUTEX_NAME "Global\\WcCasino_SingleInstance"
#define REAL_EXE_NAME "wccasino_real.exe"

#define LAUNCHER_VERSION "1.0.0"
#define LAUNCHER_REPO_URL "https://github.com/YOUR-USERNAME/wccasino-launcher"

// ----------------------------------------------------------------------
// Shows version/license info. Reached via "/ABOUT" on the command line,
// the same convention used elsewhere in this project for "/CONFIG"-style
// flags. Exits immediately after — doesn't touch the mutex or launch the
// real program, so it's safe to run alongside an already-running copy.
// ----------------------------------------------------------------------
static void ShowAboutBox()
{
    char msg[1024];
    _snprintf(msg, sizeof(msg),
        "wcCasino Launcher  v%s\n"
        "\n"
        "A single-instance guard and DPI compatibility shim for the "
        "original wcCasino, built as an external wrapper so the "
        "original program is never modified.\n"
        "\n"
        "Copyright (C) the author.\n"
        "Licensed under the GNU General Public License v3.0 or later.\n"
        "This program comes with ABSOLUTELY NO WARRANTY.\n"
        "This is free software, and you are welcome to redistribute it\n"
        "under the terms of the GPLv3 license (see LICENSE).\n"
        "\n"
        "Source: %s",
        LAUNCHER_VERSION, LAUNCHER_REPO_URL);
    msg[sizeof(msg) - 1] = '\0';

    MessageBox(NULL, msg, "About wcCasino Launcher", MB_OK | MB_ICONINFORMATION);
}

// ----------------------------------------------------------------------
// Sets the same DPI compatibility shim that Properties > Compatibility >
// "Override high DPI scaling behavior" writes when you check that box
// by hand — this is a per-user registry entry keyed by the exe's full
// path, NOT a modification to the file itself. It tells Windows to mark
// the process DPI-aware at launch instead of DPI-virtualizing it (the
// blurry bitmap-stretch behavior old GDI/VB4 apps get by default on a
// scaled display). Applying it here means you don't have to set it by
// hand on every machine this gets deployed to.
//
// "~ HIGHDPIAWARE" is the documented token Windows itself writes for
// that checkbox. This can improve crispness/rendering noticeably, but
// it's still an external shim — if wccasino_real.exe's own layout code
// has hardcoded pixel assumptions beyond what DPI virtualization was
// masking, this won't fix those; it fixes the blurry-stretch symptom,
// which is the most common form of "doesn't display right" for an old
// GDI app like this.
// ----------------------------------------------------------------------
static void ApplyDpiCompatShim(const char *exePath)
{
    HKEY hKey;
    LONG result = RegCreateKeyEx(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers",
        0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);

    if (result != ERROR_SUCCESS) return;   // non-fatal — just skip the shim if we can't set it

    const char *flagValue = "~ HIGHDPIAWARE";
    RegSetValueEx(hKey, exePath, 0, REG_SZ,
        (const BYTE*)flagValue, (DWORD)strlen(flagValue) + 1);

    RegCloseKey(hKey);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdLine, int nCmdShow)
{
    if (cmdLine && strstr(cmdLine, "/ABOUT") != NULL)
    {
        ShowAboutBox();
        return 0;
    }

    HANDLE hMutex = CreateMutex(NULL, TRUE, MUTEX_NAME);

    if (hMutex == NULL)
    {
        MessageBox(NULL, "Could not create the single-instance guard (out of "
                         "resources or a permissions issue). wcCasino was not started.",
                   "wcCasino", MB_OK | MB_ICONERROR);
        return 1;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, "wcCasino is already running.", "wcCasino", MB_OK | MB_ICONINFORMATION);
        CloseHandle(hMutex);
        return 0;
    }

    // --- Build the path to the real executable, same folder as this launcher ---
    char launcherPath[MAX_PATH];
    GetModuleFileName(NULL, launcherPath, MAX_PATH);

    char realExePath[MAX_PATH];
    strncpy(realExePath, launcherPath, MAX_PATH - 1);
    realExePath[MAX_PATH - 1] = '\0';

    char *lastSlash = strrchr(realExePath, '\\');
    if (lastSlash)
        strcpy(lastSlash + 1, REAL_EXE_NAME);
    else
        strcpy(realExePath, REAL_EXE_NAME);

    // Apply the DPI compatibility shim (see ApplyDpiCompatShim above)
    // every launch — cheap, idempotent, and means this stays applied
    // even if someone clears app compat flags system-wide.
    ApplyDpiCompatShim(realExePath);

    // --- Build a command line: "path\to\wccasino_real.exe" <passthrough args> ---
    char commandLine[2048];
    _snprintf(commandLine, sizeof(commandLine), "\"%s\" %s", realExePath, cmdLine ? cmdLine : "");
    commandLine[sizeof(commandLine) - 1] = '\0';

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL launched = CreateProcess(
        realExePath,      // application path
        commandLine,      // full command line (path + args)
        NULL, NULL,       // default security attributes
        FALSE,            // don't inherit handles
        0,                // no special creation flags
        NULL,             // inherit our environment
        NULL,             // inherit our working directory
        &si, &pi);

    if (!launched)
    {
        char errBuf[300];
        _snprintf(errBuf, sizeof(errBuf),
            "Could not start %s (error %lu). Make sure it's in the same "
            "folder as this launcher and hasn't been renamed.",
            REAL_EXE_NAME, GetLastError());
        MessageBox(NULL, errBuf, "wcCasino", MB_OK | MB_ICONERROR);
        CloseHandle(hMutex);
        return 1;
    }

    // Hold the mutex for as long as the real program is running — this
    // IS the single-instance guard. As soon as wccasino_real.exe exits,
    // we exit too, releasing the mutex, so the next launch succeeds.
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(hMutex);
    return 0;
}
