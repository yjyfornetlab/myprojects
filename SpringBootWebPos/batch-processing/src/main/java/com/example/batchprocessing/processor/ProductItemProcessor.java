package com.example.batchprocessing.processor;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.batch.item.ItemProcessor;

import com.example.batchprocessing.model.Product;

public class ProductItemProcessor implements ItemProcessor<Product,Product>{
    
    private static final Logger log = LoggerFactory.getLogger(ProductItemProcessor.class);
    @Override
    public Product process(Product product){
        return product;
    }
}
