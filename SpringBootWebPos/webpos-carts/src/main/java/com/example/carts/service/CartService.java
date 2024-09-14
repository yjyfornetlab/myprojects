package com.example.carts.service;

import java.util.List;

import com.example.carts.model.Cart;
import com.example.carts.model.Item;
import com.example.webpos.rest.dto.OrderDto;

public interface CartService {

    public Cart findCartById(Integer cartId);

    public void saveCart(Cart cart);

    // public Cart newCart();
    public List<Cart> carts();

    public OrderDto checkout(Integer cartId);

    // public void total(Cart cart);

    public Double getCartTotal(Integer cartId);

    // public boolean add(Product product, int amount);

    // public boolean add(String productId, int amount);

    public boolean add(Item item,Integer cartId);

    // public void emptyCart();

    // public void increaseItem(Integer id);

    // public void decreaseItem(Integer id);

    public boolean deleteItem(Item item,Integer cartId);

    // public List<Product> products();

    // public Product findProductById(String productId);
}
