package j05.world;

import java.io.Serializable;
import java.util.List;

import asciiPanel.AsciiPanel;
import j05.entity.Monster;
import j05.entity.Player;

public class CreatureFactory implements Serializable{
    private static final long serialVersionUID = 6529685098267757690L;

    private World world;
    public CreatureFactory(World world){
        this.world = world;
    }

    public Player newPlayer(){
        Player player = new Player('@',AsciiPanel.brightWhite,50,6,2,3);
        world.addAtEmptyLocation(player);
        return player;
    }

    public Monster newMonster(){
        Monster monster = new Monster('m',AsciiPanel.brightYellow,20,5,1,1,"ChasingMonster");
        world.addAtEmptyLocation(monster);
        return monster;
    }

    public Monster newZombie(){
        Monster zombie = new Monster('z', AsciiPanel.green, 10, 7, 4, 4, "Zombie");
        world.addAtEmptyLocation(zombie);
        return zombie;
    }
}