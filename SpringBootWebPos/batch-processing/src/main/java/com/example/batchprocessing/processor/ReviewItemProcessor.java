package com.example.batchprocessing.processor;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.StringWriter;
import java.nio.charset.StandardCharsets;
import java.util.List;

import javax.script.ScriptContext;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.SimpleScriptContext;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.batch.item.ItemProcessor;
import org.springframework.core.io.ClassPathResource;

import com.example.batchprocessing.model.Review;

public class ReviewItemProcessor implements ItemProcessor<Review, Review> {

    private static final Logger log = LoggerFactory.getLogger(ReviewItemProcessor.class);

    private String translateToZh(String reviewText) {

        try {
            String s = null;
            StringBuilder output = new StringBuilder();
            String[] cmd = new String[3];
            cmd[0] = "python";
            cmd[1] = "translate.py";
            cmd[2] = reviewText;
            Process p = Runtime.getRuntime().exec(cmd);
            BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream(),"UTF-8"));
            while ((s = in.readLine()) != null) {
                output.append(s);
            }
            return output.toString();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private String summarize(String reviewText) {

        try {
            String s = null;
            StringBuilder output = new StringBuilder();
            String[] cmd = new String[3];
            cmd[0] = "python";
            cmd[1] = "summarize.py";
            cmd[2] = reviewText;
            Process p = Runtime.getRuntime().exec(cmd);
            BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream(),"UTF-8"));
            while ((s = in.readLine()) != null) {
                output.append(s);
            }
            return output.toString();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
  
    @Override
    public Review process(Review review) {
        
        String summarizedText = summarize(review.getText());
        review.setText(summarizedText);
        // String translatedText = translateToZh(review.getText());
        // review.setText(translatedText);
        
        return review;
    }
}
