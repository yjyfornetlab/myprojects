package com.example.delivery.service;

import com.example.delivery.model.Order;
import com.example.webpos.rest.dto.OrderDto;

public interface DeliveryService {
    
    public void deliver(OrderDto orderDto);

    // public void deliver(Order order);
}
