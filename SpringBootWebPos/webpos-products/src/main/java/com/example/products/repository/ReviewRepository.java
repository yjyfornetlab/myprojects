package com.example.products.repository;

import java.util.List;

import org.springframework.data.repository.CrudRepository;

import com.example.products.model.Review;

public interface ReviewRepository extends CrudRepository<Review,Integer>{
    
    List<Review> findByParentAsin(String parentAsin);

    List<Review> findAll();
}
