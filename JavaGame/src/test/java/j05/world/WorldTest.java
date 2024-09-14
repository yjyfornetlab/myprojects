package j05.world;

import org.junit.Test;

import asciiPanel.AsciiPanel;
import j05.entity.Creature;
import j05.entity.Item;
import j05.entity.Tile;

import static org.junit.Assert.*;

public class WorldTest {
    
    @Test
    public void testCreature(){
        Creature z = new Creature('z', AsciiPanel.green, 50, 3, 4, 1);
        Creature m = new Creature('m', AsciiPanel.red, 10, 3, 7, 4);
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);
        world.getCreatures().add(z);
        world.getCreatures().add(m);
        z.setX(17);
        z.setY(25);
        m.setX(3);
        m.setY(9);
        assertSame(z, world.creature(17, 25));
        assertSame(m,world.creature(3, 9));
        assertNull(world.creature(6, 10));
    }

    @Test
    public void testTile(){
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        tiles[8][13] = Tile.WALL;
        World world = new World(tiles);

        assertEquals(Tile.FLOOR,world.tile(11,23));
        assertEquals(Tile.WALL,world.tile(8, 13));
        assertEquals(Tile.WALL,world.tile(19,17));
        assertEquals(Tile.WALL,world.tile(-1,16));
        assertEquals(Tile.WALL,world.tile(9,33));
        assertEquals(Tile.WALL,world.tile(12,0));

    }

    @Test
    public void testGetWidth(){
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);
        assertEquals(20,world.getWidth());
    }

    @Test
    public void testGetHeight(){
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);
        assertEquals(30,world.getHeight());
    }

    @Test
    public void testRemoveCreatures(){
        Creature z = new Creature('z', AsciiPanel.green, 2, 3, 4, 1);
        Creature m = new Creature('m', AsciiPanel.red, 10, 9, 7, 4);
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);
        world.getCreatures().add(z);
        world.getCreatures().add(m);
        z.setX(13);
        z.setY(21);
        m.setX(7);
        m.setY(10);

        z.beAttackedBy(m, 2);
        assertEquals(z,world.creature(13,21));
        assertEquals(m,world.creature(7, 10));
        assertEquals(2,world.getCreatures().size());
        world.removeCreatures();
        assertNull(world.creature(13,21));
        assertEquals(m,world.creature(7, 10));
        assertEquals(1,world.getCreatures().size());
    }

    @Test
    public void testContains(){
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);

        assertTrue(world.contains(17, 29));
        assertFalse(world.contains(-1, 2));
        assertFalse(world.contains(3,-2));
        assertFalse(world.contains(20,17));
        assertFalse(world.contains(12,36));
    }

    @Test
    public void testLocationBlocked(){
        Creature z = new Creature('z', AsciiPanel.green, 50, 3, 4, 1);
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        tiles[11][17] = Tile.WALL;
        World world = new World(tiles);
        world.getCreatures().add(z);
        z.setX(7);
        z.setY(26);

        assertTrue(world.locationBlocked(11, 17));
        assertTrue(world.locationBlocked(7, 26));
        assertFalse(world.locationBlocked(15, 8));
    }

    @Test
    public void testClearVisible(){
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);

        world.clearVisible();
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                assertFalse(world.isVisible(i,j));
        world.visit(11,9);

        for(int i=0;i<20;i++){
            for(int j=0;j<30;j++){
                if(i==11 && j==9){
                    assertTrue(world.isVisible(i, j));
                }
                else{
                    assertFalse(world.isVisible(i,j));
                }
            }
        }

        world.clearVisible();
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                assertFalse(world.isVisible(i,j));
    }

    @Test
    public void testAddItem(){
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);

        Item item = new Item((char)7, AsciiPanel.black, "stone");
        world.addItem(item, true);
        assertTrue(world.getItems().contains(item));
    }
}
