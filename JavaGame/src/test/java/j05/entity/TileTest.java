package j05.entity;

import org.junit.Test;

import asciiPanel.AsciiPanel;

import static org.junit.Assert.*;

public class TileTest {

    @Test
    public void testGetGlyph() {
        Tile wall = Tile.WALL;
        Tile floor = Tile.FLOOR;
        assertEquals((char) 177, wall.getGlyph());
        assertEquals((char) 250, floor.getGlyph());
    }

    @Test
    public void testGetColor() {
        Tile wall = Tile.WALL;
        Tile floor = Tile.FLOOR;
        assertEquals(AsciiPanel.brightBlack, wall.getColor());
        assertEquals(AsciiPanel.white, floor.getColor());
    }

    @Test
    public void testBlocked() {
        Tile wall = Tile.WALL;
        Tile floor = Tile.FLOOR;
        assertTrue(wall.blocked());
        assertFalse(floor.blocked());
    }
}
