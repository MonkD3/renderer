#pragma once

#include "log.h"
#include "scene.hpp"
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
    Window* win; // The window owning the data
    void* user_data; // Attached user data
    
    // Window size related attributes
    int frame_h; // Height of the framebuffer
    int frame_w; // Width of the framebuffer
    float aspectRatio; // Aspect ratio of the screen

    // User interaction related attributes
    glm::dvec2 mouse_xy;  // Position of mouse in screen coordinates
    bool m1_pressed;
    bool m2_pressed;
};

typedef void (*cursorPosCallbackFunc)(Window*, double, double);
typedef void (*framebufferSizeCallbackFunc)(Window*, int, int);
typedef void (*mouseButtonCallbackFunc)(Window*, int, int, int);
typedef void (*mouseScrollCallbackFunc)(Window*, double, double);
typedef void (*keyCallbackFunc)(Window*, int, int, int, int);

enum callbackType {
    CALLBACK_CURSOR_POS = 0,
    CALLBACK_FRAMEBUFFER_SIZE = 1,
    CALLBACK_MOUSE_BUTTON = 2,
    CALLBACK_MOUSE_SCROLL = 3,
    CALLBACK_KEY = 4,
};

struct Window {
    GLFWwindow* win;    // The GLFW window context
    WindowData* windata; // Data attached to the window
    Window* share;       // Window to share ressources with
    Monitor* monitor;    // Monitor to which the window is attached (default is primary monitor)
    char const * name;   // The name of the window
    int width, height;   // Width and height of the window
    Scene* scene;        // The scene attached to the window
    
    bool useDefaultCursorPositionCallback = true;
    bool useDefaultFrameBufferSizeCallback = true;
    bool useDefaultMouseButtonCallback = true;
    bool useDefaultMouseScrollCallback = true;
    bool useDefaultKeyCallback = true;
    
    cursorPosCallbackFunc cursorPosCallback;
    framebufferSizeCallbackFunc framebufferSizeCallback;
    mouseButtonCallbackFunc mouseButtonCallback;
    mouseScrollCallbackFunc mouseScrollCallback;
    keyCallbackFunc keyCallback;

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
            case CALLBACK_MOUSE_SCROLL:
                useDefaultMouseScrollCallback = false;
                break;
            case CALLBACK_KEY:
                useDefaultKeyCallback = false;
                break;
        }
    };
    void inline setDefaultCallbackOn(callbackType ct) {
        DEBUG("Setting default callback %d ON", ct);
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
            case CALLBACK_MOUSE_SCROLL:
                useDefaultMouseScrollCallback = true;
                break;
            case CALLBACK_KEY:
                useDefaultKeyCallback = true;
                break;
        }
    };

    // Wrapper around Scene::addModel
    size_t addModel(Model* m, bool isRendered = true);

    // Wrapper around Scene::draw()
    void draw() const;

};

void defaultGLFWCursorPositionCallback(GLFWwindow* window, double x, double y);
void defaultGLFWFrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void defaultGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void defaultGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void defaultGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, __attribute_maybe_unused__ int mods);
