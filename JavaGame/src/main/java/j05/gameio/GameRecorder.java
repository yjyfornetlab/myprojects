package j05.gameio;

import java.awt.Color;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import asciiPanel.AsciiPanel;

public class GameRecorder {

    private StringBuilder frames;
    private BufferedWriter recorder;
    private List<char[][]> glyphs;
    private List<Color[][]> colors;
    private List<String[]> status;

    public GameRecorder() {
        frames = new StringBuilder();
        recorder = null;
        glyphs = new ArrayList<>();
        colors = new ArrayList<>();
        status = new ArrayList<>();
    }

    public void addFrame(char[][] glyph, Color[][] color, String[] st) {
        glyphs.add(glyph);
        colors.add(color);
        status.add(st);
    }

    public void setSize(int width, int height) {
        String s = width + " " + height + "\n";
        frames.append(s);
    }

    private void startNewFrame() {
        // frames = frames + "frame\n";
        frames.append("frame\n");
    }

    private void setGlyphs(char[][] glyphs) {
        // frames = frames + "glyphs\n";
        frames.append("glyphs\n");
        for (int x = 0; x < glyphs.length; x++) {
            for (int y = 0; y < glyphs[0].length; y++) {
                // frames = frames + getGlyphNumber(glyphs[x][y]) + " ";
                frames.append(getGlyphNumber(glyphs[x][y])).append(" ");
            }
            // frames = frames + "\n";
            frames.append("\n");
        }
    }

    private void setColors(Color[][] colors) {
        // frames = frames + "colors\n";
        frames.append("colors\n");
        for (int x = 0; x < colors.length; x++) {
            for (int y = 0; y < colors[0].length; y++) {
                // frames = frames+getColorNumber(colors[x][y])+" ";
                frames.append(getColorNumber(colors[x][y])).append(" ");
            }
            // frames = frames+"\n";
            frames.append("\n");
        }
    }

    private void setStatus(String[] status) {
        // frames = frames+"status\n";
        frames.append("status\n");
        for (int i = 0; i < status.length; i++) {
            // frames = frames+status[i]+"\n";
            frames.append(status[i]).append("\n");
        }
    }

    public void save() throws IOException {
        for (int i = 0; i < glyphs.size(); i++) {
            startNewFrame();
            setGlyphs(glyphs.get(i));
            setColors(colors.get(i));
            setStatus(status.get(i));
        }
        try {
            recorder = new BufferedWriter(new FileWriter("record.txt"));
            recorder.write(frames.toString());
        } finally {
            if (recorder != null) {
                recorder.close();
            }
        }
    }

    private int getColorNumber(Color color) {
        if (color == AsciiPanel.brightBlack) {
            return 0;
        }
        if (color == AsciiPanel.white) {
            return 1;
        }
        if (color == AsciiPanel.brightWhite) {
            return 2;
        }
        if (color == AsciiPanel.brightYellow) {
            return 3;
        }
        if (color == AsciiPanel.green) {
            return 4;
        }
        if (color == AsciiPanel.red) {
            return 5;
        }
        if (color == AsciiPanel.brightRed) {
            return 6;
        }
        if (color == AsciiPanel.brightGreen) {
            return 7;
        }
        if (color == Color.DARK_GRAY) {
            return 8;
        }
        return 8;
    }

    public int getGlyphNumber(char glyph) {
        if (glyph == ((char) 177)) {
            return 0;
        }
        if (glyph == ((char) 250)) {
            return 1;
        }
        if (glyph == '@') {
            return 2;
        }
        if (glyph == 'm') {
            return 3;
        }
        if (glyph == 'z') {
            return 4;
        }
        if (glyph == ((char) 3)) {
            return 5;
        }
        return 1;
    }

}
