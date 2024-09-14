package com.example.products.model;

import java.io.Serializable;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
@Entity
@Table(name = "products")
public class Product implements Serializable{

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    private String category;
    private String name;
    private Double averageRating;
    private Integer ratingNumber;
    private String img;
    private String parentAsin;
    private Integer stock;
    private Integer quantity;
    private Double price;

    public Product(){

    }

    @Override
    public String toString() {
        return getId() + "\t" + getName() + "\t" + getParentAsin();
    }

}
