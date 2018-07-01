cl /D_WIN32=1 /W4 /wd 4820 /wd 4668 /wd 4710 /wd 4619 /wd 4826 /Wall /EHsc /nologo /MT %1.cpp %2.cpp /Zi /link /dll /OUT:..\..\..\py_libs\%1.pyd /DEBUG
