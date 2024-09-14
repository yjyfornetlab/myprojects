package com.example.carts.repository;

import java.util.Optional;

import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.Repository;

import com.example.carts.model.Item;

public interface ItemRepository extends CrudRepository<Item,Integer>{

    Optional<Item> findById(Integer id);

    // Item save(Item item);

    void deleteById(Integer id);

    void deleteAll();
}
