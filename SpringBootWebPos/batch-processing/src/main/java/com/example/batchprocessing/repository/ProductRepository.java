package com.example.batchprocessing.repository;

import org.springframework.data.repository.CrudRepository;

import com.example.batchprocessing.model.Product;

public interface ProductRepository extends CrudRepository<Product,Integer>{
    
    // Product save(Product product);
}
