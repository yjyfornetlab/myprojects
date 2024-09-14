package j05.gameio;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import j05.entity.Tile;

public class MapSaver {

    private Tile[][] map;
    private BufferedWriter outputStream;
    private int width;
    private int height;

    public MapSaver(Tile[][] map) {
        this.map = map;
        this.outputStream = null;
        this.width = map.length;
        this.height = map[0].length;
    }

    public void saveMap() throws IOException {
        try {
            // outputStream = new BufferedWriter(new FileWriter("resources"+File.separator+"map.txt"));
            outputStream = new BufferedWriter(new FileWriter("map.txt"));
            StringBuilder sbuilder = new StringBuilder();
            String s = width + " " + height + " " + "\n";
            sbuilder.append(s);
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    if (map[x][y] == Tile.FLOOR) {
                        // s = s + 1 + " ";
                        sbuilder.append(1).append(" ");
                    } else {
                        // s = s + 0 + " ";
                        sbuilder.append(0).append(" ");
                    }
                }
                // s = s + "\n";
                sbuilder.append("\n");
            }
            outputStream.write(sbuilder.toString());
        } finally {
            if (outputStream != null) {
                outputStream.close();
            }
        }
    }
}
