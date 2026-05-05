package org.lwjgl.opengl;

import org.lwjgl.LWJGLException;

import java.nio.ByteBuffer;

public class GLFWPeerInfo extends PeerInfo {
    GLFWPeerInfo(ContextAttribs contextAttribs, PixelFormat pixelFormat) {
        super(createHandle(contextAttribs, pixelFormat));
    }
    private static native ByteBuffer createHandle(ContextAttribs attribs, PixelFormat pixelFormat);

    protected void doLockAndInitHandle() throws LWJGLException {

    }

    protected void doUnlock() throws LWJGLException {

    }
}
