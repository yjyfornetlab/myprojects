package j05.entity;

import java.awt.Color;
import java.io.Serializable;

public class Entity implements Serializable{
    private static final long serialVersionUID = 6529685098267757690L;

    protected int x;
    protected int y;
    protected char glyph;
    protected Color color;

    public Entity(char glyph, Color color){
        this.glyph = glyph;
        this.color = color;
    }
    
    public Entity(int x,int y,char glyph,Color color){
        this.x = x;
        this.y = y;
        this.glyph = glyph;
        this.color = color;
    }

    public int getX(){
        return x;
    }
    
    public int getY(){
        return y;
    }

    public void setX(int x){
        this.x = x;
    }

    public void setY(int y){
        this.y = y;
    }

    public char getGlyph(){
        return glyph;
    }

    public Color getColor(){
        return color;
    }
}