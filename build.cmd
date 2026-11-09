@echo off
rem Builds dist\SKSE\Plugins\DeviousCurses.dll with the CMake preset (VS 2022 + its bundled vcpkg).
setlocal
cd /d "%~dp0"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`call "%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSPATH=%%i"
if not defined VSPATH (
	echo Visual Studio C++ build tools not found.
	exit /b 1
)
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
set "VCPKG_ROOT=%VSPATH%\VC\vcpkg"
set "PATH=%VSPATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja;%PATH%"

cmake --preset release || exit /b 1
cmake --build build\release || exit /b 1
echo.
echo Built: %~dp0dist\SKSE\Plugins\DeviousCurses.dll
