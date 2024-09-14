package j05.screen;

import java.awt.event.KeyEvent;
import java.io.IOException;

import asciiPanel.AsciiPanel;
import j05.Roguelike;
import j05.gameio.GamePlayback;
import j05.gameio.GameProgressLoader;
import j05.gameio.MapLoader;
import j05.entity.Tile;
import j05.world.World;

public class MenuScreen implements Screen {

    private Roguelike game;

    public MenuScreen(Roguelike game) {
        this.game = game;
    }

    @Override
    public void displayOutput(AsciiPanel terminal) {
        int top = (SCREEN_HEIGHT - 10) / 2;
        terminal.writeCenter("-- press [1] to load previous map --", top,AsciiPanel.brightWhite);
        terminal.writeCenter("-- press [2] to load previous progress --", top + 3,AsciiPanel.brightWhite);
        terminal.writeCenter("-- press [3] to play back game record --", top + 6,AsciiPanel.brightWhite);
        terminal.writeCenter("-- press [4] to start a new game --", top + 9,AsciiPanel.brightWhite);
    }

    @Override
    public Screen respondToUserInput(KeyEvent key) {
        switch (key.getKeyCode()) {
            case KeyEvent.VK_1:
                Tile[][] map = loadMap();
                if (map != null) {
                    return new GameScreen(game, map);
                }
                break;
            case KeyEvent.VK_2:
                World world = loadGameProgress();
                if (world != null) {
                    return new GameScreen(game, world);
                }
                break;
            case KeyEvent.VK_3:
                GamePlayback gpback = new GamePlayback();
                if (loadPlayback(gpback)) {
                    return new GameScreen(game, gpback.getGlyphs(), gpback.getColors(), gpback.getStatuss());
                }
            case KeyEvent.VK_4:
                return new GameScreen(game);
        }
        return this;
    }

    private Tile[][] loadMap() {
        try {
            return new MapLoader().load();
        } catch (IOException e) {
            System.err.println("fail to load map");
        }
        return null;
    }

    private World loadGameProgress() {
        try {
            return new GameProgressLoader().load();
        } catch (IOException e) {
            e.printStackTrace();
            System.err.println("fail to load game progress");
        } catch (ClassNotFoundException e) {
            System.err.println("class is not found");
        }
        return null;
    }

    private boolean loadPlayback(GamePlayback gpback) {
        try {
            gpback.load();
            return true;
        } catch (IOException e) {
            System.err.println("fail to load playback");
        }
        return false;
    }
}
