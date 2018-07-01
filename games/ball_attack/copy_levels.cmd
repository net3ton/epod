@echo off
set DEST_DIR=..\..\bin\data.bundle\levels
set LIST=*.lvl

cd res\levels
mkdir %DEST_DIR%
for %%f in (%LIST%) do copy %%f %DEST_DIR%\%%f /Y
cd ..
cd ..

echo Ok
