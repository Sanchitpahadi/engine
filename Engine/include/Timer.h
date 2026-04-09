#include<GLFW/glfw3.h>
class Timer {
    float last = 0;
public:
    float Delta() {
        float now = glfwGetTime();
        float dt  = now - last;
        last = now;
        return dt;
    }
};