package org.lwjgl.opengl;

import org.lwjgl.LWJGLException;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

public class GLFWContextImplementation implements ContextImplementation {

    protected static long getTrueHandle(ByteBuffer lwjgl_handle) {
        if(lwjgl_handle == null) return 0;
        return lwjgl_handle.getLong(0);
    }

    public ByteBuffer create(PeerInfo peer_info, IntBuffer attribs, ByteBuffer shared_context_handle) throws LWJGLException {
        if(peer_info == null) throw new LWJGLException("PeerInfo must not be null");
        if(!(peer_info instanceof GLFWPeerInfo)) throw new LWJGLException("Expected GLFWPeerInfo");
        try {
            ByteBuffer peerInfoBuffer = peer_info.lockAndGetHandle();
            if(peerInfoBuffer == null) throw new LWJGLException("PeerInfo handle must not be null");
            return nCreateContext(peerInfoBuffer, getTrueHandle(shared_context_handle));
        } finally {
            peer_info.unlock();
        }

    }

    public void swapBuffers() throws LWJGLException {
        ContextGL current_context = ContextGL.getCurrentContext();
        if ( current_context == null )
            throw new IllegalStateException("No context is current");
        synchronized ( current_context ) {
            nSwapBuffers(getTrueHandle(current_context.getHandle()));
        }
    }

    public void releaseDrawable(ByteBuffer context_handle) throws LWJGLException {

    }

    public void releaseCurrentContext() throws LWJGLException {
        nMakeCurrent(0);
    }

    public void update(ByteBuffer context_handle) {

    }

    public void makeCurrent(PeerInfo peer_info, ByteBuffer handle) throws LWJGLException {
        nMakeCurrent(getTrueHandle(handle));
    }

    public boolean isCurrent(ByteBuffer handle) throws LWJGLException {
        return nIsCurrent(getTrueHandle(handle));
    }

    public void setSwapInterval(int value) {
        ContextGL current_context = ContextGL.getCurrentContext();
        if ( current_context == null )
            throw new IllegalStateException("No context is current");
        nSetSwapInterval(value);
    }

    public void destroy(PeerInfo peer_info, ByteBuffer handle) throws LWJGLException {
        nDestroy(getTrueHandle(handle));
    }

    private static native ByteBuffer nCreateContext(ByteBuffer peerInfo, long shared_context);
    private static native void nSwapBuffers(long contextHandle);
    private static native void nMakeCurrent(long contextHandle);
    private static native boolean nIsCurrent(long contextHandle);
    private static native void nSetSwapInterval(int interval);
    private static native void nDestroy(long context_handle);
}
