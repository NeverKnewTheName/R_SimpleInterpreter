@echo off
for %%a in (*.svg) do (
inkscape -D -z --file=%%a --export-png=%%~na.png
)