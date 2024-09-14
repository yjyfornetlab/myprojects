package j05;

import asciiPanel.AsciiPanel;
import asciiPanel.AsciiFont;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;

import j05.screen.GameScreen;
import j05.screen.Screen;
import j05.screen.StartScreen;

public class Roguelike extends JFrame implements KeyListener, MouseListener {

    private AsciiPanel terminal;
    private Screen screen;
    // private AsciiFont font;
    private boolean running;
    private Queue<KeyEvent> userInputs;
    private int framesPerSecond = 60;
    private int timePerLoop = 1000000000 / framesPerSecond;
    // private boolean recording;
    // private JMenuBar menuBar;
    // private JMenu menu;

    public Roguelike() {
        super();
        // font = new AsciiFont("rogue.png", 16, 16);
        // menuBar = new JMenuBar();
        // menu = new JMenu("A menu");
        // menuBar.add(menu);
        // setJMenuBar(menuBar);
        running = false;
        // recording = false;
        userInputs = new LinkedList<>();
        terminal = new AsciiPanel(Screen.SCREEN_WIDTH, Screen.SCREEN_HEIGHT, AsciiFont.TALRYTH_15_15);
        add(terminal);
        pack();

        // setScreen(new StartScreen(this));
        screen = new StartScreen(this);
        addKeyListener(this);
        addMouseListener(this);
        // repaint();
        // run();
    }

    public void setScreen(Screen screen) {
        this.screen = screen;
    }

    public void repaint() {
        terminal.clear();
        screen.displayOutput(terminal);
        super.repaint();
    }

    public void keyPressed(KeyEvent e) {
        userInputs.add(e);
    }

    public void keyReleased(KeyEvent e) {

    }

    public void keyTyped(KeyEvent e) {

    }

    public void mouseClicked(MouseEvent e){

    }

    public void mouseEntered(MouseEvent e){

    }

    public void mouseExited(MouseEvent e){

    }

    public void mousePressed(MouseEvent e){
        // System.out.println("pressed");
    }

    public void mouseReleased(MouseEvent e){

    }

    public void processInput() {
        KeyEvent e = userInputs.poll();
        if (e != null) {
            screen = screen.respondToUserInput(e);
        }
    }

    public void clearInputs(){
        userInputs.clear();
    }

    // public void record(){

    // }

    // public void startToRecord(){
    //     recording = true;
    // }

    public void run() {
        running = true;

        while (running) {
            long startTime = System.nanoTime();
            processInput();
            repaint();
            // System.out.println("exit");
            long endTime = System.nanoTime();

            long sleepTime = timePerLoop - (endTime - startTime);
            if (sleepTime > 0) {
                try {
                    Thread.sleep(sleepTime / 1000000);
                } catch (InterruptedException e) {
                    running = false;
                }
            }

        }
    }

    public static void main(String[] args) {
        Roguelike game = new Roguelike();
        game.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        game.setVisible(true);
        game.run();
    }
}