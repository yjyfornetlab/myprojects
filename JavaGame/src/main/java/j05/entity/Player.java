package j05.entity;

import java.awt.Color;
import java.util.List;

import j05.world.World;
import rlforj.math.Point;
import rlforj.pathfinding.AStar;

public class Player extends Creature {

    public Player(char glyph, Color color, int maxHp, int attackValue, int defenseValue, int attackRange) {
        super(glyph, color, maxHp, attackValue, defenseValue, attackRange);
    }

    public boolean canSee(World world, Creature creature) {
        return world.isVisible(creature.x, creature.y) && world.inSight(creature.x, creature.y);
    }

    public void randomAttack(World world) {
        for (Creature creature : world.getCreatures()) {
            if ((creature instanceof Monster) && canSee(world, creature) && !outOfRange(creature)) {
                attack(creature);
                break;
            }
        }
    }

    @Override
    public void move(World world, int dx, int dy) {
        if (world.notBlockedThenOccupy(this, x + dx, y + dy)) {
            Item item = world.item(x, y);
            if (item != null) {
                if (item.getType() == "PotionOfHealth") {
                    if (hp + 5 > maxHp) {
                        hp = maxHp;
                    } else {
                        hp = hp + 5;
                    }
                }
                world.removeItem(item);
            }

            Creature target = null;
            world.clearLead();
            for (Creature creature : world.getCreatures()) {
                if ((creature instanceof Monster) && (!canSee(world, creature))) {
                    target = creature;
                }
            }

            if (target != null) {
                AStar a = new AStar(world, world.getWidth(), world.getHeight(), false);
                Point[] path = a.findPath(this.x, this.y, target.x, target.y, 30);
                if (path != null && path.length >= 6) {
                    for (int i = 1; i <= 4; i++) {
                        world.setLead(path[i].x, path[i].y);
                    }
                }
            }
        }
    }

    // @Override
    // public void addAttackEvent(Creature other,int harm){
    // String event = String.format("You attack '%s' at (%d,%d) for %d damage.",
    // other.glyph, other.x, other.y, harm);
    // events.add(event);
    // }
}