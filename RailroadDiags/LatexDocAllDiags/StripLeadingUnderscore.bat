@echo off
setlocal enabledelayedexpansion
for %%a in (_*.svg) do (
set oldName=%%a
set newName=!oldName:~1!
ren !oldName! !newName!
echo !newName!
)