package com.example.orders.service;

import java.util.List;

import com.example.orders.model.Item;
import com.example.orders.model.Order;

public interface OrderService {

    public Order findOrderById(Integer orderId);

    public Order newOrder();

    public void saveOrder(Order order);

    public List<Order> orders();

    // public void processOrderDelivery(Order order);

}
