package j05.entity;

import org.junit.Test;

import asciiPanel.AsciiPanel;
import j05.world.World;

import static org.junit.Assert.*;

public class PlayerTest {
    
    @Test
    public void testCanSee(){
        Creature z = new Creature('z', AsciiPanel.green, 50, 3, 4, 1);
        Creature m = new Creature('m', AsciiPanel.red, 10, 3, 7, 4);
        Player p = new Player('@', AsciiPanel.green, 20, 5, 3, 1);
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);
        z.setX(17);
        z.setY(25);

        m.setX(3);
        m.setY(9);

        world.visit(17,25);
        world.setWithinScreen(17, 25);

        assertTrue(p.canSee(world, z));
        assertFalse(p.canSee(world,m));
    }
}
