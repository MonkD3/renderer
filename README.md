Hey there, welcome to my renderer !

This renderer is currently a basic abstraction layer for OpenGL. *Why ?* Because I don't want to have to rewrite a bunch of boilerplate code to read/compile shaders and perform basic user interactivity everytime I do an OpenGL project.

My goal is simply to have a library that *simplifies* OpenGL usage without changing much of its inner workings. I want to be able to write pure GL code if I need to. Basically, you can think of this library as *OpenGL syntactic sugar* (with sometimes **lots** of sugar).

In this regard, (currently) *every attribute* is public and user-modifiable. If you need access to something in order to write more complex features that are not in the library, you have it. 
In consequence, you can use parts of the library as helper (for examle : reading/compiling shaders) and then do the rest of your program in pure OpenGL.


# Default user-interactivity :

The renderer performs some default callback to enable basic user-interactivity. Each of these default can be disabled by calling `win.setDefaultCallbackOff(callbackType ct)` with the callback you want to disable :

```cpp 
enum callbackType {
    CALLBACK_CURSOR_POS = 0,
    CALLBACK_FRAMEBUFFER_SIZE = 1,
};
```

It can also be re-enabled by calling `win.setDefaultCallbackOn(CallbackType ct)`

**NOTE:** You can attach your own callbacks to the window, which will run **after** the default ones (if they are enabled). If the default callbacks are disabled, it will just run your own.

## Cursor Position Callback 

By default, the renderer attaches a cursor position callback that stores the current cursor position and computes a translation vector if the left mouse button is held-down, allowing to easily implement world translation.

## Framebuffer Size Callback 

By default, the renderer stores the width and height of the framebuffer and adapt the viewport when the window is resized.
