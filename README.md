# wcCasino Launcher

## About

A small Win32 wrapper for the original `wcCasino` WinServer BBS door.
It adds two things the original lacks, without modifying the original
binary at all:

- **Single-instance detection** — refuses to start a second copy while
  one is already running, using a named mutex.
- **DPI compatibility fix** — automatically applies the same
  "Override high DPI scaling behavior" compatibility shim Windows
  offers via Properties > Compatibility, so the old app renders
  correctly on modern high-DPI displays.

GitHub "About" box suggestion: *Single-instance + DPI-aware launcher wrapper for the legacy wcCasino WinServer BBS door.*

## How it works

The launcher renames/wraps the real `wccasino_real.exe`, holds a mutex
for as long as it runs, and applies a per-user registry compatibility
flag before launching it — see `docs/NOTES.md` for full details.

## Build

Open `wccasino_launcher.dsw` in Visual C++ 6.0 and build. Output is
`wccasino_launcher.exe`.

## Install

Run `INSTALL_WCCASINO_LAUNCHER.BAT` from the folder containing the
original `wccasino.exe` and the built `wccasino_launcher.exe`.

## License

GPLv3 — see `LICENSE`.
