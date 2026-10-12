@echo off
setlocal
if "%~1"=="" (
    echo Usage: run_cp949.cmd "full path to the built exe"
    exit /b 1
)
set "bank_exe=%~f1"
chcp 949 >nul
pushd "%~dp0"
"%bank_exe%"
set "bank_result=%errorlevel%"
popd
exit /b %bank_result%
