#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/ext/vector_double2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <sys/cdefs.h>

#include "window.hpp"
#include "glad/gl.h"
#include "log.h"

void defaultGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;

    if (win->useDefaultMouseScrollCallback){
        int const w = windata->frame_w;
        int const h = windata->frame_h;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // Normalize cursor position
        xpos = (2.f*xpos - w)/w;
        ypos = (h - 2.f*ypos)/h;

        const float zoomStrength = 1.05f;
        float newZoom;
        if (yoffset > 0.f) newZoom = zoomStrength; // zooming in
        else newZoom = 1.f / zoomStrength;       // zomming out

        newZoom = fmax(0.001f, newZoom * windata->zoom);
        const float scaling = newZoom / windata->zoom;
        windata->zoom = newZoom;

        // Perform in sequence :
        // 1) a translation towards the mouse
        // 2) a scaling 
        // 3) a translation translation back towards the center
        //      [tx']   [1 0 mx] [zx 0  0] [1 0 -mx] [tx]
        //      [ty'] = [0 1 my] [0  zy 0] [0 1 -mx] [ty]
        //      [1]     [0 0 1 ] [0  0  1] [0 0   1] [1]
        //
        // This is not doable with GLM because glm::scale 
        // DOES NOT scale the translation
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                windata->mvp[i][j] *= scaling;
            }
        }

        windata->mvp[3][0] = (windata->mvp[3][0] - xpos) * scaling + xpos;
        windata->mvp[3][1] = (windata->mvp[3][1] - ypos) * scaling + ypos;

        windata->imvp = glm::inverse(windata->mvp);
    }

    // Call the user-defined callback
    if (win->mouseScrollCallback) win->mouseScrollCallback(win, xoffset, yoffset);
}

void defaultGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, __attribute_maybe_unused__ int mods){
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;

    if (win->useDefaultMouseButtonCallback){
        switch (button) {
            case GLFW_MOUSE_BUTTON_1:
                windata->m1_pressed = action > 0; // RELEASE=0, PRESS=1, REPEAT=2
                break;
            case GLFW_MOUSE_BUTTON_2:
                windata->m2_pressed = action > 0; // RELEASE=0, PRESS=1, REPEAT=2
                break;
        }
    }

    // Call the user-defined callback
    if (win->mouseButtonCallback) win->mouseButtonCallback(win, button, action, mods);
}

void defaultGLFWCursorPositionCallback(GLFWwindow* window, double x, double y){
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;

    int const w = windata->frame_w;
    int const h = windata->frame_h;

    if (win->useDefaultCursorPositionCallback){
        windata->mouse_screen_xy[0] = x;
        windata->mouse_screen_xy[1] = y;

        // Normalized coordinates
        double x_n = (2.f*x - w)/w;
        double y_n = (h - 2.f*y)/h;

        // Execute only on first call
        static double prev_x_n = x_n;
        static double prev_y_n = y_n;

        // Translate if m1 is held down
        if (windata->m1_pressed) {
            windata->mvp = glm::translate(windata->mvp, glm::vec3(x_n - prev_x_n, y_n - prev_y_n, 0.0f));
            windata->imvp = glm::inverse(windata->mvp);
        }

        // Update
        prev_x_n = x_n;
        prev_y_n = y_n;
    }

    // Call the user-defined callback
    if (win->cursorPosCallback) win->cursorPosCallback(win, x, y);
}

void defaultGLFWFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;

    if (win->useDefaultFrameBufferSizeCallback){
        glViewport(0, 0, width, height); 
        windata->frame_h = height;
        windata->frame_w = width;
    }

    // Call the user-defined callback
    if (win->framebufferSizeCallback) win->framebufferSizeCallback(win, width, height);
}


Window::Window(int _width, int _height, char const* _name, Monitor* _monitor, Window* _share) : name(_name), width(_width), height(_height){
    DEBUG("Creating window '%s'", name);
    glfwInit();

    // =========================== Create window =============================
    GLFWmonitor* m = _monitor == NULL ? NULL : _monitor->monitor;
    GLFWwindow* w = _share == NULL ? NULL : _share->win;
    share = _share;
    monitor = _monitor;

    win = glfwCreateWindow(width, height, name, m, w); 
    if (win == NULL){
        glfwTerminate();
        CRITICAL_ERROR("Failed to initialize window '%s'", name);
        exit(0);
    }
    glfwMakeContextCurrent(win);

    // =========================== Load GLAD ==================================
    int const version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        glfwTerminate();
        CRITICAL_ERROR("Failed to initialize GLAD");
        exit(0);
    }
    DEBUG("Loaded OpenGL %d.%d with GLAD for window '%s'", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version), name);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLAD_VERSION_MAJOR(version));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLAD_VERSION_MINOR(version));
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ========================= Initialize window data =======================
    windata = new WindowData;
    glfwSetWindowUserPointer(win, windata);

    windata->frame_h = height;
    windata->frame_w = width;

    // Get general aspect-ratio of the monitor
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    windata->aspectRatio = (float) mode->width / mode->height;

    windata->win = this;

    windata->zoom = 1.0f;
    windata->mvp = glm::mat4(1.0f); // Identity matrix
    windata->mvp = glm::scale(windata->mvp, glm::vec3(windata->aspectRatio,1.0f, 1.0f)); // Apply the aspect-ratio

    windata->imvp = glm::inverse(windata->mvp); // Get the inverse of the transform

    // ======================== Attach default callbacks ======================
    // Set the custom callback to null
    cursorPosCallback = NULL;
    framebufferSizeCallback = NULL;
    mouseButtonCallback = NULL;
    mouseScrollCallback = NULL;

    // Attach the default callbacks to the GLFW window
    glfwSetCursorPosCallback(win, defaultGLFWCursorPositionCallback);
    glfwSetFramebufferSizeCallback(win, defaultGLFWFrameBufferSizeCallback);
    glfwSetMouseButtonCallback(win, defaultGLFWMouseButtonCallback);
    glfwSetScrollCallback(win, defaultGLFWScrollCallback);
}

Window::~Window(){
    DEBUG("Destroying window '%s'", name);
    glfwDestroyWindow(win);
    delete windata;
}


