package com.example.carts.rest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.http.HttpStatus;
import org.springframework.http.HttpStatusCode;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.carts.mapper.CartMapper;
import com.example.carts.model.Cart;
import com.example.carts.model.Item;
import com.example.carts.service.CartService;
import com.example.webpos.rest.api.CartsApi;
import com.example.webpos.rest.dto.CartDto;
import com.example.webpos.rest.dto.ItemDto;
import com.example.webpos.rest.dto.OrderDto;
import com.example.webpos.rest.dto.ProductDto;

@RestController
@CrossOrigin(exposedHeaders = "errors, content-type")
@RequestMapping("api")
public class CartRestController implements CartsApi{

    private final CartService cartService;
    private final CartMapper cartMapper;
    // private final ItemMapper itemMapper;
    public CartRestController(CartService cartService,CartMapper cartMapper){
        this.cartService = cartService;
        this.cartMapper = cartMapper;
        // this.itemMapper = itemMapper;
    }

    @Override
    public ResponseEntity<CartDto> addItemToCart(Integer cartId,ItemDto itemDto){
        Cart cart = this.cartService.findCartById(cartId);
        if(cart==null){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        Item item = this.cartMapper.toItem(itemDto,cartId);
        this.cartService.add(item, cartId);
        CartDto cartDto = this.cartMapper.toCartDto(cart);
        return new ResponseEntity<>(cartDto,HttpStatus.CREATED);
    }

    @Override
    public ResponseEntity<CartDto> createCart(CartDto cartDto){
        this.cartService.saveCart(this.cartMapper.toCart(cartDto));
        return new ResponseEntity<>(cartDto,HttpStatus.OK);
    }

    @Override
    public ResponseEntity<List<CartDto>> listCarts(){
        List<CartDto> cartsDto = new ArrayList<>(cartMapper.toCartsDto(this.cartService.carts()));
        if(cartsDto.isEmpty()){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<>(cartsDto,HttpStatus.OK);
    }

    @Override
    public ResponseEntity<CartDto> showCartById(Integer cartId){
        CartDto cart = this.cartMapper.toCartDto(this.cartService.findCartById(cartId));
        if(cart==null){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<>(cart,HttpStatus.OK);
    }

    @Override
    public ResponseEntity<Double> showCartTotal(Integer cartId){
        Double total = this.cartService.getCartTotal(cartId);
        if(total < 0){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<>(total,HttpStatus.OK);
    }

    @Override
    public ResponseEntity<CartDto> deleteItem(Integer cartId,ItemDto itemDto){
        Item item = this.cartMapper.toItem(itemDto, cartId);
        if(this.cartService.deleteItem(item, cartId)){
            return new ResponseEntity<>(HttpStatus.NO_CONTENT);
        }
        return new ResponseEntity<>(HttpStatus.NOT_FOUND);
    }

    @Override 
    public ResponseEntity<OrderDto> checkout(Integer cartId){
        OrderDto orderDto = this.cartService.checkout(cartId);
        if(orderDto==null){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<>(orderDto,HttpStatus.OK);
    }
}
