package com.example.batchprocessing.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.Repository;

import com.example.batchprocessing.model.Review;

public interface ReviewRepository extends CrudRepository<Review,Integer>{
    
    // Review save(Review review);
}
