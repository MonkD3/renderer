#include <cstdlib>

#include "window.hpp"
#include "log.h"

Window::Window(int _width, int _height, char const* _name, Monitor* _monitor, Window* _share) : name(_name), width(_width), height(_height){
    DEBUG("Creating window '%s'", name);
    glfwInit();

    GLFWmonitor* m = _monitor == NULL ? NULL : _monitor->monitor;
    GLFWwindow* w = _share == NULL ? NULL : _share->win;

    win = glfwCreateWindow(width, height, name, m, w); 
    share = _share;
    monitor = _monitor;

    if (win == NULL){
        glfwTerminate();
        CRITICAL_ERROR("Failed to initialize window '%s'", name);
        exit(0);
    }

    glfwMakeContextCurrent(win);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        glfwTerminate();
        CRITICAL_ERROR("Failed to initialize GLAD");
        exit(0);
    }
    printf("Loaded OpenGL %d.%d with GLAD\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
}

Window::~Window(){
    DEBUG("Destroying window '%s'", name);
    glfwDestroyWindow(win);
}
