//
// Created by maks on 23.04.2026.
//
#include <GLFW/glfw3.h>

#include "common_tools.h"
#include "lwjgl_event.h"

static jclass class_DisplayMode;
static jmethodID constructor_DisplayMode;
static jclass class_GLFWDisplay;

static jmethodID method_cUpdateWindowSize;
static jmethodID method_cUpdateWindowPos;
static jmethodID method_cUpdateWindowFocus;
static jmethodID method_cUpdateWindowVisible;
static jmethodID method_cUpdateWindowHovered;
static jmethodID method_cUpdateWindowCloseRequested;

static GLFWmonitor* primaryMonitor = NULL;

static GLFWwindow* attachedWindow = NULL;
static JNIEnv *callbackEnv;

#define EVENT_BUFFER_SIZE (49)

#define EVENT_BUFFER(NAME, EVTYPE)    \
static struct {                       \
    jint count;                       \
    EVTYPE events[EVENT_BUFFER_SIZE]; \
} NAME = {                            \
    .count = 0                        \
};                                    \

EVENT_BUFFER(mouse, MouseEvent)
EVENT_BUFFER(keyboard, KeyEvent)

static MouseBuffer mouseBuffer;
static jbyte mouseKeyBuffer[8];
static jbyte* keyboardKeyBuffer;
static bool mouseMoved = false;
static bool isInGrabMode = false;
static int windowHeight = 0;


static jobject createVideoMode(JNIEnv *env, const GLFWvidmode* mode) {
    if (mode == NULL) return NULL;
    int bpp = mode->redBits + mode->greenBits + mode->blueBits;
    return (*env)->NewObject(env, class_DisplayMode, constructor_DisplayMode, mode->width, mode->height, bpp, mode->refreshRate);
}

#define EXCEPTION_DISMISS { \
    if ((*callbackEnv)->ExceptionCheck(callbackEnv)) { \
        (*callbackEnv)->ExceptionDescribe(callbackEnv); \
        (*callbackEnv)->ExceptionClear(callbackEnv); \
    } \
} \

#define ARGS(...) __VA_ARGS__
#define GLFW_CALLBACK(NAME, FUNCARGS, CALLARGS, EXTRA) \
static void callback##NAME(GLFWwindow* window FUNCARGS) { \
    if (window != attachedWindow) return; \
    (*callbackEnv)->CallStaticVoidMethod(callbackEnv, class_GLFWDisplay, method_cUpdate##NAME, CALLARGS); \
    EXCEPTION_DISMISS \
    EXTRA \
} \

GLFW_CALLBACK(WindowSize, ARGS(,int width, int height), ARGS(width, height),{
    windowHeight = height;
})
GLFW_CALLBACK(WindowPos, ARGS(,int xpos, int ypos), ARGS(xpos, ypos),)
GLFW_CALLBACK(WindowFocus, ARGS(,int focus), ARGS(focus == GLFW_TRUE),)
GLFW_CALLBACK(WindowVisible, ARGS(,int iconified), ARGS(iconified == GLFW_FALSE),)
GLFW_CALLBACK(WindowHovered, ARGS(,int entered), ARGS(entered == GLFW_TRUE),);
GLFW_CALLBACK(WindowCloseRequested,,ARGS(true),{
    glfwSetWindowShouldClose(attachedWindow, GLFW_FALSE);
})

static jint getNextEventIndex(jint *count) {
    jint index = *count;
    if (index >= EVENT_BUFFER_SIZE) index = 0;
    (*count) = index + 1;
    return index;
}

#define NEXT_EVENT(QUEUE) &QUEUE.events[getNextEventIndex(&QUEUE.count)];

static void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (window != attachedWindow) return;
    if (action == GLFW_REPEAT) return;
    jint jkey = getLwjgl2Keycode(key);
    if (jkey == -1) return;
    jbyte state = (action == GLFW_PRESS) ? 1 : 0;
    KeyEvent *pendingEvent = NEXT_EVENT(keyboard)
    pendingEvent->character = 0;
    pendingEvent->key = jkey;
    pendingEvent->state = state;
    pendingEvent->nanos = (jlong)(glfwGetTime() * 1E9);
    pendingEvent->repeat = 0;
    keyboardKeyBuffer[jkey] = state;
}

static void submitCharEvent(unsigned int codepoint, jbyte state) {
    KeyEvent *pendingEvent = NEXT_EVENT(keyboard);
    pendingEvent->character = (jint) codepoint;
    pendingEvent->key = -1;
    pendingEvent->state = state;
    pendingEvent->nanos = (jlong)(glfwGetTime() * 1E9);
    pendingEvent->repeat = 0;
}

static void callbackChar(GLFWwindow* window, unsigned int codepoint) {
    if (window != attachedWindow) return;
    submitCharEvent(codepoint, 1);
    submitCharEvent(codepoint, 0);
}

static void callbackMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (window != attachedWindow) return;
    MouseEvent *pendingEvent = NEXT_EVENT(mouse);
    pendingEvent->x = isInGrabMode ? 0 : mouseBuffer.x;
    pendingEvent->y = isInGrabMode ? 0 : mouseBuffer.y;
    pendingEvent->dwheel = 0;

    pendingEvent->button = (jbyte) button;
    const jbyte button_on = (action == GLFW_PRESS) ? 1 : 0;
    pendingEvent->state = button_on;
    if (button > 0) mouseKeyBuffer[button] = button_on;
}

static void callbackMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {
    if (window != attachedWindow) return;
    jint wheelOffset = (jint) yoffset;
    MouseEvent *pendingEvent = NEXT_EVENT(mouse);
    pendingEvent->x = isInGrabMode ? 0 : mouseBuffer.x;
    pendingEvent->y = isInGrabMode ? 0 : mouseBuffer.y;
    pendingEvent->dwheel = wheelOffset;
    pendingEvent->button = -1;
    pendingEvent->state = 0;
    mouseBuffer.dwheel += wheelOffset;
}

static void callbackMousePos(GLFWwindow* window, double xpos, double ypos) {
    mouseMoved = true;
}

static void submitMouseMovedEvent() {
    MouseEvent *pendingEvent = NEXT_EVENT(mouse)
    pendingEvent->x = mouseBuffer.x;
    pendingEvent->y = mouseBuffer.y;
    pendingEvent->dwheel = 0;
    pendingEvent->button = -1;
    pendingEvent->state = 0;
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWDisplay_sInit(JNIEnv *env, jclass clazz, jobject keyDownBuffer) {
    class_DisplayMode = (*env)->NewGlobalRef(env, (*env)->FindClass(env, "org/lwjgl/opengl/DisplayMode"));
    constructor_DisplayMode = (*env)->GetMethodID(env, class_DisplayMode, "<init>", "(IIII)V");
    class_GLFWDisplay = (*env)->NewGlobalRef(env, clazz);
    method_cUpdateWindowSize = (*env)->GetStaticMethodID(env, class_GLFWDisplay, "cUpdateWindowSize", "(II)V");
    method_cUpdateWindowPos = (*env)->GetStaticMethodID(env, class_GLFWDisplay, "cUpdateWindowPos", "(II)V");
    method_cUpdateWindowFocus = (*env)->GetStaticMethodID(env, class_GLFWDisplay, "cUpdateWindowFocus", "(Z)V");
    method_cUpdateWindowVisible = (*env)->GetStaticMethodID(env, class_GLFWDisplay, "cUpdateWindowVisible", "(Z)V");
    method_cUpdateWindowHovered = (*env)->GetStaticMethodID(env, class_GLFWDisplay, "cUpdateWindowHovered", "(Z)V");
    method_cUpdateWindowCloseRequested = (*env)->GetStaticMethodID(env, class_GLFWDisplay, "cUpdateWindowCloseRequested", "(Z)V");
    keyboardKeyBuffer = (*env)->GetDirectBufferAddress(env, keyDownBuffer);
}

JNIEXPORT jobject JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nInit(JNIEnv *env, jclass clazz) {
    int major = 0, minor = 0, rev = 0;
    glfwGetVersion(&major, &minor, &rev);
    if (major < 3 || (major == 3 && minor < 2) || major > 3) throwFormattedException(env, "Unsupported GLFW version %i.%i.%i", major, minor, rev);
    if (glfwInit() == GLFW_FALSE) {
        const char* glfw_error = NULL;
        glfwGetError(&glfw_error);
        if (glfw_error != NULL) throwFormattedException(env, "Unable to initialize GLFW: %s", glfw_error);
        else throwException(env, "Unable to initialize GLFW (unknown)");
    }
    primaryMonitor = glfwGetPrimaryMonitor();
    callbackEnv = env;
    return createVideoMode(env, glfwGetVideoMode(primaryMonitor));
}

JNIEXPORT jobjectArray JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nGetVideoModes(JNIEnv *env, jclass clazz) {
    int numModes;
    const GLFWvidmode* modes = glfwGetVideoModes(primaryMonitor, &numModes);
    if (numModes == 0) return NULL;
    jobjectArray modesArray = (*env)->NewObjectArray(env, numModes, class_DisplayMode, NULL);
    (*env)->PushLocalFrame(env, numModes);
    for (int i = 0; i < numModes; i++) {
        jobject mode = createVideoMode(env, &modes[i]);
        (*env)->SetObjectArrayElement(env, modesArray, i, mode);
    }
    (*env)->PopLocalFrame(env, NULL);
    return modesArray;
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nGoFullscreen(JNIEnv *env, jclass clazz, jint width, jint height, jint refreshRate) {
    if (attachedWindow == NULL) return;
    glfwSetWindowMonitor(attachedWindow, primaryMonitor, 0, 0, width, height, refreshRate);
    windowHeight = height;
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nReshape(JNIEnv* env, jclass clazz, jint x, jint y, jint width, jint height) {
    if (attachedWindow == NULL) return;
    glfwSetWindowMonitor(attachedWindow, NULL, x, y, width, height, GLFW_DONT_CARE);
    windowHeight = height;
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nSetTitle(JNIEnv *env, jclass clazz, jstring title) {
    if (attachedWindow == NULL) return;
    const char* utf_title = (*env)->GetStringUTFChars(env, title, NULL);
    glfwSetWindowTitle(attachedWindow, utf_title);
    (*env)->ReleaseStringUTFChars(env, title, utf_title);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nEnableGrab(JNIEnv *env, jclass clazz, jboolean enable) {
    if (attachedWindow == NULL) return;
    isInGrabMode = enable;
    glfwSetInputMode(attachedWindow, GLFW_CURSOR, enable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    if (enable) glfwSetCursorPos(attachedWindow, 0, 0);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nAttachWindow(JNIEnv *env, jclass clazz, jlong handle, jboolean resizable) {
    GLFWwindow* window = (GLFWwindow*) handle;
    attachedWindow = window;
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwSetWindowSizeCallback(window, callbackWindowSize);
    glfwSetWindowPosCallback(window, callbackWindowPos);
    glfwSetWindowFocusCallback(window, callbackWindowFocus);
    glfwSetWindowIconifyCallback(window, callbackWindowVisible);
    glfwSetCursorEnterCallback(window, callbackWindowHovered);
    glfwSetWindowCloseCallback(window, callbackWindowCloseRequested);
    glfwSetKeyCallback(window, callbackKey);
    glfwSetCharCallback(window, callbackChar);
    glfwSetMouseButtonCallback(window, callbackMouseButton);
    glfwSetCursorPosCallback(window, callbackMousePos);
    glfwSetScrollCallback(window, callbackMouseWheel);
    glfwShowWindow(window);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nDetachWindow(JNIEnv *env, jclass clazz) {
    isInGrabMode = false;
    glfwSetInputMode(attachedWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowSizeCallback(attachedWindow, NULL);
    glfwSetWindowPosCallback(attachedWindow, NULL);
    glfwSetWindowFocusCallback(attachedWindow, NULL);
    glfwSetWindowIconifyCallback(attachedWindow, NULL);
    glfwSetCursorEnterCallback(attachedWindow, NULL);
    glfwSetWindowCloseCallback(attachedWindow, NULL);
    glfwSetKeyCallback(attachedWindow, NULL);
    glfwSetCharCallback(attachedWindow, NULL);
    glfwSetMouseButtonCallback(attachedWindow, NULL);
    glfwSetCursorPosCallback(attachedWindow, NULL);
    glfwSetScrollCallback(attachedWindow, NULL);
    glfwHideWindow(attachedWindow);
    attachedWindow = NULL;
}

JNIEXPORT jobject JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nGetMouseBuffer(JNIEnv *env, jclass clazz) {
    return (*env)->NewDirectByteBuffer(env, &mouseBuffer, sizeof(mouseBuffer));
}

JNIEXPORT jobject JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nGetKeyEventBuffer(JNIEnv *env, jclass clazz) {
    return (*env)->NewDirectByteBuffer(env, &keyboard.events, sizeof(keyboard.events));
}

JNIEXPORT jobject JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nGetMouseEventBuffer(JNIEnv *env, jclass clazz) {
    return (*env)->NewDirectByteBuffer(env, &mouse.events, sizeof(mouse.events));
}

JNIEXPORT jobject JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nGetMouseKeyBuffer(JNIEnv *env, jclass clazz) {
    return (*env)->NewDirectByteBuffer(env, &mouseKeyBuffer, sizeof(mouseKeyBuffer));
}


JNIEXPORT jlong JNICALL Java_org_lwjgl_opengl_GLFWDisplay_nPollEvents(JNIEnv *env, jclass clazz) {
    glfwPollEvents();
    if (mouseMoved) {
        double xpos, ypos;
        glfwGetCursorPos(attachedWindow, &xpos, &ypos);
        if (isInGrabMode) {
            ypos *= -1;
            glfwSetCursorPos(attachedWindow, 0, 0);
        } else {
            ypos = -ypos + windowHeight;
        }
        mouseBuffer.x = (jint) xpos;
        mouseBuffer.y = (jint) ypos;
        submitMouseMovedEvent();
        mouseMoved = false;
    }else if (isInGrabMode) {
        mouseBuffer.x = mouseBuffer.y = 0;
    }
    jlong compoundEventCount = ((jlong) mouse.count) | ((jlong)keyboard.count) << 32;
    mouse.count = 0;
    keyboard.count = 0;
    return compoundEventCount;
}