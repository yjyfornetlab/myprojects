package j05.gameio;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

import j05.world.World;

public class GameProgressSaver {

    private ObjectOutputStream oos;
    private World world;

    public GameProgressSaver(World world) {
        this.world = world;
        this.oos = null;
    }

    public void save() throws IOException {
        try {
            oos = new ObjectOutputStream(new FileOutputStream("gameprogress.txt"));
            oos.writeObject(world);
            oos.flush();
        } finally {
            if (oos != null) {
                oos.close();
            }
        }
    }
}
