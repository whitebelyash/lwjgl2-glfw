package org.lwjgl;

import java.awt.*;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

public abstract class DesktopSysImplementation extends J2SESysImplementation {

    public boolean openURL(String url) {
        if(!Desktop.isDesktopSupported()) return false;
        try {
            Desktop.getDesktop().browse(new URI(url));
        }catch (IOException e) {
            e.printStackTrace(System.err);
            return false;
        } catch (URISyntaxException e) {
            e.printStackTrace(System.err);
            return false;
        }
        return false;
    }
}
