@echo off
REM ============================================================================
REM  INSTALL_WCCASINO_LAUNCHER.BAT
REM
REM  Deploys the wcCasino single-instance/DPI launcher wrapper next to the
REM  original wccasino.exe, WITHOUT modifying wccasino.exe itself:
REM
REM    1. Renames the existing wccasino.exe to wccasino_real.exe
REM    2. Copies the newly-built launcher (wccasino_launcher.exe) into place
REM       as wccasino.exe
REM
REM  Run this from the SAME FOLDER as both wccasino.exe (the original) and
REM  wccasino_launcher.exe (built from WCCASINO_LAUNCHER.CPP via the
REM  wccasino_launcher.dsp project — note the project builds its OUTPUT
REM  as wccasino.exe directly, so if you used that as-is, rename the build
REM  output to wccasino_launcher.exe first, or just build it to a
REM  different output folder and copy it in as wccasino_launcher.exe
REM  before running this script).
REM
REM  Safe to re-run: if wccasino_real.exe already exists, it assumes the
REM  swap already happened and won't overwrite your renamed original.
REM ============================================================================

setlocal

if not exist "wccasino.exe" (
    echo ERROR: wccasino.exe not found in this folder. Run this script from
    echo the folder containing the original wcCasino installation.
    goto :end
)

if not exist "wccasino_launcher.exe" (
    echo ERROR: wccasino_launcher.exe not found in this folder. Build it
    echo from WCCASINO_LAUNCHER.CPP first and place the output here
    echo ^(named wccasino_launcher.exe, not wccasino.exe^).
    goto :end
)

if exist "wccasino_real.exe" (
    echo wccasino_real.exe already exists - it looks like the launcher is
    echo already installed. Skipping the rename step to avoid overwriting
    echo your original program.
    goto :copylauncher
)

echo Backing up the original as wccasino_real.exe...
ren "wccasino.exe" "wccasino_real.exe"
if errorlevel 1 (
    echo ERROR: Could not rename wccasino.exe. Is it currently running?
    echo Close it and try again.
    goto :end
)

:copylauncher
echo Installing the launcher as wccasino.exe...
copy /Y "wccasino_launcher.exe" "wccasino.exe" >nul
if errorlevel 1 (
    echo ERROR: Could not copy the launcher into place.
    goto :end
)

echo.
echo Done. wccasino.exe is now the single-instance/DPI-aware launcher,
echo and your original program is preserved as wccasino_real.exe.
echo Nothing else needs to change - launch wcCasino exactly as before.

:end
endlocal
pause
