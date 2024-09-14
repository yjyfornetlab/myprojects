package com.example.orders.service;

import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cloud.client.circuitbreaker.CircuitBreaker;
import org.springframework.cloud.client.circuitbreaker.CircuitBreakerFactory;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import com.example.orders.configuration.OrderServiceConfiguration;
import com.example.orders.mapper.OrderMapper;
import com.example.orders.model.Item;
import com.example.orders.model.Order;
import com.example.orders.repository.ItemRepository;
import com.example.orders.repository.OrderRepository;
import com.example.webpos.rest.dto.CartDto;
import com.example.webpos.rest.dto.OrderDto;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

// import io.github.resilience4j.circuitbreaker.CircuitBreaker;

import java.util.List;
import java.util.Optional;

@Component
@Service
public class OrderServiceImp implements OrderService {

    private OrderRepository orderRepository;
    private ItemRepository itemRepository;

    // @Autowired
    // private RabbitTemplate rabbitTemplate;
    // private RestTemplate restTemplate;
    // private CircuitBreakerFactory circuitBreakerFactory;

    private OrderMapper orderMapper;

    private final String COUNTER_URL = "http://COUNTER-SERVICE/counter";

    // @Override
    // public void processOrderDelivery(Order order){
    //     rabbitTemplate.convertAndSend(OrderServiceConfiguration.ORDER_DIRECT_EXCHANGE,OrderServiceConfiguration.ORDER_CREATED, order);
    // }

    @Autowired
    public void setOrderMapper(OrderMapper orderMapper) {
        this.orderMapper = orderMapper;
    }

    // @Autowired
    // public void setCircuitBreakerFactory(CircuitBreakerFactory circuitBreakerFactory) {
    //     this.circuitBreakerFactory = circuitBreakerFactory;
    // }

    // @Autowired
    // public void setRestTemplate(RestTemplate restTemplate){
    //     this.restTemplate = restTemplate;
        
    // }

    @Autowired
    public void setOrderRepository(OrderRepository orderRepository){
        this.orderRepository = orderRepository;
    }

    @Autowired
    public void setItemRepository(ItemRepository itemRepository){
        this.itemRepository = itemRepository;
    }

    

    @Override
    public Order newOrder(){
        Order order = new Order();
        order = this.orderRepository.save(order);
        // processOrderDelivery(order);
        return order;
    }

    @Override
    public void saveOrder(Order order){
        // processOrderDelivery(order);
        this.orderRepository.save(order);
    }

    @Override
    public Order findOrderById(Integer orderId){
        Optional<Order> orderOp = orderRepository.findById(orderId);
        if(orderOp.isPresent()){
            return orderOp.get();
        }
        return null;
    }

    @Override
    public List<Order> orders(){
        return orderRepository.findAll();
    }

}
