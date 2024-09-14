package com.example.batchprocessing.writer;

import org.springframework.batch.item.Chunk;
import org.springframework.batch.item.ItemWriter;
import org.springframework.beans.factory.annotation.Autowired;

import com.example.batchprocessing.model.Product;
import com.example.batchprocessing.repository.ProductRepository;

public class ProductWriter implements ItemWriter<Product>{
    private ProductRepository productRepository;

    @Autowired
    public void setProductRepository(ProductRepository productRepository){
        this.productRepository = productRepository;
    }

    @Override
    public void write(Chunk<? extends Product> products) {
        for(Product product : products){
            productRepository.save(product);
        }
    }
}
