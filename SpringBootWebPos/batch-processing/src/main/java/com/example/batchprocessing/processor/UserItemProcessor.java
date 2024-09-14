package com.example.batchprocessing.processor;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.batch.item.ItemProcessor;

import com.example.batchprocessing.model.User;

public class UserItemProcessor implements ItemProcessor<User,User>{
    
    private static final Logger log = LoggerFactory.getLogger(UserItemProcessor.class);
    @Override
    public User process(User user){
        return user;
    }
}
