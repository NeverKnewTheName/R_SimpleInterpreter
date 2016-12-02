@echo off
for %%a in (*.svg) do (
inkscape -D -z --file=%%a --export-plain-svg=_%%a
)