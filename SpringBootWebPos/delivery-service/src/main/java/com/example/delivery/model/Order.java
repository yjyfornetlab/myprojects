package com.example.delivery.model;

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

    @Override
    public String toString() {
        return String.valueOf(this.id);
    }
}
