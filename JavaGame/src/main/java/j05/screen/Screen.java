package j05.screen;

import asciiPanel.AsciiPanel;

import java.awt.event.KeyEvent;

public interface Screen {

    public static final int SCREEN_WIDTH = 50;
    public static final int SCREEN_HEIGHT = 30;
    
    public void displayOutput(AsciiPanel terminal);

    public Screen respondToUserInput(KeyEvent key);
}