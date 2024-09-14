package j05.entity;

import java.awt.Color;
// import java.io.Serializable;

import asciiPanel.AsciiPanel;

public enum Tile {

    WALL((char)177, AsciiPanel.brightBlack),
    FLOOR((char)250, AsciiPanel.white);

    private char glyph;
    private Color color;
    
    Tile(char glyph, Color color) {
        this.glyph = glyph;
        this.color = color;
    }

    public char getGlyph(){
        return glyph;
    }

    public Color getColor(){
        return color;
    }

    public synchronized boolean blocked() {
        return this==Tile.WALL;
    }

    

}