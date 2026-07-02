package org.lwjgl.input;

import net.java.openjdk.cacio.ctc.ExternalMouseReader;
import net.java.openjdk.cacio.ctc.InfdevGrabHandler;

public class InfdevMouse implements ExternalMouseReader, Mouse.EmptyCursorGrabListener {
    static {
        InfdevGrabHandler.setMouseReader(new InfdevMouse());
    }

    @Override
    public int getX() {
        return Mouse.getX();
    }

    @Override
    public int getY() {
        return Mouse.getY();
    }

    @Override
    public void onGrab(boolean grabbing) {
        InfdevGrabHandler.setGrabbed(grabbing);
    }
}