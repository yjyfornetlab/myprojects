package j05.screen;

import asciiPanel.AsciiPanel;

import j05.Roguelike;

import javax.swing.*;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseAdapter;

public class StartScreen implements Screen{

    private Roguelike game;

    public StartScreen(Roguelike game){
        this.game = game;
    }
    
    @Override
    public void displayOutput(AsciiPanel terminal){
        terminal.writeCenter("RoguelikeGame",12,AsciiPanel.cyan);
        terminal.writeCenter("-- press [m] to go to menu --", 20);
    }

    @Override
    public Screen respondToUserInput(KeyEvent key){
        return key.getKeyCode() == KeyEvent.VK_M ? new MenuScreen(game) : this;
    }

}