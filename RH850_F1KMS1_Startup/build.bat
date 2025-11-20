@rem RH850 Scheduler Project Build Script for CS+ CC-RH
@echo off
setlocal

set CCRH="C:\Program Files (x86)\Renesas Electronics\CS+\CC\CC-RH\V2.07.00\bin\ccrh.exe"
set ASRH="C:\Program Files (x86)\Renesas Electronics\CS+\CC\CC-RH\V2.07.00\bin\asrh.exe"
set RLINK="C:\Program Files (x86)\Renesas Electronics\CS+\CC\CC-RH\V2.07.00\bin\rlink.exe"

echo ========================================
echo Building RH850 Scheduler Project
echo ========================================

@rem Clean previous build
if exist *.obj del /Q *.obj
if exist *.o del /Q *.o
if exist utput=* del /Q utput=*
if exist firmware.elf del /Q firmware.elf
if exist firmware.hex del /Q firmware.hex

@rem Assemble startup code
echo.
echo [1/5] Assembling boot.asm...
%ASRH% -Xcpu=g3m src\boot.asm
if errorlevel 1 goto :error

@rem Compile system_rh850.c
echo.
echo [2/5] Compiling system_rh850.c...
%CCRH% -Xcpu=g3m -Iinc -c src\system_rh850.c
if errorlevel 1 goto :error

@rem Compile main.c
echo.
echo [3/5] Compiling main.c...
%CCRH% -Xcpu=g3m -Iinc -c src\main.c
if errorlevel 1 goto :error

@rem Compile scheduler.c
echo.
echo [4/7] Compiling scheduler.c...
%CCRH% -Xcpu=g3m -Iinc -c src\scheduler.c
if errorlevel 1 goto :error

@rem Compile tauj_timer.c
echo.
echo [5/7] Compiling tauj_timer.c...
%CCRH% -Xcpu=g3m -Iinc -c src\tauj_timer.c
if errorlevel 1 goto :error

@rem Compile linker_symbols.c (placeholder section symbols)
echo.
echo [6/7] Compiling linker_symbols.c...
%CCRH% -Xcpu=g3m -Iinc -c src\linker_symbols.c
if errorlevel 1 goto :error
@rem Assemble crt_symbols.asm to provide exact labels expected by startup
echo.
echo [6.5/7] Assembling crt_symbols.asm...
%ASRH% -Xcpu=g3m src\crt_symbols.asm
if errorlevel 1 goto :error

@rem Link
echo.
echo [7/7] Linking firmware.elf...
%RLINK% boot.obj system_rh850.obj main.obj scheduler.obj tauj_timer.obj linker_symbols.obj crt_symbols.obj -output=firmware.elf -list=firmware.map -start=RESET/0
if errorlevel 1 goto :error

echo.
echo ========================================
echo Build completed successfully!
echo Output: firmware.elf
echo ========================================
goto :end

:error
echo.
echo ========================================
echo Build failed with errors!
echo ========================================
exit /b 1

:end
endlocal
