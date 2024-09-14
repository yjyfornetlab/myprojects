package j05.entity;

import org.junit.Test;
import static org.junit.Assert.*;

import asciiPanel.AsciiPanel;

public class EntityTest {

    @Test
    public void testGetX() {
        Entity e = new Entity(12, 31, 'm', AsciiPanel.green);
        assertEquals(12, e.getX());
    }

    @Test
    public void testGetY() {
        Entity e = new Entity(12, 31, 'm', AsciiPanel.green);
        assertEquals(31, e.getY());
    }

    @Test
    public void testGetGlyph() {
        Entity e = new Entity(12, 31, 'm', AsciiPanel.green);
        assertEquals('m', e.getGlyph());
    }

    @Test
    public void testGetColor() {
        Entity e = new Entity(12, 31, 'm', AsciiPanel.green);
        assertEquals(AsciiPanel.green, e.getColor());
    }

    @Test
    public void testSetX() {
        Entity e = new Entity('z', AsciiPanel.red);
        e.setX(5);
        assertEquals(5, e.getX());
    }

    @Test
    public void testSetY() {
        Entity e = new Entity('z', AsciiPanel.red);
        e.setY(62);
        assertEquals(62, e.getY());
    }

}
