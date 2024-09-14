package com.example.delivery.model;


import java.io.Serializable;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
// import lombok.AllArgsConstructor;
import lombok.Data;

@Data
// @AllArgsConstructor
@Entity
public class Item implements Serializable{
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    // @ManyToOne
    // private Product product;
    @JoinColumn(name = "product_id")
    private Integer productId;

    @JoinColumn(name = "product_name")
    private String productName;

    @JoinColumn(name = "product_price")
    private double productPrice;

    private int quantity;
    
    // @ManyToOne
    @JoinColumn(name = "order_id")
    // public Cart cart;
    private Integer orderId;
    public Item(){

    }

    // public Item(Product product,int quantity,Cart cart){
    //     this.product = product;
    //     this.quantity = quantity;
    //     this.cart = cart;
    // }

    public void increase(){
        quantity+=1;
    }

    public void decrease(){
        quantity-=1;
    }

    @Override
    public String toString() {
        // return product.toString() + "\t" + quantity;
        return productId + "\t" + quantity;
    }
}
