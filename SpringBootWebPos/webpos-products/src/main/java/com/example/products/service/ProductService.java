package com.example.products.service;

import java.util.List;

import com.example.products.model.Product;
import com.example.products.model.Review;

public interface ProductService {

    // public Cart findCartById(Integer cartId);

    // public void saveCart(Cart cart);

    // public Cart newCart();
    // public List<Cart> carts();

    // public void checkout();

    // public void total(Cart cart);

    // public double getCartTotal(Integer cartId);

    // public boolean add(Product product, int amount);

    // public boolean add(String productId, int amount);

    // public boolean add(Item item,Integer cartId);

    // public void emptyCart();

    // public void increaseItem(Integer id);

    // public void decreaseItem(Integer id);

    // public boolean deleteItem(Item item,Integer cartId);

    public List<Product> products();

    public Product findProductById(Integer productId);

    public List<Review> getReviews(Integer productId);
}
