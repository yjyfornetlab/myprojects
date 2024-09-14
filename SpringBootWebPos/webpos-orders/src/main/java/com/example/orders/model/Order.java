package com.example.orders.model;

import lombok.Data;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import jakarta.persistence.CascadeType;
import jakarta.persistence.Entity;
import jakarta.persistence.FetchType;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.OneToMany;

@Data
@Entity(name = "Orders")
public class Order implements Serializable{

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    @OneToMany(cascade = CascadeType.ALL,fetch = FetchType.EAGER)
    private List<Item> items = new ArrayList<>();

    // private double totalPrice;

    public Order(){
        
    }

    public boolean addItem(Item item) {
        return items.add(item);
    }

    public int getSize(){
        return items.size();
    }

    public void clearItems(){
        items.clear();
    }

    public void removeItem(Item item){
        items.remove(item);
    }

    // public double calculateTotal(){
    //     double totalPrice = 0;
    //     for (int i = 0; i < items.size(); i++) {
    //         totalPrice += items.get(i).getQuantity() * items.get(i).getProductPrice();
    //     }
    //     return totalPrice;
    // }

    @Override
    public String toString() {
        if (items.size() == 0) {
            return "Empty Order";
        }
        double total = 0;
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("Order -----------------\n");

        for (int i = 0; i < items.size(); i++) {
            stringBuilder.append(items.get(i).toString()).append("\n");
            total += items.get(i).getQuantity() * items.get(i).getProductPrice();
        }
        stringBuilder.append("----------------------\n");

        stringBuilder.append("Total...\t\t\t" + total);

        return stringBuilder.toString();
    }
}
