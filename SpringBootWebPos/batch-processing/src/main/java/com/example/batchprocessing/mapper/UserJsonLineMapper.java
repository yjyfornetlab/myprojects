package com.example.batchprocessing.mapper;

import java.util.Map;

import org.springframework.batch.item.file.LineMapper;
import org.springframework.batch.item.file.mapping.JsonLineMapper;

import com.example.batchprocessing.model.User;

public class UserJsonLineMapper implements LineMapper<User>{
    
    private JsonLineMapper jsonLineMapper = new JsonLineMapper();

    @Override
    public User mapLine(String line,int lineNumber) throws Exception{
        User user = new User();
        Map<String,Object> userMap = jsonLineMapper.mapLine(line, lineNumber);
        user.setUserId((String)userMap.get("user_id"));
        return user;
    }
}
