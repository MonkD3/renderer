#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/ext/vector_double2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "window.hpp"
#include "glad/gl.h"
#include "log.h"
#include "scene.hpp"
#include "uniforms.hpp"

void defaultGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;

    if (win->useDefaultKeyCallback){
        printf("Key pressed : %c, scancode %d, action %d, mods %d\n", key, scancode, action, mods);
    }
    
    if (win->keyCallback) win->keyCallback(win, key, scancode, action, mods);
}

void defaultGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;
    Scene* scene = win->scene;
    WorldUniformBlock& world = scene->worldBlock;

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

        newZoom = fmax(0.001f, newZoom * world.scalings[1]);
        const float scaling = newZoom / world.scalings[1];
        world.scalings[1] = newZoom;

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
                world.view[i][j] *= scaling;
            }
        }

        world.view[3][0] = (world.view[3][0] - xpos) * scaling + xpos;
        world.view[3][1] = (world.view[3][1] - ypos) * scaling + ypos;

        world.iview = glm::inverse(world.view);
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
    Scene* scene = win->scene;
    WorldUniformBlock& world = scene->worldBlock;

    int const w = windata->frame_w;
    int const h = windata->frame_h;

    if (win->useDefaultCursorPositionCallback){
        windata->mouse_xy[0] = x;
        windata->mouse_xy[1] = y;

        // Normalized coordinates
        double x_n = (2.f*x - w)/w;
        double y_n = (h - 2.f*y)/h;

        // Execute only on first call
        static double prev_x_n = x_n;
        static double prev_y_n = y_n;

        // Translate if m1 is held down
        if (windata->m1_pressed) {
            world.view[3][0] -= prev_x_n - x_n;
            world.view[3][1] -= prev_y_n - y_n;
            world.iview = glm::inverse(world.view);
        }
        // Rotate if m2 is held down
        else if (windata->m2_pressed) {
            // Compute normal to movement
            double x_normal = - (y_n - prev_y_n);
            double y_normal = x_n - prev_x_n;
            float d = 100.f*hypot(x_n - prev_x_n, y_n - prev_y_n);

            // Fun things of linear algebra :
            // Suppose R is a rotation matrix around axis a.
            // Mutliplying R with a transformation matrix T does not 
            // rotate the space around axis a, it actually rotates the
            // space around axis Ta ! 
            // Therefore, to rotate the space around a we need to apply the 
            // rotation matrix around axis b = T^{-1}a, then : Tb = TT^{-1}a = a
            glm::vec4 rotAxis = world.iview * glm::vec4(x_normal, y_normal, 0.0f, 0.0f);
            world.view = glm::rotate(world.view, glm::radians(d), glm::vec3(rotAxis[0], rotAxis[1], rotAxis[2]));  

            world.iview = glm::inverse(world.view);
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
    Scene* scene = win->scene;
    WorldUniformBlock& world = scene->worldBlock;

    if (win->useDefaultFrameBufferSizeCallback){
        glViewport(0, 0, width, height); 

        float const wRatio = (float) windata->frame_w / width;
        float const hRatio = (float) windata->frame_h / height;

        /*float const curAspectRatio = windata->aspectRatio;*/
        float const newAspectRatio = (float) width / height;

        // Keep the area of models constant. We do this by scaling with the 
        // inverse of the ratio of lengths in x and y directions
        // i.e. The ratio of lengths in x is : newHeight / currentHeight 
        //      so we scale the x axis by the inverse ratio : newHeight / currentHeight 
        for (int i = 0; i < 3; i++) {
            world.view[i][0] *= wRatio;
            world.view[i][1] *= hRatio;
        }

        world.iview = glm::inverse(world.view);

        windata->aspectRatio = newAspectRatio;
        world.scalings[0] = newAspectRatio;
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

    // ========================= Initialize scene =============================
    scene = new Scene();

    // Get general aspect-ratio of the buffer
    int frame_w;
    int frame_h;
    glfwGetFramebufferSize(win, &frame_w, &frame_h);
    float const aspectRatio = (float)frame_w / frame_h;

    WorldUniformBlock& world = scene->worldBlock;
    world.scalings[0] = aspectRatio;
    world.view = glm::scale(world.view, glm::vec3(1.0f, aspectRatio, 1.0f)); // Apply the aspect-ratio
    world.iview = glm::inverse(world.view); // Get the inverse of the transform
    world.scalings[2] = 1.0f;

    // ========================= Initialize window data =======================
    windata = new WindowData;
    glfwSetWindowUserPointer(win, windata);

    windata->aspectRatio = aspectRatio;
    windata->frame_h = frame_h;
    windata->frame_w = frame_w;
    windata->win = this;
    windata->m1_pressed = false;
    windata->m2_pressed = false;

    // ======================== Attach default callbacks ======================
    // Set the custom callback to null
    cursorPosCallback = NULL;
    framebufferSizeCallback = NULL;
    mouseButtonCallback = NULL;
    mouseScrollCallback = NULL;
    keyCallback = NULL;

    // Attach the default callbacks to the GLFW window
    glfwSetCursorPosCallback(win, defaultGLFWCursorPositionCallback);
    glfwSetFramebufferSizeCallback(win, defaultGLFWFrameBufferSizeCallback);
    glfwSetMouseButtonCallback(win, defaultGLFWMouseButtonCallback);
    glfwSetScrollCallback(win, defaultGLFWScrollCallback);
    glfwSetKeyCallback(win, defaultGLFWKeyCallback);

}

Window::~Window(){
    DEBUG("Destroying window '%s'", name);
    glfwDestroyWindow(win);
    delete windata;
    delete scene;
}

size_t Window::addModel(Model* m, bool isRendered){
    return scene->addModel(m, isRendered);
}

void Window::draw() const {
    scene->draw();
}
