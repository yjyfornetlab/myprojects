package j05.gameio;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;

import j05.world.World;

public class GameProgressLoader {

    private ObjectInputStream ois;

    public GameProgressLoader() {
        ois = null;
    }

    public World load() throws IOException, ClassNotFoundException {
        World world = null;
        try {
            ois = new ObjectInputStream(new FileInputStream("gameprogress.txt"));
            world = (World) ois.readObject();
        } finally {
            if (ois != null) {
                ois.close();
            }
        }
        return world;
    }
}
