//
// Created by maks on 07.03.2026.
//

#include "context.h"
#include <stdlib.h>
#include "common_tools.h"

void apply_gl_context_hints(JNIEnv* env, const GLFWContextInfo context_info) {
    int clientApi;
    if (context_info.mask & (CONTEXT_CORE_PROFILE_BIT | CONTEXT_COMPATIBILITY_PROFILE_BIT) || context_info.mask == 0) {
        clientApi = GLFW_OPENGL_API;
        int profile = GLFW_OPENGL_COMPAT_PROFILE;
        if (context_info.mask & CONTEXT_CORE_PROFILE_BIT) {
            profile = GLFW_OPENGL_CORE_PROFILE;
        }
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (context_info.flags & CONTEXT_FC_BIT) ? GLFW_TRUE : GLFW_FALSE);
    } else if (context_info.mask & CONTEXT_ES2_PROFILE_BIT) {
        clientApi = GLFW_OPENGL_ES_API;
    } else {
        throwFormattedException(env, "Unexpected context mask: %x", context_info.mask);
        return;
    }
    glfwWindowHint(GLFW_CLIENT_API, clientApi);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, (context_info.flags & CONTEXT_DEBUG_BIT) ? GLFW_TRUE : GLFW_FALSE);
    int contextReleaseBehavior;
    switch (context_info.releaseBehaviour) {
        case CONTEXT_RELEASE_NONE: contextReleaseBehavior = GLFW_RELEASE_BEHAVIOR_NONE; break;
        case CONTEXT_RELEASE_FLUSH: contextReleaseBehavior = GLFW_RELEASE_BEHAVIOR_FLUSH; break;
        default: contextReleaseBehavior = GLFW_ANY_RELEASE_BEHAVIOR; break;
    }
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, contextReleaseBehavior);
    if (context_info.flags & CONTEXT_ROBUST_BIT) {
        int robustnessStrategy;
        switch (context_info.resetStrategy) {
            case CONTEXT_LOSE_CONTEXT_ON_RESET: robustnessStrategy = GLFW_LOSE_CONTEXT_ON_RESET; break;
            case CONTEXT_NO_RESET_NOTIFICATION: robustnessStrategy = GLFW_NO_RESET_NOTIFICATION; break;
            default: robustnessStrategy = GLFW_NO_RESET_NOTIFICATION;
        }
        glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, robustnessStrategy);
    }else {
        glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_info.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_info.minor);
}

void apply_pixel_format_hints(JNIEnv *env, const GLFWPixelFormat pixel_format) {
    // TODO make me give a shit
}