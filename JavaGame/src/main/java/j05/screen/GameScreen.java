package j05.screen;

import asciiPanel.AsciiPanel;

import j05.Roguelike;
import j05.gameio.GameProgressSaver;
import j05.gameio.GameRecorder;
import j05.gameio.MapSaver;
import j05.world.World;
import j05.world.WorldBuilder;
import rlforj.los.IFovAlgorithm;
import rlforj.los.PrecisePermissive;
import rlforj.los.ShadowCasting;
import j05.entity.Creature;
import j05.entity.Item;
import j05.entity.Monster;
import j05.entity.Player;
import j05.entity.Tile;

import java.awt.Color;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class GameScreen implements Screen {

    private Roguelike game;
    private World world;
    private Screen subscreen;
    private static final int WORLD_WIDTH = 100;
    private static final int WORLD_HEIGHT = 100;
    // private Creature player;
    private Player player;
    private boolean recording;
    private GameRecorder recorder;
    private char[][] rGlyphs;
    private Color[][] rColors;
    private String[] ss;
    private boolean isPlayback;
    private List<char[][]> glyphs;
    private List<Color[][]> colors;
    private List<String[]> status;
    private int frame;
    // private boolean loadMap;
    // private boolean loadProgress;

    public GameScreen(Roguelike game) {
        this.game = game;
        createWorld();
        initResource();
        initField();
        this.isPlayback = false;
        // this.loadMap = false;
        // this.loadProgress = false;
    }

    public GameScreen(Roguelike game, Tile[][] map) {
        this.game = game;
        world = new World(map);
        initResource();
        initField();
        this.isPlayback = false;
    }

    public GameScreen(Roguelike game, World world) {
        this.game = game;
        this.world = world;
        player = world.getPlayer();
        world.setMonsters();
        initField();
        this.isPlayback = false;
    }

    public GameScreen(Roguelike game, List<char[][]> glyphs, List<Color[][]> colors, List<String[]> status) {
        this.glyphs = glyphs;
        this.colors = colors;
        this.status = status;
        this.isPlayback = true;
        initField();
    }

    private void initResource() {
        player = world.addPlayer();
        world.addMonsters(2, 3);
        for (int i = 0; i < 10; i++) {
            world.addItem(new Item((char) 3, AsciiPanel.brightRed, "PotionOfHealth"), true);
        }
    }

    private void initField() {
        this.recording = false;
        this.recorder = null;
        this.rGlyphs = null;
        this.rColors = null;
        this.ss = null;
        this.frame = 0;
    }

    public void startRecording() {
        recording = true;
        recorder = new GameRecorder();
        recorder.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    private void stopRecording() {
        try {
            recorder.save();
        } catch (IOException e) {
            System.err.println("fail to save record.");
        }
        recording = false;
    }

    @Override
    public void displayOutput(AsciiPanel terminal) {
        if (isPlayback) {
            playback(terminal);
            return;
        }

        if (subscreen != null) {
            subscreen.displayOutput(terminal);
            return;
        }
        if (player.isDead()) {
            game.clearInputs();
            game.setScreen(new LoseScreen(game));
            return;
        }

        world.removeCreatures();
        if (world.monstersLeft() == 0) {
            game.clearInputs();
            game.setScreen(new WinScreen(game));
            return;
        }

        int left = getScrollX();
        int top = getScrollY();
        world.clearVisible();
        world.clearSight();
        // world.updateItems();
        // IFovAlgorithm a = new ShadowCasting();
        IFovAlgorithm a = new PrecisePermissive();
        a.visitFoV(world, player.getX(), player.getY(), world.getRadius());
        if (recording) {
            // recorder.startNewFrame();
            rGlyphs = new char[SCREEN_WIDTH][SCREEN_HEIGHT];
            rColors = new Color[SCREEN_WIDTH][SCREEN_HEIGHT];
            ss = new String[2];
        }
        displayTiles(terminal, left, top);
        displayCreaturesAndItems(terminal, left, top);
        displayStatus(terminal);
        if (recording) {
            recorder.addFrame(rGlyphs, rColors, ss);
        }
        // displayGameEvents(terminal);
    }

    @Override
    public Screen respondToUserInput(KeyEvent key) {
        if (isPlayback) {
            if (key.getKeyCode() == KeyEvent.VK_R) {
                return new MenuScreen(game);
            }
        } else {
            if (subscreen != null) {
                subscreen = subscreen.respondToUserInput(key);
            } else {
                switch (key.getKeyCode()) {
                    case KeyEvent.VK_LEFT:
                        player.move(world, -1, 0);
                        break;
                    case KeyEvent.VK_RIGHT:
                        player.move(world, 1, 0);
                        break;
                    case KeyEvent.VK_UP:
                        player.move(world, 0, -1);
                        break;
                    case KeyEvent.VK_DOWN:
                        player.move(world, 0, 1);
                        break;
                    case KeyEvent.VK_A:
                        player.randomAttack(world);
                        break;
                    case KeyEvent.VK_H:
                        subscreen = new SaveScreen(this);
                        break;
                    case KeyEvent.VK_S:
                        if (recording) {
                            stopRecording();
                            subscreen = new PromptScreen("Record saved successfully.", 1);
                        }
                        break;
                }
            }
        }
        // System.out.println(player.getX() + " " + player.getY());
        return this;
    }

    private void createWorld() {
        world = new WorldBuilder(WORLD_WIDTH, WORLD_HEIGHT).createCaves(10, 10, 3000).build();

    }

    public int getScrollX() {
        return Math.max(0, Math.min(player.getX() - SCREEN_WIDTH / 2, WORLD_WIDTH - SCREEN_WIDTH));
    }

    public int getScrollY() {
        return Math.max(0, Math.min(player.getY() - SCREEN_HEIGHT / 2, WORLD_HEIGHT - SCREEN_HEIGHT));
    }

    private void playback(AsciiPanel terminal) {
        if (frame < glyphs.size()) {
            char[][] currGlyphs = glyphs.get(frame);
            Color[][] currColors = colors.get(frame);
            String[] currStatus = status.get(frame);
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                for (int y = 0; y < SCREEN_HEIGHT; y++) {
                    // System.out.println(currColors[x][y]);
                    terminal.write(currGlyphs[x][y], x, y, currColors[x][y]);
                }
            }
            terminal.write(currStatus[0], 1, 1);
            terminal.write(currStatus[1], 1, 2);
            String s = "press [r] to return to menu";
            terminal.write(s, SCREEN_WIDTH - s.length(), 1);
            if (frame == glyphs.size() - 1) {
                terminal.write("Playback completes.", 1, 28);
            } else {
                frame++;
            }
        }
    }

    private void displayTiles(AsciiPanel terminal, int left, int top) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                int wx = x + left;
                int wy = y + top;
                if (world.isVisible(wx, wy)) {
                    Color tileColor = world.color(wx, wy);
                    if (world.isLead(wx, wy)) {
                        tileColor = AsciiPanel.brightGreen;
                    }
                    terminal.write(world.glyph(wx, wy), x, y, tileColor);
                    world.setWithinScreen(wx, wy);
                    if (recording) {
                        rGlyphs[x][y] = world.glyph(wx, wy);
                        rColors[x][y] = tileColor;
                    }
                } else {
                    terminal.write((char) 250, x, y, Color.DARK_GRAY);
                    if (recording) {
                        rGlyphs[x][y] = (char) 250;
                        rColors[x][y] = Color.DARK_GRAY;
                    }
                }
            }
        }
    }

    private void displayCreaturesAndItems(AsciiPanel terminal, int left, int top) {
        for (Item item : world.getItems()) {
            int x = item.getX();
            int y = item.getY();
            if (world.isVisible(x, y)) {
                int fx = x - left;
                int fy = y - top;
                if (fx >= 0 && fx < SCREEN_WIDTH && fy >= 0 && fy < SCREEN_HEIGHT) {
                    terminal.write(item.getGlyph(), fx, fy, item.getColor());
                    if (recording) {
                        rGlyphs[fx][fy] = item.getGlyph();
                        rColors[fx][fy] = item.getColor();
                    }
                }
            }
        }

        for (Creature creature : world.getCreatures()) {
            int x = creature.getX();
            int y = creature.getY();
            if (world.isVisible(x, y)) {
                int fx = x - left;
                int fy = y - top;
                if (fx >= 0 && fx < SCREEN_WIDTH && fy >= 0 && fy < SCREEN_HEIGHT) {
                    terminal.write(creature.getGlyph(), fx, fy, creature.getColor());
                    if (recording) {
                        rGlyphs[fx][fy] = creature.getGlyph();
                        rColors[fx][fy] = creature.getColor();
                    }
                }
            }
        }
    }

    private void displayStatus(AsciiPanel terminal) {
        String hpStatus = String.format("%d/%d hp", player.getHp(), player.getMaxHp());
        terminal.write(hpStatus, 1, 1);
        String monstersStatus = String.format("%d/%d monsters", world.monstersLeft(), world.monstersSum());
        terminal.write(monstersStatus, 1, 2);
        if (recording) {
            ss[0] = hpStatus;
            ss[1] = monstersStatus;
            String s = "press [s] to stop recording";
            terminal.write(s, SCREEN_WIDTH - s.length(), 1);
        }
    }

    public boolean saveMap() {
        MapSaver mapSaver = new MapSaver(world.getTiles());
        try {
            mapSaver.saveMap();
            return true;
        } catch (IOException e) {
            System.err.println("fail to save map");
        }
        return false;
    }

    public boolean saveGameProgress() {
        GameProgressSaver gpSaver = new GameProgressSaver(world);
        try {
            gpSaver.save();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            System.err.println("fail to save game progress");
        }
        return false;
    }

    // private void displayGameEvents(AsciiPanel terminal){
    // int top = SCREEN_HEIGHT - gameEvents.size();
    // for(int i = 0;i<gameEvents.size();i++){
    // terminal.writeCenter(gameEvents.get(i),top+i);
    // }
    // gameEvents.clear();
    // }
}