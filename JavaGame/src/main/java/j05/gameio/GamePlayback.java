package j05.gameio;

import java.awt.Color;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import asciiPanel.AsciiPanel;

public class GamePlayback {
    
    private BufferedReader playback;
    private List<char[][]> glyphs;
    private List<Color[][]> colors;
    private List<String[]> status;

    public GamePlayback(){
        playback = null;
        glyphs = new ArrayList<>();
        colors = new ArrayList<>();
        status = new ArrayList<>();
    }

    public List<char[][]> getGlyphs(){
        return glyphs;
    }

    public List<Color[][]> getColors(){
        return colors;
    }

    public List<String[]> getStatuss(){
        return status;
    }

    public void load() throws IOException{
        try{
            playback = new BufferedReader(new FileReader("record.txt"));
            String s = playback.readLine();
            // System.out.println(s);
            int width = 0;
            int height = 0;
            if(s!=null){
                String[] mapSize = s.split(" ");
                width = Integer.parseInt(mapSize[0]);
                height = Integer.parseInt(mapSize[1]);
            }
            // s = playback.readLine();
            // System.out.print(s.equals("frame"));
            while((s=playback.readLine())!=null){
                // System.out.println(s);
                if(s.equals("frame")){
                    s=playback.readLine();
                    // System.out.println(s);
                    if(s.equals("glyphs")){
                        char[][] glyph = new char[width][height];
                        for(int x=0;x<width;x++){
                            s = playback.readLine();
                            if(s!=null){
                                String[] types = s.split(" ");
                                for(int y=0;y<height;y++){
                                    glyph[x][y] = getGlyph(Integer.parseInt(types[y]));
                                }
                            }
                        }
                        glyphs.add(glyph);
                    }
                    s = playback.readLine();
                    if(s.equals("colors")){
                        // System.out.println(1);
                        Color[][] color = new Color[width][height];
                        for(int x=0;x<width;x++){
                            s = playback.readLine();
                            if(s!=null){
                                String[] types = s.split(" ");
                                for(int y=0;y<height;y++){
                                    color[x][y] = getColor(Integer.parseInt(types[y]));
                                }
                            }
                        }
                        colors.add(color);
                    }
                    s = playback.readLine();
                    if(s.equals("status")){
                        String[] ss = new String[2];
                        ss[0] = playback.readLine();
                        ss[1] = playback.readLine();
                        status.add(ss);
                    }
                }
            }
            // System.out.println(1);

        }finally{
            if(playback!=null){
                playback.close();
            }
        }
    }

    private char getGlyph(int no){
        if(no==0){
            return (char)177;
        }
        if(no==1){
            return (char)250;
        }
        if(no==2){
        return '@';
        }
        if(no==3){
            return 'm';
        }
        if(no==4){
            return 'z';
        }
        if(no==5){
            return (char)3;
        }
        return (char)250;
    }

    private Color getColor(int no){
        if (no == 0) {
            return AsciiPanel.brightBlack;
        }
        if (no == 1) {
            return AsciiPanel.white;
        }
        if (no == 2) {
            return AsciiPanel.brightWhite;
        }
        if (no == 3) {
            return AsciiPanel.brightYellow;
        }
        if (no == 4) {
            return AsciiPanel.green;
        }
        if (no == 5) {
            return AsciiPanel.red;
        }
        if (no == 6) {
            return AsciiPanel.brightRed;
        }
        if (no == 7) {
            return AsciiPanel.brightGreen;
        }
        if (no == 8) {
            return Color.DARK_GRAY;
        }
        return Color.DARK_GRAY;
    }
}
