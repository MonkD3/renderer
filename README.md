Hey there, welcome to my renderer !

This renderer is currently a basic abstraction layer for OpenGL. *Why ?* Because I don't want to have to rewrite a bunch of boilerplate code to read/compile shaders and perform basic user interactivity everytime I do an OpenGL project.

My goal is simply to have a library that *simplifies* OpenGL usage without changing much of its inner workings. I want to be able to write pure GL code if I need to. Basically, you can think of this library as *OpenGL syntactic sugar* (with sometimes **lots** of sugar).

In this regard, (currently) *every attribute* is public and user-modifiable. If you need access to something in order to write more complex features that are not in the library, you have it. 
In consequence, you can use parts of the library as helper (for example : reading/compiling shaders) and then do the rest of your program in pure OpenGL.

> [!NOTE] 
> I'm primarily interested in exploratory visualisation for my research topics (computational geometry / numerical methods) and will adapt the library based on these needs.

> [!WARNING] 
> The API is unstable.

# Compilation

Dependencies :
- glfw
- OpenGL (I use 4.6 as its the latest but 3.X should be fine if we adapt the shader headers)
- glm
- CMake
- A C/C++ compiler
- glad (bundled)


```bash 
mkdir build
cd build
cmake .. 
make -j
```

The project is then compiled as a shared library that you can link to your programs.

# Getting started 

The examples can be compiled by adding the option `-DENABLE_EXAMPLES=On` to cmake.

## Getting an empty window 

The [first example](./examples/000-empty-window/) show how to instantiate a window. The initialization of GLFW/GLAD is done by the library when you initialize the first window.

```cpp
#include "window.hpp"
#include <cstdio>
#include <cstdlib>

int main(void){

    // Initializing a window also initializes GLFW and GLAD
    // automatically
    Window window(800, 600, "Hello renderer", NULL, NULL);

    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
```

## First model

The [3rd example](./examples/003-mesh3D-triangle/) shows how to instantiate *models* (i.e. objects that are drawn on screen). The example's models are two triangular meshes (with only one triangle each). The visualisation is simple but it shows that in ~70 LOC (most of which is actually positions and colors) you can:

1. Instantiate a window and draw items on screen 
2. Perform basic user-input (translation, rotation, zoom)
3. Implement user-defined callbacks, here by pressing 1 and 2 on your keyboard the first/second triangle appear and disappear.

# Default user-interactivity

The renderer performs some default callback to enable basic user-interactivity. Each of these default can be disabled by calling `win.setDefaultCallbackOff(callbackType ct)` with the callback you want to disable :

```cpp 
enum callbackType {
    CALLBACK_CURSOR_POS = 0,
    CALLBACK_FRAMEBUFFER_SIZE = 1,
    CALLBACK_MOUSE_BUTTON = 2,
    CALLBACK_MOUSE_SCROLL = 3,
    CALLBACK_KEY = 4,
};
```

It can also be re-enabled by calling `win.setDefaultCallbackOn(CallbackType ct)`

**NOTE:** You can attach your own callbacks to the window, which will run **after** the default ones (if they are enabled). If the default callbacks are disabled, it will just run your own.

## Cursor Position Callback 

By default, the renderer attaches a cursor position callback that performs the following operations : 
1. Stores the current cursor position 
2. Computes a translation vector if the left mouse button is held-down and apply the corresponding transformation to the view matrix 
3. Computes a (3D) rotation if the right mouse button is held-down and apply the corresponding transformation to the view matrix

## Framebuffer Size Callback 

By default, the renderer :
1. stores the width and height of the framebuffer 
2. Adapt the viewport 
3. Adapt the view matrix to fit with the new width/heigh ratio

## Mouse Button Callback 

By default the renderer stores whether or not the right and left mouse button are held down.

## Mouse Scroll Callback 

By default, the renderer allows to zoom in and out by scrolling. Note that the zoom is computed such that it zoom towards (or away of) the mouse cursos.

## Key Callback 

By default, the renderer simply prints down the key/scancode/action/mods of the pressed keys.

# Shader Management 

> [!TODO]
>
