package j05.entity;

import java.awt.Color;

import j05.world.World;

public class Item extends Entity {
    private String type;
    // private int spreadcount;

    public Item(char glyph,Color color,String type){
        super(glyph,color);
        this.type = type;
    }
    public Item(int x, int y, char glyph, Color color, String type) {
        super(x, y, glyph, color);
        this.type = type;
        // this.spreadcount = 0;
    }

    // public void update(World world) {
    //     if (type == "PotionOfHealth") {
    //         if (spreadcount < 5 && Math.random() < 0.02) {
    //             spread(world);
    //         }
    //     }
    // }

    public String getType(){
        return type;
    }

    // private void spread(World world) {
    //     int nx = x + (int) (Math.random() * 11) - 5;
    //     int ny = y + (int) (Math.random() * 11) - 5;

    //     if ((!world.locationBlocked(nx, ny)) && (world.item(nx, ny)==null)) {
    //         Item child = new Item(nx, ny, glyph, color, type);
    //         world.addItem(child, false);
    //         spreadcount++;
    //     }
    // }
}