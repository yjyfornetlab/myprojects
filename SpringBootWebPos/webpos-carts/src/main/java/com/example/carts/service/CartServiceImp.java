package com.example.carts.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cloud.client.circuitbreaker.CircuitBreaker;
import org.springframework.cloud.client.circuitbreaker.CircuitBreakerFactory;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import com.example.carts.mapper.CartMapper;
import com.example.carts.model.Cart;
import com.example.carts.model.Item;
import com.example.carts.repository.CartRepository;
import com.example.carts.repository.ItemRepository;
import com.example.webpos.rest.dto.CartDto;
import com.example.webpos.rest.dto.OrderDto;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

// import io.github.resilience4j.circuitbreaker.CircuitBreaker;

import java.util.List;
import java.util.Optional;

@Component
@Service
public class CartServiceImp implements CartService {

    // private PosDB posDB;
    // private ProductRepository productRepository;
    private CartRepository cartRepository;
    private ItemRepository itemRepository;

    private RestTemplate restTemplate;
    private CircuitBreakerFactory circuitBreakerFactory;

    private CartMapper cartMapper;

    private final String COUNTER_URL = "http://COUNTER-SERVICE/api/counter";

    private final String ORDERS_URL = "http://ORDERS-SERVICE/api/orders";
    @Autowired
    public void setCartMapper(CartMapper cartMapper) {
        this.cartMapper = cartMapper;
    }

    @Autowired
    public void setCircuitBreakerFactory(CircuitBreakerFactory circuitBreakerFactory) {
        this.circuitBreakerFactory = circuitBreakerFactory;
    }

    @Autowired
    public void setRestTemplate(RestTemplate restTemplate){
        this.restTemplate = restTemplate;
        
    }

    @Autowired
    public void setCartRepository(CartRepository cartRepository){
        this.cartRepository = cartRepository;
    }

    @Autowired
    public void setItemRepository(ItemRepository itemRepository){
        this.itemRepository = itemRepository;
    }

    @Override
    public void saveCart(Cart cart){
        this.cartRepository.save(cart);
    }

    // @Override
    // public Cart getCart() {
        // Cart cart = posDB.getCart();
        // if (cart == null) {
        //     cart = this.newCart();
        // }
        // return cart;
    //     List<Cart> carts = cartRepository.findAll();
    //     if(carts.isEmpty()){
    //         return this.newCart();
    //     }
    //     return carts.get(0);
    // }

    @Override
    public Cart findCartById(Integer cartId){
        Optional<Cart> cartOp = cartRepository.findById(cartId);
        if(cartOp.isPresent()){
            return cartOp.get();
        }
        return null;
    }

    @Override
    public List<Cart> carts(){
        return cartRepository.findAll();
    }

    @Override
    public Double getCartTotal(Integer cartId){
        CircuitBreaker circuitBreaker = circuitBreakerFactory.create("getCartTotal");

        Cart cart = this.findCartById(cartId);
        CartDto cartDto = cartMapper.toCartDto(cart);
        ObjectMapper mapper = new ObjectMapper();
        
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);
        try{
            final HttpEntity<String> request = new HttpEntity<>(mapper.writeValueAsString(cartDto),headers);
            return circuitBreaker.run(()->restTemplate.postForObject(COUNTER_URL+"/total", request, Double.class),throwable->Double.valueOf(-1));
        }catch(JsonProcessingException e){
            throw new RuntimeException(e);
        }
    }

    @Override
    public OrderDto checkout(Integer cartId){
        CircuitBreaker circuitBreaker = circuitBreakerFactory.create("checkout");

        Cart cart = this.findCartById(cartId);
        CartDto cartDto = cartMapper.toCartDto(cart);
        ObjectMapper mapper = new ObjectMapper();
        
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);
        try{
            final HttpEntity<String> request = new HttpEntity<>(mapper.writeValueAsString(cartDto.getItems()),headers);
            return circuitBreaker.run(()->restTemplate.postForObject(ORDERS_URL, request, OrderDto.class),throwable->null);
        }catch(JsonProcessingException e){
            throw new RuntimeException(e);
        }
    }

    @Override
    public boolean add(Item item, Integer cartId){
        Cart cart = this.findCartById(cartId);
        if(cart==null){
            return false;
        }
        cart.addItem(item);
        cartRepository.save(cart);
        return true;
    }

    // @Override
    // public void emptyCart(){
    //     Cart cart = this.getCart();
    //     cart.clearItems();
    //     itemRepository.deleteAll();
    //     this.total(cart);
    // }

    // @Override
    // public void increaseItem(Integer id){
    //     Optional<Item> itemOp = itemRepository.findById(id);
    //     if(itemOp.isPresent()){
    //         Item item = itemOp.get();
    //         item.increase();
    //         itemRepository.save(item);
    //         this.total(null);
    //     }
    // }

    // @Override
    // public void decreaseItem(Integer id){
    //     Optional<Item> itemOp = itemRepository.findById(id);
    //     if(itemOp.isPresent()){
    //         Item item = itemOp.get();
    //         item.decrease();
    //         itemRepository.save(item);
    //         if(item.getQuantity()==0){
    //             this.deleteItem(item.getId());
    //             return;
    //         }
    //         this.total(null);
    //     }
    // }
    
    @Override
    public boolean deleteItem(Item item,Integer cartId){
        Optional<Item> itemOp = this.itemRepository.findById(item.getId());
        Optional<Cart> cartOp = this.cartRepository.findById(cartId);
        if(itemOp.isPresent() && cartOp.isPresent()){
            Cart cart = cartOp.get();
            cart.removeItem(itemOp.get());
            itemRepository.deleteById(item.getId());
            // this.total(cart);
            cartRepository.save(cart);
            return true;
        }
        return false;
    }

}
