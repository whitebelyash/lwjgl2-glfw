//
// Created by maks on 05.03.2026.
//

#include "context.h"
#include <jni.h>
#include "common_tools.h"

static void unpack_pixel_format(JNIEnv *env, jobject pixel_format, GLFWPixelFormat* my_pixel_format) {
    const jclass cls_pixel_format = (*env)->GetObjectClass(env, pixel_format);
    my_pixel_format->alpha = (int)(*env)->GetIntField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "alpha", "I"));
    my_pixel_format->depth = (int)(*env)->GetIntField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "depth", "I"));
    my_pixel_format->stencil = (int)(*env)->GetIntField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "stencil", "I"));
    my_pixel_format->samples = (int)(*env)->GetIntField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "samples", "I"));
    my_pixel_format->colorSamples = (int)(*env)->GetIntField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "colorSamples", "I"));
    my_pixel_format->num_aux_buffers = (int)(*env)->GetIntField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "num_aux_buffers", "I"));
    my_pixel_format->accum_bpp = (int)(*env)->GetIntField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "accum_bpp", "I"));
    my_pixel_format->accum_alpha = (int)(*env)->GetIntField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "accum_alpha", "I"));

    my_pixel_format->stereo = (bool)(*env)->GetBooleanField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "stereo", "Z"));
    my_pixel_format->srgb = (bool)(*env)->GetBooleanField(env, pixel_format, (*env)->GetFieldID(env, cls_pixel_format, "sRGB", "Z"));
}

static void unpack_attribs(JNIEnv* env, jobject attribs, GLFWContextInfo* my_attribs) {
    const jclass cl_attribs = (*env)->GetObjectClass(env, attribs);
    my_attribs->major = (int)(*env)->GetIntField(env, attribs, (*env)->GetFieldID(env, cl_attribs, "majorVersion", "I"));
    my_attribs->minor = (int)(*env)->GetIntField(env, attribs, (*env)->GetFieldID(env, cl_attribs, "minorVersion", "I"));
    my_attribs->mask = (int)(*env)->GetIntField(env, attribs, (*env)->GetFieldID(env, cl_attribs, "profileMask", "I"));
    my_attribs->flags = (int)(*env)->GetIntField(env, attribs, (*env)->GetFieldID(env, cl_attribs, "contextFlags", "I"));
    my_attribs->resetStrategy = (int)(*env)->GetIntField(env, attribs, (*env)->GetFieldID(env, cl_attribs, "contextResetNotificationStrategy", "I"));
    my_attribs->releaseBehaviour = (int)(*env)->GetIntField(env, attribs, (*env)->GetFieldID(env, cl_attribs, "contextReleaseBehaviour", "I"));
}

JNIEXPORT jobject JNICALL Java_org_lwjgl_opengl_GLFWPeerInfo_createHandle
  (JNIEnv *env, jclass clazz, jobject attribs, jobject pixel_format) {
    jobject buffer = newJavaManagedByteBuffer(env, sizeof(GLFWPeerInfo));
    GLFWPeerInfo* peer_info = (*env)->GetDirectBufferAddress(env, buffer);
    if (pixel_format != NULL) {
        unpack_pixel_format(env, pixel_format, &peer_info->pixelFormat);
        peer_info->has_pf = true;
    }
    if (attribs != NULL) {
        unpack_attribs(env, attribs, &peer_info->info);
        peer_info->has_info = true;
    }
    return buffer;
}