package j05.entity;

import java.awt.Color;
import java.util.List;

import asciiPanel.AsciiPanel;
import j05.world.World;
import rlforj.math.Point;

public class Creature extends Entity {

    protected int maxHp;
    protected int attackValue;
    protected int defenseValue;
    protected int hp;
    protected int attackRange;
    // protected int speed;
    // protected List<String> events;
    private boolean[] beingAttacked;
    private int attackDuration;

    public Creature(char glyph, Color color, int maxHp, int attackValue, int defenseValue, int attackRange) {
        super(glyph, color);
        this.maxHp = maxHp;
        this.hp = maxHp;
        this.attackValue = attackValue;
        this.defenseValue = defenseValue;
        this.attackRange = attackRange;
        // this.speed = 0;
        // this.events = events;
        this.attackDuration = 6;
        this.beingAttacked = new boolean[attackDuration];
        // this.prevBeAttacked = false;
        for (int i = 0; i < attackDuration; i++)
            beingAttacked[i] = false;
    }

    public void attack(Creature other) {
        int amount = (int) (Math.random() * Math.max(0, attackValue - other.defenseValue)) + 1;
        other.beAttackedBy(this, amount);
        // addAttackEvent(other, amount);
    }

    public synchronized void beAttackedBy(Creature attacker, int harm) {
        if (hp <= harm) {
            hp = 0;
        } else {
            hp = hp - harm;
        }
        for (int i = 0; i < attackDuration; i++)
            beingAttacked[i] = true;
        // prevBeAttacked = true;
    }

    public void move(World world, int dx, int dy) {
        // System.out.println("before:"+" "+this.x+" "+this.y);
        world.notBlockedThenOccupy(this, x + dx, y + dy);
        // System.out.println("after:"+" "+this.x+" "+this.y);
    }

    public void moveTo(World world, int desX, int desY) {
        move(world, desX - x, desY - y);
    }

    public boolean outOfRange(Creature other) {
        if (new Point(x, y).distance2(new Point(other.x, other.y)) > attackRange) {
            return true;
        }
        return false;
    }

    // public void addAttackEvent(Creature other,int harm){
    // String event = String.format("Creature at (%d,%d) attacks creature at (%d,%d)
    // for %d damage.",this.x,this.y,other.x,other.y,harm);
    // events.add(event);
    // }

    public boolean isDead() {
        return hp <= 0;
    }

    public int getHp() {
        return hp;
    }

    public int getMaxHp() {
        return maxHp;
    }

    @Override
    public Color getColor() {
        for (int i = 0; i < attackDuration; i++) {
            if (beingAttacked[i]) {
                beingAttacked[i] = false;
                return AsciiPanel.red;
            }
        }
        return color;
    }

}