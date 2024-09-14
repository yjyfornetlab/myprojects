package com.example.batchprocessing.repository;

import org.springframework.data.repository.CrudRepository;

import com.example.batchprocessing.model.User;

public interface UserRepository extends CrudRepository<User,String>{
    
}
