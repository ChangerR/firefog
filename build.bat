@echo on
call env.bat
nmake -f makefile clean
nmake
@echo off