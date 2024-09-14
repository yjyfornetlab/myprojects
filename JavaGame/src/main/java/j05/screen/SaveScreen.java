package j05.screen;

import java.awt.event.KeyEvent;

import asciiPanel.AsciiPanel;
import j05.Roguelike;

public class SaveScreen implements Screen{

    // private Roguelike game;
    private GameScreen parentScreen;

    public SaveScreen(GameScreen parentScreen){
        this.parentScreen = parentScreen;
    }

    @Override
    public void displayOutput(AsciiPanel terminal){
        int top = (SCREEN_HEIGHT-7)/2;
        terminal.writeCenter("-- press [1] to save map and exit -- ", top,AsciiPanel.brightWhite);
        terminal.writeCenter("-- press [2] to save game progress and exit --", top+3,AsciiPanel.brightWhite);
        terminal.writeCenter("-- press [3] to start recording --", top+6,AsciiPanel.brightWhite);
    }

    @Override
    public Screen respondToUserInput(KeyEvent key){
        switch(key.getKeyCode()){
            case KeyEvent.VK_1:
                if(parentScreen.saveMap())
                    return new PromptScreen("Map saved successfully.",0);
                break;
            case KeyEvent.VK_2:
                if(parentScreen.saveGameProgress())
                    return new PromptScreen("Game progress saved successfully.",0);
                break;
            case KeyEvent.VK_3:
                parentScreen.startRecording();
                return null;
        }
        return this;
    }

    public void saveMap(){

    }

    public void saveGameProgress(){

    }
}
