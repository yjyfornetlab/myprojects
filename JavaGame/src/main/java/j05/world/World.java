package j05.world;

import j05.entity.Tile;
import j05.entity.Creature;
import j05.entity.Item;
import j05.entity.Monster;
import j05.entity.Player;

import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.awt.Color;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import rlforj.IBoard;

public class World implements IBoard,Serializable {
    private static final long serialVersionUID = 6529685098267757690L;

    private int width;
    private int height;
    private Tile[][] tiles;
    private HashSet<Creature> creatures;
    private HashSet<Item> items;
    private boolean[][] visible;
    private boolean[][] sight;
    private boolean[][] lead;
    private CreatureFactory factory;
    private Player player;
    private int visibleRadius;
    private transient ExecutorService exec;
    private int numberOfMonsters;
    // private int leftMonsters;
    private int numberOfPlayers;

    public World(Tile[][] tiles) {
        this.width = tiles.length;
        this.height = tiles[0].length;
        this.tiles = tiles;
        this.creatures = new HashSet<>();
        this.items = new HashSet<>();
        this.factory = new CreatureFactory(this);
        this.visibleRadius = 15;
        this.exec = Executors.newCachedThreadPool();
        this.numberOfMonsters = 0;
        // this.leftMonsters = 0;
        this.numberOfPlayers = 0;

        visible = new boolean[width][height];
        sight = new boolean[width][height];
        lead = new boolean[width][height];
        clearVisible();
        clearSight();
        clearLead();
    }

    // public void addCreature(Creature creature, int x, int y){
    // creatures.add(creature);
    // }

    public Tile[][] getTiles(){
        return tiles;
    }

    public char glyph(int x, int y) {
        return tiles[x][y].getGlyph();
    }

    public Color color(int x, int y) {
        return tiles[x][y].getColor();
    }

    public HashSet<Creature> getCreatures() {
        return creatures;
    }

    public HashSet<Item> getItems(){
        return items;
    }

    public synchronized Creature creature(int x, int y) {
        for (Creature c : creatures)
            if (c.getX() == x && c.getY() == y)
                return c;
        return null;
    }

    public synchronized Tile tile(int x, int y) {
        if (x > 0 && x < width - 1 && y > 0 && y < height - 1)
            return tiles[x][y];
        return Tile.WALL;
    }

    public synchronized boolean notBlockedThenOccupy(Creature creature, int x, int y) {
        if (!locationBlocked(x, y)) {
            creature.setX(x);
            creature.setY(y);
            return true;
        }
        return false;
    }

    public Item item(int x,int y){
        for (Item i : items)
            if (i.getX() == x && i.getY() == y)
                return i;
        return null;
    }

    public synchronized boolean locationBlocked(int x, int y) {
        return tile(x, y).blocked() || (creature(x, y) != null);
    }

    public void addAtEmptyLocation(Creature creature) {
        int x;
        int y;
        do {
            x = (int) (Math.random() * width);
            y = (int) (Math.random() * height);
        } while (locationBlocked(x, y));

        creature.setX(x);
        creature.setY(y);

        creatures.add(creature);
    }

    public void addItem(Item item, boolean setPos) {
        if (setPos) {
            int x;
            int y;
            do {
                x = (int) (Math.random() * width);
                y = (int) (Math.random() * height);
            } while (locationBlocked(x, y) || item(x,y)!=null);

            item.setX(x);
            item.setY(y);
        }
        items.add(item);
    }

    public Player addPlayer() {
        player = factory.newPlayer();
        numberOfPlayers++;
        return player;
    }

    public Player getPlayer() {
        return player;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public int getRadius() {
        return visibleRadius;
    }

    public void addMonsters(int numM, int numZ) {
        numberOfMonsters = numM + numZ;
        // leftMonsters = numM+numZ;
        for (int i = 0; i < numM; i++) {
            exec.execute(new CreatureController(factory.newMonster(), this));
        }
        for (int i = 0; i < numZ; i++) {
            exec.execute(new CreatureController(factory.newZombie(), this));
        }
        exec.shutdown();
    }

    public void setMonsters(){
        exec = Executors.newCachedThreadPool();
        for(Creature c : creatures){
            if(c instanceof Monster){
                exec.execute(new CreatureController(c, this));
            }
        }
        exec.shutdown();
    }

    public int monstersSum() {
        return numberOfMonsters;
    }

    public void removeCreatures() {
        creatures.removeIf(creature -> creature.isDead());
    }

    public void removeItem(Item item){
        items.remove(item);
    }

    public int monstersLeft() {
        return creatures.size() - numberOfPlayers;
    }

    // public void updateItems(){
    //     List<Item> toUpdate = new ArrayList<>(this.items);
    //     for(Item item : toUpdate){
    //         item.update(this);
    //     }
    // }

    @Override
    public boolean contains(int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    @Override
    public boolean blocksLight(int x, int y) {
        return tile(x, y).blocked();
    }

    @Override
    public boolean blocksStep(int x, int y) {
        return locationBlocked(x, y);
    }

    @Override
    public void visit(int x, int y) {
        visible[x][y] = true;
    }

    public void setWithinScreen(int x, int y) {
        sight[x][y] = true;
    }

    public void setLead(int x,int y){
        lead[x][y] = true;
    }

    public void clearVisible() {
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                visible[i][j] = false;
    }

    public void clearSight() {
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                sight[i][j] = false;
    }

    public void clearLead(){
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                lead[i][j] = false;
    }

    public boolean isVisible(int x, int y) {
        return visible[x][y];
    }

    public boolean inSight(int x, int y) {
        return sight[x][y];
    }

    public boolean isLead(int x,int y){
        return lead[x][y];
    }
}