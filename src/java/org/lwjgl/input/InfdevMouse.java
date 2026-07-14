package org.lwjgl.input;

import net.java.openjdk.cacio.ctc.ExternalMouseReader;
import net.java.openjdk.cacio.ctc.InfdevGrabHandler;
import org.lwjgl.opengl.GLFWDisplay;

public class InfdevMouse implements ExternalMouseReader, GLFWDisplay.EmptyCursorGrabListener {
    static {
        InfdevGrabHandler.setMouseReader(new InfdevMouse());
    }
    private int x;
    private int y;

    @Override
    public int getX() {
        x += Mouse.getAbsX();
        return x;
    }

    @Override
    public int getY() {
        y += Mouse.getAbsY();
        return y;
    }

    @Override
    public void onGrab(boolean grabbing) {
        InfdevGrabHandler.setGrabbed(grabbing);
    }
}