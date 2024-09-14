package j05.screen;

import java.awt.event.KeyEvent;

import asciiPanel.AsciiPanel;
import j05.Roguelike;

public class WinScreen implements Screen{

    private Roguelike game;

    public WinScreen(Roguelike game){
        this.game = game;
    }

    @Override
    public void displayOutput(AsciiPanel terminal){
        terminal.writeCenter("You won.",12,AsciiPanel.brightGreen);
        terminal.writeCenter("-- press [enter] to restart --",20);
    }

    @Override
    public Screen respondToUserInput(KeyEvent key){
        return key.getKeyCode() == KeyEvent.VK_ENTER ? new GameScreen(game) : this;
    }
}