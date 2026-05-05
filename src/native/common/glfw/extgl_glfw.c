//
// Created by maks on 27.04.2026.
//

#include "extgl.h"

typedef void (*GLFWglproc)(void);
extern GLFWglproc glfwGetProcAddress(const char* procname);

bool extgl_Open(JNIEnv *env) {
    return true;
}

void *extgl_GetProcAddress(const char *name) {
    return glfwGetProcAddress(name);
}

void extgl_Close(void) {
}