package com.example.delivery.repository;

import java.util.List;
import java.util.Optional;

import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.Repository;

import com.example.delivery.model.Order;

public interface OrderRepository extends CrudRepository<Order,Integer>{
    
    // Order save(Order cart);

    // Optional<Order> findById(Integer id);

    List<Order> findAll();
}
