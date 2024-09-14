package com.example.delivery.mapper;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import org.mapstruct.Mapper;

import com.example.delivery.model.Item;
import com.example.delivery.model.Order;
import com.example.webpos.rest.dto.ItemDto;
import com.example.webpos.rest.dto.OrderDto;
import com.example.webpos.rest.dto.ProductDto;

@Mapper
public interface OrderMapper {

    default OrderDto toOrderDto(Order order){
        OrderDto orderDto = new OrderDto();
        orderDto.setId(order.getId());
        orderDto.setItems(toItemsDto(order.getItems()));
        return orderDto;
    }

    default Order toOrder(OrderDto orderDto){
        Order order = new Order();
        order.setId(orderDto.getId());
        order.setItems(toItems(orderDto.getItems(),orderDto.getId()));
        return order;
    }
    // Cart toCart(CartDto cartDto);
    Collection<OrderDto> toOrdersDto(Collection<Order> orders);
    Collection<Order> toOrders(Collection<OrderDto> orderDtos);

    default List<Item> toItems(List<ItemDto> itemDtos,Integer orderId){
        if(itemDtos==null){
            return null;
        }
        List<Item> items = new ArrayList<>(itemDtos.size());
        for(ItemDto itemDto : itemDtos){
            items.add(toItem(itemDto,orderId));
        }
        return items;
    }

    default Item toItem(ItemDto itemDto,Integer orderId){
        Item item = new Item();
        item.setId(itemDto.getId());
        item.setOrderId(orderId);
        item.setProductId(itemDto.getProduct().getId());
        item.setProductName(itemDto.getProduct().getName());
        item.setProductPrice(itemDto.getProduct().getPrice());
        item.setQuantity(itemDto.getQuantity());
        return item;
    }

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

