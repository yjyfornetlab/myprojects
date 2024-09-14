package com.example.orders.repository;

import java.util.List;
import java.util.Optional;

import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.Repository;

import com.example.orders.model.Order;

public interface OrderRepository extends CrudRepository<Order,Integer>{
    
    // Order save(Order cart);

    // Optional<Order> findById(Integer id);

    List<Order> findAll();
}
