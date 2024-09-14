package com.example.batchprocessing.writer;

import org.springframework.batch.item.Chunk;
import org.springframework.batch.item.ItemWriter;
import org.springframework.beans.factory.annotation.Autowired;

import com.example.batchprocessing.model.User;
import com.example.batchprocessing.repository.UserRepository;

public class UserWriter implements ItemWriter<User>{
    
    private UserRepository userRepository;

    @Autowired
    public void setUserRepository(UserRepository userRepository){
        this.userRepository = userRepository;
    }

    @Override
    public void write(Chunk<? extends User> users){
        for(User user : users){
            userRepository.save(user);
        }
    }
}
