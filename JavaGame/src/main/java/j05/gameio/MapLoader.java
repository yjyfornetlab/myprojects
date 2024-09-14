package j05.gameio;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import j05.entity.Tile;

public class MapLoader {

    private BufferedReader inputStream;

    public MapLoader() {
        inputStream = null;
    }

    public Tile[][] load() throws IOException {
        Tile[][] map = null;
        try {
            inputStream = new BufferedReader(new FileReader("map.txt"));
            String s = inputStream.readLine();
            int width = 0;
            int height = 0;
            if (s != null) {
                String[] mapSize = s.split(" ");
                width = Integer.parseInt(mapSize[0]);
                height = Integer.parseInt(mapSize[1]);
                map = new Tile[width][height];
            }
            if (map != null) {
                for (int x = 0; x < width; x++) {
                    s = inputStream.readLine();
                    if (s != null) {
                        String[] types = s.split(" ");
                        for (int y = 0; y < height; y++) {
                            map[x][y] = Integer.parseInt(types[y]) == 0 ? Tile.WALL : Tile.FLOOR;
                        }
                    }
                }
            }
        } finally {
            if (inputStream != null) {
                inputStream.close();
            }
        }
        return map;
    }
}
