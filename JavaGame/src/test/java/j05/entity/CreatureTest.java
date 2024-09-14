package j05.entity;

import org.junit.Test;
import static org.junit.Assert.*;

import asciiPanel.AsciiPanel;

public class CreatureTest {

    @Test
    public void testGetMaxHp() {
        Creature creature = new Creature('z', AsciiPanel.green, 30, 10, 5, 2);
        assertEquals(30, creature.getMaxHp());
    }

    @Test
    public void testBeAttackedBy() {
        Creature creature = new Creature('z', AsciiPanel.green, 30, 10, 1, 2);
        Creature attacker = new Creature('@', AsciiPanel.brightWhite, 20, 5, 3, 4);
        creature.beAttackedBy(attacker, 4);
        assertEquals(26, creature.getHp());
    }

    @Test
    public void testAttack() {
        Creature creature = new Creature('z', AsciiPanel.green, 10, 10, 1, 2);
        Creature attacker = new Creature('@', AsciiPanel.brightWhite, 20, 5, 3, 4);
        attacker.attack(creature);
        assertTrue((creature.getHp() >= 5) && (creature.getHp() <= 9));
    }

    @Test
    public void testIsDead() {
        Creature creature = new Creature('z', AsciiPanel.green, 5, 10, 1, 2);
        Creature attacker = new Creature('@', AsciiPanel.brightWhite, 20, 5, 3, 4);
        creature.beAttackedBy(attacker, 4);
        assertFalse(creature.isDead());
        creature.beAttackedBy(attacker, 3);
        assertTrue(creature.isDead());
    }

    @Test
    public void testGetColor() {
        Creature creature = new Creature('z', AsciiPanel.green, 30, 10, 1, 2);
        Creature attacker = new Creature('@', AsciiPanel.brightWhite, 20, 5, 3, 4);
        assertEquals(AsciiPanel.green, creature.getColor());
        creature.beAttackedBy(attacker, 3);
        assertEquals(AsciiPanel.red, creature.getColor());
    }

    @Test
    public void testOutOfRange(){
        Creature z = new Creature('z', AsciiPanel.green, 50, 3, 4, 1);
        Creature m = new Creature('m', AsciiPanel.red, 10, 3, 7, 4);
        z.setX(4);
        z.setY(7);
        m.setX(3);
        m.setY(9);
        assertTrue(z.outOfRange(m));
        assertFalse(m.outOfRange(z));
    }
}
