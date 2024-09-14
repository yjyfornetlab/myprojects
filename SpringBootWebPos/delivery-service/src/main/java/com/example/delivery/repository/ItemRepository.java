package com.example.delivery.repository;

import java.util.Optional;

import org.springframework.data.repository.Repository;

import com.example.delivery.model.Item;

public interface ItemRepository extends Repository<Item,Integer>{

    Optional<Item> findById(Integer id);

    Item save(Item item);

    void deleteById(Integer id);

    void deleteAll();
}
