package org.lwjgl.opengl;

import org.lwjgl.LWJGLException;

import java.awt.*;

public class GLFWCanvasImplementation implements AWTCanvasImplementation {
    public PeerInfo createPeerInfo(Canvas component, PixelFormat pixel_format, ContextAttribs attribs) throws LWJGLException {
        return null;
    }

    public GraphicsConfiguration findConfiguration(GraphicsDevice device, PixelFormat pixel_format) throws LWJGLException {
        return null;
    }
}
