#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

struct Monitor {
    GLFWmonitor* monitor;
};

struct Window {
    GLFWwindow* win;
    Window* share;
    Monitor* monitor;
    char const * name;
    int width, height;

    Window(int _width, int _height, char const* _name, Monitor* _monitor, Window* _share);
    ~Window();
};
