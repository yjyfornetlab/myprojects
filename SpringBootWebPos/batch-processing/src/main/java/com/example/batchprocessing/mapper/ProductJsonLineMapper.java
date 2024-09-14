package com.example.batchprocessing.mapper;

import java.util.List;
import java.util.Map;

import org.springframework.batch.item.file.LineMapper;
import org.springframework.batch.item.file.mapping.JsonLineMapper;

import com.example.batchprocessing.model.Product;

public class ProductJsonLineMapper implements LineMapper<Product>{
    
    private JsonLineMapper jsonLineMapper = new JsonLineMapper();

    @Override
    public Product mapLine(String line,int lineNumber) throws Exception{
        Product product = new Product();
        Map<String,Object> productMap = jsonLineMapper.mapLine(line, lineNumber);
        product.setCategory((String)productMap.get("main_category"));
        product.setName((String)productMap.get("title"));
        product.setAverageRating((Double)productMap.get("average_rating"));
        product.setRatingNumber((Integer)productMap.get("rating_number"));
        List<Map<String,Object>> images = (List<Map<String,Object>>)productMap.get("images");
        Map<String,Object> image = images.get(0);
        String imageUrl = (String)image.get("hi_res");
        if(imageUrl == null){
            imageUrl = (String)image.get("large");
        }
        if(imageUrl == null){
            imageUrl = "";
        }
        product.setImg(imageUrl);
        product.setParentAsin((String)productMap.get("parent_asin"));
        product.setStock(1);
        product.setQuantity(20);
        String priceString = (String)productMap.get("price");
        Double price = null;
        try{
            price = Double.parseDouble(priceString);
        }catch(Exception e){
            price = 10.0;
        }
        product.setPrice(price);
        return product;
    }
}
