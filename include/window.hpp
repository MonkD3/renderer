#pragma once

#include "log.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>

struct Monitor {
    GLFWmonitor* monitor;
};

// Forward declaration for WindowData @win attribute
typedef struct Window Window;

// Default data attached to a window
struct WindowData {
    glm::dvec2 mouse_screen_xy;  // Position of mouse in screen coordinates
    glm::dvec2 mouse_world_xy; // Position of mouse in world coordinates
    glm::mat4  mvp;  // Model-Viewport-Projection matrix
    glm::mat4  imvp; // inverse of Model-Viewport-Projection matrix
    int frame_h; // Height of the framebuffer
    int frame_w; // Width of the framebuffer
    float aspectRatio; // Aspect ratio of the screen
    void* user_data; // Attached user data
    Window* win; // The window owning the data

    bool m1_pressed;
    bool m2_pressed;
};

typedef void (*cursorPosCallbackFunc)(Window*, double, double);
typedef void (*framebufferSizeCallbackFunc)(Window*, int, int);
typedef void (*mouseButtonCallbackFunc)(Window*, int, int, int);

enum callbackType {
    CALLBACK_CURSOR_POS = 0,
    CALLBACK_FRAMEBUFFER_SIZE = 1,
    CALLBACK_MOUSE_BUTTON = 2
};

struct Window {
    GLFWwindow* win;    // The GLFW window context
    WindowData* windata; // Data attached to the window
    Window* share;       // Window to share ressources with
    Monitor* monitor;    // Monitor to which the window is attached (default is primary monitor)
    char const * name;   // The name of the window
    int width, height;   // Width and height of the window
    
    bool useDefaultCursorPositionCallback = true;
    bool useDefaultFrameBufferSizeCallback = true;
    bool useDefaultMouseButtonCallback = true;
    
    cursorPosCallbackFunc cursorPosCallback;
    framebufferSizeCallbackFunc framebufferSizeCallback;
    mouseButtonCallbackFunc mouseButtonCallback;

    Window(int _width, int _height, char const* _name, Monitor* _monitor, Window* _share);
    ~Window();

    void inline setDefaultCallbackOff(callbackType ct) {
        DEBUG("Setting default callback %d OFF", ct);
        switch (ct) {
            case CALLBACK_CURSOR_POS:
                useDefaultCursorPositionCallback = false;
                break;
            case CALLBACK_FRAMEBUFFER_SIZE:
                useDefaultFrameBufferSizeCallback = false;
                break;
            case CALLBACK_MOUSE_BUTTON:
                useDefaultMouseButtonCallback = false;
                break;
        }
    };
    void inline setDefaultCallbackOn(callbackType ct) {
        switch (ct) {
            case CALLBACK_CURSOR_POS:
                useDefaultCursorPositionCallback = true;
                break;
            case CALLBACK_FRAMEBUFFER_SIZE:
                useDefaultFrameBufferSizeCallback = true;
                break;
            case CALLBACK_MOUSE_BUTTON:
                useDefaultMouseButtonCallback = true;
                break;
        }
    };

};

void defaultGLFWCursorPositionCallback(GLFWwindow* window, double x, double y);
void defaultGLFWFrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void defaultGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
