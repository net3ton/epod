echo off
call setup_env.bat

echo on
call c.bat gsi_pvr common

echo off
del ..\..\..\py_libs\gsi_pvr.exp
del ..\..\..\py_libs\gsi_pvr.ilk
del ..\..\..\py_libs\gsi_pvr.lib
del ..\..\..\py_libs\gsi_pvr.pdb

del gsi_pvr.obj
del common.obj
del vc80.pdb

copy ..\bin\corona.dll     ..\..\..\py_libs\corona.dll
copy ..\bin\PVRTexLib.dll  ..\..\..\py_libs\PVRTexLib.dll
