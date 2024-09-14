package j05.world;

import java.util.List;
import java.util.Random;

import j05.entity.Tile;

public class WorldBuilder {

    private int width;
    private int height;
    private Tile[][] tiles;

    public WorldBuilder(int width, int height) {
        this.width = width;
        this.height = height;
        tiles = new Tile[width][height];
    }

    public World build() {
        return new World(tiles);
    }

    public WorldBuilder createCaves(int startX, int startY, int steps) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
                tiles[x][y] = Tile.WALL;

        Random random = new Random();
        int direction;
        int x = startX;
        int y = startY;

        int i = 0;
        while (i < steps) {
            direction = random.nextInt(4);
            if ((direction == 0) && (x + 1) < (width - 1))
                x += 1;
            else if (direction == 1 && (x - 1) > 0)
                x -= 1;
            else if (direction == 2 && (y + 1) < (height - 1))
                y += 1;
            else if (direction == 3 && (y - 1) > 0)
                y -= 1;
            if (tiles[x][y] != Tile.FLOOR) {
                tiles[x][y] = Tile.FLOOR;
                i++;
            }
            // System.out.println(1);
        }
        return this;
    }

}