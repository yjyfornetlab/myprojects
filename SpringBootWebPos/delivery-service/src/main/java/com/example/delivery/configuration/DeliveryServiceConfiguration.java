package com.example.delivery.configuration;

import java.util.List;

import org.springframework.amqp.support.converter.Jackson2JsonMessageConverter;
import org.springframework.amqp.support.converter.MessageConverter;
import org.springframework.amqp.support.converter.SimpleMessageConverter;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.fasterxml.jackson.databind.ObjectMapper;

@Configuration
public class DeliveryServiceConfiguration {
    
    public static final String DELIVERY_QUEUE = "delivery-queue";

    // @Bean
    // public SimpleMessageConverter converter(){
    //     SimpleMessageConverter converter = new SimpleMessageConverter();
    //     converter.setAllowedListPatterns(List.of("com.example.delivery.model.*"));
    //     return converter;
    // }

    @Bean
    public MessageConverter jsonMessageConverter(ObjectMapper objectMapper){
        return new Jackson2JsonMessageConverter(objectMapper);
    }
}
