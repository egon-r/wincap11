# wincap11
Take screenshots on Windows 11 with transparent rounded corners. 

> Snipping Tool
>
> ![](/README_assets/calc_snipping_tool2.png)

> wincap11
> 
> ![](/README_assets/calc_wincap112.png)


# Build
### Requirements
- cmake, c++ toolchain
- Qt6
- NSIS

### Create Setup
1. Check that `CMAKE_PREFIX_PATH` in `CMakeLists.txt` points to Qt (e.g. `"C:/Qt/6.6.0/mingw_64"`)
2. Use `cmake` to create makefiles/project
3. Build
4. `makensis installer.nsi`