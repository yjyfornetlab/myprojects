package com.example.products.mapper;

import java.util.Collection;
// import java.util.List;

import org.mapstruct.Mapper;

import com.example.products.model.Product;
import com.example.webpos.rest.dto.ProductDto;

@Mapper
public interface ProductMapper {
    
    ProductDto toProductDto(Product product);
    Product toProduct(ProductDto productDto);
    Collection<ProductDto> toProductDtos(Collection<Product> products);
    Collection<Product> toProducts(Collection<ProductDto> productDtos);
}
