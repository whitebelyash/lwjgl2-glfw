//
// Created by maks on 04.03.2026.
//

#ifndef LWJGL2_CONTEXT_H
#define LWJGL2_CONTEXT_H

#include <GLFW/glfw3.h>
#include <jni.h>
#include <stdbool.h>

#define CONTEXT_CORE_PROFILE_BIT 1
#define CONTEXT_COMPATIBILITY_PROFILE_BIT 2
#define CONTEXT_ES2_PROFILE_BIT 4

#define CONTEXT_DEBUG_BIT 1
#define CONTEXT_FC_BIT 2
#define CONTEXT_ROBUST_BIT 4
#define CONTEXT_RESET_ISOLATION_BIT 8

#define CONTEXT_RELEASE_NONE 0
#define CONTEXT_RELEASE_FLUSH 0x2098

#define CONTEXT_NO_RESET_NOTIFICATION 0x8261
#define CONTEXT_LOSE_CONTEXT_ON_RESET 0x8252

typedef struct {
    int major, minor;
    int mask, flags;
    int resetStrategy;
    int releaseBehaviour;
} GLFWContextInfo;

typedef struct {
    int alpha, depth, stencil, samples, colorSamples, num_aux_buffers, accum_bpp, accum_alpha;
    bool stereo, srgb;
} GLFWPixelFormat;

typedef struct {
    bool has_info, has_pf;
    GLFWContextInfo info;
    GLFWPixelFormat pixelFormat;
} GLFWPeerInfo;

void apply_gl_context_hints(JNIEnv* env, GLFWContextInfo context_info);
void apply_pixel_format_hints(JNIEnv *env, GLFWPixelFormat pixel_format) ;

#endif //LWJGL2_CONTEXT_H