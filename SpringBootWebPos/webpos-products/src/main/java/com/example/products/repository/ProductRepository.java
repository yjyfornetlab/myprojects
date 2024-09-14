package com.example.products.repository;

import java.util.List;
import java.util.Optional;

import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.Repository;

import com.example.products.model.Product;

public interface ProductRepository extends CrudRepository<Product,Integer>{

    // Optional<Product> findById(String id);

    // Product save(Product product);

    List<Product> findAll();
}
