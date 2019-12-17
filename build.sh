cd build/
gcc ../src/01_triangle.c $(sdl2-config --cflags) -lGL -lm $(sdl2-config --libs) -o 01_triangle
gcc ../src/02_textures.c $(sdl2-config --cflags) -lGL -lm $(sdl2-config --libs) -o 02_textures
gcc ../src/03_vertex_buffers.c $(sdl2-config --cflags) -lGL -lm $(sdl2-config --libs) -o 03_vertex_buffers
gcc ../src/04_framebuffers.c $(sdl2-config --cflags) -lGL -lm $(sdl2-config --libs) -o 04_framebuffers
gcc ../src/05_msaa.c $(sdl2-config --cflags) -lGL -lm $(sdl2-config --libs) -o 05_msaa
