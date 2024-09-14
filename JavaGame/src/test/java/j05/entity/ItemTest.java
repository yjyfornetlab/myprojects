package j05.entity;

import org.junit.Test;

import asciiPanel.AsciiPanel;

import static org.junit.Assert.*;

public class ItemTest {
    
    @Test
    public void testGetType(){
        Item item = new Item((char)7, AsciiPanel.black, "stone");
        assertEquals("stone",item.getType());
    }
}
