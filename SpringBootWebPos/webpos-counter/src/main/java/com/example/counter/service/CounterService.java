package com.example.counter.service;

import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;

import com.example.webpos.rest.dto.CartDto;
import com.example.webpos.rest.dto.ItemDto;

@Component
@Service
public class CounterService {

    public double getTotal(CartDto cartDto) {
        double total = 0;
        for(ItemDto itemDto : cartDto.getItems()) {
            total += itemDto.getQuantity()*itemDto.getProduct().getPrice();
        }
        return total;
    }
}
