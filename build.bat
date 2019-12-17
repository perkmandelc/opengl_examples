@echo off

pushd build
copy ..\libs\SDL2-2.0.10-VC\lib\x64\SDL2.dll

cl -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7 ../src/01_triangle.c -I../libs/SDL2-2.0.10-VC/include -link -incremental:no -opt:ref ../libs/SDL2-2.0.10-VC/lib/x64/SDL2.lib ../libs/SDL2-2.0.10-VC/lib/x64/SDL2main.lib opengl32.lib -SUBSYSTEM:CONSOLE
cl -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7 ../src/02_textures.c -I../libs/SDL2-2.0.10-VC/include -link -incremental:no -opt:ref ../libs/SDL2-2.0.10-VC/lib/x64/SDL2.lib ../libs/SDL2-2.0.10-VC/lib/x64/SDL2main.lib opengl32.lib -SUBSYSTEM:CONSOLE
cl -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7 ../src/03_vertex_buffers.c -I../libs/SDL2-2.0.10-VC/include -link -incremental:no -opt:ref ../libs/SDL2-2.0.10-VC/lib/x64/SDL2.lib ../libs/SDL2-2.0.10-VC/lib/x64/SDL2main.lib opengl32.lib -SUBSYSTEM:CONSOLE
cl -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7 ../src/04_framebuffers.c -I../libs/SDL2-2.0.10-VC/include -link -incremental:no -opt:ref ../libs/SDL2-2.0.10-VC/lib/x64/SDL2.lib ../libs/SDL2-2.0.10-VC/lib/x64/SDL2main.lib opengl32.lib -SUBSYSTEM:CONSOLE
cl -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7 ../src/05_msaa.c -I../libs/SDL2-2.0.10-VC/include -link -incremental:no -opt:ref ../libs/SDL2-2.0.10-VC/lib/x64/SDL2.lib ../libs/SDL2-2.0.10-VC/lib/x64/SDL2main.lib opengl32.lib -SUBSYSTEM:CONSOLE
popd
exit 0
