package j05.world;

import java.util.concurrent.TimeUnit;

import j05.entity.Creature;
import j05.entity.Monster;

public class CreatureController implements Runnable {
    private Creature creature;
    private World world;

    public CreatureController(Creature creature, World world) {
        this.creature = creature;
        this.world = world;
    }

    public void run() {
        if (creature instanceof Monster) {
            while (!creature.isDead()) {
                long startTime = System.currentTimeMillis();
                if (world.getPlayer().canSee(world, creature)) {
                    if (creature.outOfRange(world.getPlayer())) {
                        ((Monster) creature).chase(world, world.getPlayer());
                    }
                    else{
                        creature.attack(world.getPlayer());
                    }
                }
                long endTime = System.currentTimeMillis();
                try {
                    TimeUnit.MILLISECONDS
                            .sleep((1000 / ((Monster) creature).getSpeed()) - (endTime - startTime));
                } catch (InterruptedException e) {
                    System.err.println("Interrupted");
                }
            }
        }
    }
}