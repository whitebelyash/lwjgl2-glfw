#include <jni.h>
#include "org_lwjgl_LinuxSysImplementation.h"

JNIEXPORT jint JNICALL Java_org_lwjgl_DefaultSysImplementation_getJNIVersion
  (JNIEnv *env, jobject ignored) {
        return org_lwjgl_LinuxSysImplementation_JNI_VERSION;
}
