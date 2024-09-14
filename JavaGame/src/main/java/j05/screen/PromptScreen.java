package j05.screen;

import java.awt.event.KeyEvent;

import asciiPanel.AsciiPanel;

public class PromptScreen implements Screen{

    private String prompt;
    private int type;

    public PromptScreen(String prompt,int type){
        this.prompt = prompt;
        this.type = type;
    }

    @Override
    public void displayOutput(AsciiPanel terminal){
        int top = (SCREEN_HEIGHT-3)/2;
        terminal.writeCenter(prompt, top,AsciiPanel.brightWhite);
        if(type==0){
            terminal.writeCenter("You can exit now.",top+2,AsciiPanel.brightWhite);
        }

        if(type==1){
            terminal.writeCenter("-- press [r] to return to game --", top+2,AsciiPanel.brightWhite);
        }
    }

    @Override
    public Screen respondToUserInput(KeyEvent key){
        if(type==1){
            if(key.getKeyCode()==KeyEvent.VK_R){
                return null;
            }
        }
        return this;
    }
}
