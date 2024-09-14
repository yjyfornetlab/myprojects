package com.example.counter.rest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.counter.service.CounterService;
import com.example.webpos.rest.api.CounterApi;
import com.example.webpos.rest.dto.CartDto;

@RestController
@CrossOrigin(exposedHeaders = "errors, content-type")
@RequestMapping("api")
public class CounterRestController implements CounterApi{

    private final CounterService counterService;
    public CounterRestController(CounterService counterService){
        this.counterService = counterService;
    }
    
    @Override
    public ResponseEntity<Double> total(CartDto cartDto) {
        return new ResponseEntity<>(this.counterService.getTotal(cartDto),HttpStatus.OK);
    }
}
