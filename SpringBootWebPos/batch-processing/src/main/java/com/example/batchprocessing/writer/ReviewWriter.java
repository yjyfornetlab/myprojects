package com.example.batchprocessing.writer;

import org.springframework.batch.item.Chunk;
import org.springframework.batch.item.ItemWriter;
import org.springframework.beans.factory.annotation.Autowired;

import com.example.batchprocessing.model.Review;
import com.example.batchprocessing.repository.ReviewRepository;

public class ReviewWriter implements ItemWriter<Review>{
    
    private ReviewRepository reviewRepository;

    @Autowired
    public void setReviewRepository(ReviewRepository reviewRepository){
        this.reviewRepository = reviewRepository;
    }

    @Override
    public void write(Chunk<? extends Review> reviews) {
        for(Review review : reviews){
            reviewRepository.save(review);
        }
    }
}
