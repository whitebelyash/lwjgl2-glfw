//
// Created by maks on 07.03.2026.
//

#include "context.h"
#include "common_tools.h"

JNIEXPORT jobject JNICALL Java_org_lwjgl_opengl_GLFWContextImplementation_nCreateContext
    (JNIEnv *env, jclass clazz, jobject peerInfo, jlong sharedHandle) {
    GLFWPeerInfo* peer_info = (*env)->GetDirectBufferAddress(env, peerInfo);
    GLFWwindow* shareContext = (GLFWwindow*) sharedHandle;
    glfwDefaultWindowHints();
    if (peer_info->has_info) apply_gl_context_hints(env, peer_info->info);
    if (peer_info->has_pf) apply_pixel_format_hints(env, peer_info->pixelFormat);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "LWJGL2-GLFW Window", NULL, shareContext);

    jobject buffer = newJavaManagedByteBuffer(env, sizeof(jlong));
    jlong* ptr = (*env)->GetDirectBufferAddress(env, buffer);
    memset(ptr, 0, sizeof(jlong));
    *ptr = (jlong) window;
    return buffer;
}

JNIEXPORT jboolean JNICALL Java_org_lwjgl_opengl_GLFWContextImplementation_nIsCurrent
    (JNIEnv *env, jclass clazz, jlong contextHandle) {
    const GLFWwindow* window = (GLFWwindow*) contextHandle;
    return glfwGetCurrentContext() == window;
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWContextImplementation_nSetSwapInterval
    (JNIEnv *env, jclass clazz, jint swapInterval) {
    glfwSwapInterval(swapInterval);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWContextImplementation_nMakeCurrent
    (JNIEnv *env, jclass clazz, jlong contextHandle) {
    GLFWwindow* window = (GLFWwindow*) contextHandle;
    glfwMakeContextCurrent(window);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWContextImplementation_nSwapBuffers
    (JNIEnv *env, jclass clazz, jlong contextHandle) {
    GLFWwindow* window = (GLFWwindow*) contextHandle;
    glfwSwapBuffers(window);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWContextImplementation_nDestroy
    (JNIEnv *env, jclass clazz, jlong contextHandle) {
    GLFWwindow* window = (GLFWwindow*) contextHandle;
    glfwDestroyWindow(window);
}