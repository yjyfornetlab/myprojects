package j05.world;

import org.junit.Test;

import j05.entity.Monster;
import j05.entity.Player;
import j05.entity.Tile;

import static org.junit.Assert.*;
public class CreatureFactoryTest {
    
    @Test
    public void testNewPlayer(){
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);

        CreatureFactory creatureFactory = new CreatureFactory(world);
        Player player = creatureFactory.newPlayer();
        assertTrue(world.getCreatures().contains(player));
    }

    @Test
    public void testNewMonster(){
        Tile[][] tiles = new Tile[20][30];
        for(int i=0;i<20;i++)
            for(int j=0;j<30;j++)
                tiles[i][j] = Tile.FLOOR;
        World world = new World(tiles);

        CreatureFactory creatureFactory = new CreatureFactory(world);
        Monster monster = creatureFactory.newMonster();
        assertTrue(world.getCreatures().contains(monster));
    }
}
