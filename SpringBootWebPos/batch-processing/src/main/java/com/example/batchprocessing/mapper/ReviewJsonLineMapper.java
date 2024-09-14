package com.example.batchprocessing.mapper;

import java.util.Map;

import org.springframework.batch.item.file.LineMapper;
import org.springframework.batch.item.file.mapping.JsonLineMapper;

import com.example.batchprocessing.model.Review;

public class ReviewJsonLineMapper implements LineMapper<Review>{
    
    private JsonLineMapper jsonLineMapper = new JsonLineMapper();

    @Override
    public Review mapLine(String line,int lineNumber) throws Exception{
        Review review = new Review();
        Map<String,Object> reviewMap = jsonLineMapper.mapLine(line, lineNumber);
        review.setRating((Double)reviewMap.get("rating"));
        review.setTitle((String)reviewMap.get("title"));
        review.setText((String)reviewMap.get("text"));
        review.setParentAsin((String)reviewMap.get("parent_asin"));
        review.setUserId((String)reviewMap.get("user_id"));
        review.setTimestamp((Long)reviewMap.get("timestamp"));
        return review;
    }
}
