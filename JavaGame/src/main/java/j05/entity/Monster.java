package j05.entity;

import java.awt.Color;
import java.util.List;

import j05.world.World;
import rlforj.math.Point;
import rlforj.pathfinding.AStar;

public class Monster extends Creature {

    private String type;
    private int speed;

    public Monster(char glyph, Color color, int maxHp, int attackValue, int defenseValue, int attackRange,
            String type) {
        super(glyph, color, maxHp, attackValue, defenseValue, attackRange);
        this.speed = 2;
        this.type = type;
    }

    public int getSpeed() {
        return speed;
    }

    public void chase(World world, Creature other) {
        // System.out.println(this.x+" "+this.y);
        if (type == "ChasingMonster") {
            // System.out.println(1);
            AStar a = new AStar(world, world.getWidth(), world.getHeight(), true);
            Point[] path = a.findPath(this.x, this.y, other.x, other.y, world.getRadius());
            // System.out.println(this.x+" "+this.y);
            if (path != null && path.length > 1) {
                // System.out.println(path[0].x+" "+path[0].y);
                moveTo(world, path[1].x, path[1].y);
            }
        }
    }

    // @Override
    // public void addAttackEvent(Creature other,int harm){
    //     String event = String.format("Monster '%s' attacks you for %d damage", this.glyph, harm);
    //     events.add(event);
    // }
}