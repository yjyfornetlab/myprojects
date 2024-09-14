package com.example.delivery.service;

import org.springframework.amqp.rabbit.annotation.RabbitHandler;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.example.delivery.configuration.DeliveryServiceConfiguration;
import com.example.delivery.mapper.OrderMapper;
import com.example.delivery.model.Order;
import com.example.delivery.repository.OrderRepository;
import com.example.webpos.rest.dto.OrderDto;

@Component
@RabbitListener(queues = DeliveryServiceConfiguration.DELIVERY_QUEUE)
public class DeliveryServiceImp implements DeliveryService{
    
    private OrderRepository orderRepository;

    private OrderMapper orderMapper;

    @Autowired
    public void setOrderRepository(OrderRepository orderRepository){
        this.orderRepository = orderRepository;
    }

    @Autowired
    public void setOrderMapper(OrderMapper orderMapper){
        this.orderMapper = orderMapper;
    }
    

    @Override
    @RabbitHandler
    public void deliver(OrderDto orderDto){
        Order order = orderMapper.toOrder(orderDto);
        orderRepository.save(order);
        System.out.println("Deliver Order: "+order+" now!");
    }

    // @Override
    // @RabbitHandler
    // public void deliver(Order order){
    //     // Order order = orderMapper.toOrder(orderDto);
    //     orderRepository.save(order);
    //     System.out.println("Deliver Order: "+order+" now!");
    // }
}
