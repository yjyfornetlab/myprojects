package com.example.products.rest;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import com.example.products.mapper.ProductMapper;
import com.example.products.mapper.ReviewMapper;
import com.example.products.model.Product;
import com.example.products.service.ProductService;
import com.example.webpos.rest.api.ProductApi;
// import com.example.webpos.rest.api.ProductsApi;
import com.example.webpos.rest.dto.ProductDto;
import com.example.webpos.rest.dto.ReviewDto;

@RestController
@CrossOrigin(exposedHeaders = "errors, content-type")
@RequestMapping("api")
public class ProductRestController implements ProductApi{

    private static final Logger log = LoggerFactory.getLogger(ProductRestController.class);

    private final ProductService productService;
    private final ProductMapper productMapper;
    private final ReviewMapper reviewMapper;

    public ProductRestController(ProductService productService,ProductMapper productMapper,ReviewMapper reviewMapper){
        this.productService = productService;
        this.productMapper = productMapper;
        this.reviewMapper = reviewMapper;
    }
    
    @Override
    // @RequestMapping(value = "/product",method = RequestMethod.GET)
    public ResponseEntity<List<ProductDto>> listProducts(){
        // log.info("reach");
        List<ProductDto> productsDto = new ArrayList<>(productMapper.toProductDtos(this.productService.products()));
        if(productsDto.isEmpty()){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<>(productsDto,HttpStatus.OK);
    }

    @Override
    public ResponseEntity<ProductDto> showProductById(Integer productId){
        ProductDto product = this.productMapper.toProductDto(this.productService.findProductById(productId));
        if(product==null){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<>(product,HttpStatus.OK);
    }

    @Override
    public ResponseEntity<List<ReviewDto>> showReview(Integer productId){
        List<ReviewDto> reviewDtos = new ArrayList<>(reviewMapper.toReviewDtos(this.productService.getReviews(productId)));
        return new ResponseEntity<>(reviewDtos,HttpStatus.OK);
    }
}
