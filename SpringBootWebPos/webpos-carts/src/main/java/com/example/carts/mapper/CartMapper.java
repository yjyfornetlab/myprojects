package com.example.carts.mapper;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import org.mapstruct.Mapper;

import com.example.carts.model.Cart;
import com.example.carts.model.Item;
import com.example.webpos.rest.dto.CartDto;
import com.example.webpos.rest.dto.ItemDto;
import com.example.webpos.rest.dto.ProductDto;

@Mapper
public interface CartMapper {

    default CartDto toCartDto(Cart cart){
        CartDto cartDto = new CartDto();
        cartDto.setId(cart.getId());
        cartDto.setItems(toItemsDto(cart.getItems()));
        return cartDto;
    }

    default Cart toCart(CartDto cartDto){
        Cart cart = new Cart();
        cart.setId(cartDto.getId());
        cart.setItems(toItems(cartDto.getItems(),cartDto.getId()));
        return cart;
    }
    // Cart toCart(CartDto cartDto);
    Collection<CartDto> toCartsDto(Collection<Cart> carts);
    Collection<Cart> toCarts(Collection<CartDto> cartDtos);

    default List<Item> toItems(List<ItemDto> itemDtos,Integer cartId){
        if(itemDtos==null){
            return null;
        }
        List<Item> items = new ArrayList<>(itemDtos.size());
        for(ItemDto itemDto : itemDtos){
            items.add(toItem(itemDto,cartId));
        }
        return items;
    }

    default Item toItem(ItemDto itemDto,Integer cartId){
        Item item = new Item();
        item.setId(itemDto.getId());
        item.setCartId(cartId);
        item.setProductId(itemDto.getProduct().getId());
        item.setProductName(itemDto.getProduct().getName());
        item.setProductPrice(itemDto.getProduct().getPrice());
        item.setQuantity(itemDto.getQuantity());
        return item;
    }
    // default Cart map(CartDto cartDto){
    //     Cart cart = new Cart();
    //     cart.setId(cartDto.getId());
    //     List<ItemDto> itemDtos = cartDto.getItems();
    //     List<Item> items = new ArrayList<>();
    //     for(ItemDto itemDto : itemDtos){
            
    //     }
    // }

    default List<ItemDto> toItemsDto(List<Item> items){
        if(items==null){
            return null;
        }
        List<ItemDto> itemDtos = new ArrayList<>(items.size());
        for(Item item : items){
            itemDtos.add(toItemDto(item));
        }
        return itemDtos;
    }

    default ItemDto toItemDto(Item item){
        ItemDto itemDto = new ItemDto();
        itemDto.setId(item.getId());
        itemDto.setQuantity(item.getQuantity());
        itemDto.setProduct(getProductDto(item));
        return itemDto;
    }

    default ProductDto getProductDto(Item item){
        ProductDto productDto = new ProductDto();
        productDto.setId(item.getProductId());
        productDto.setPrice(item.getProductPrice());
        productDto.setName(item.getProductName());
        return productDto;
    }

}
