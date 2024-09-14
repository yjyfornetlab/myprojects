package com.example.orders.configuration;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.DirectExchange;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.amqp.support.converter.Jackson2JsonMessageConverter;
import org.springframework.amqp.support.converter.MessageConverter;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class OrderServiceConfiguration {

    public static final String ORDER_DIRECT_EXCHANGE = "order-direct-exchange";
    public static final String DELIVERY_QUEUE = "delivery-queue";
    public static final String ORDER_CREATED = "order-created";

    // @Bean
    // public RabbitTemplate rabbitTemplate(ConnectionFactory connectionFactory){
    //     RabbitTemplate rabbitTemplate = new RabbitTemplate();
    //     rabbitTemplate.setConnectionFactory(connectionFactory);
    //     rabbitTemplate.setMessageConverter(jsonMessageConverter());
    //     return rabbitTemplate;
    // }

    @Bean
    public MessageConverter jsonMessageConverter(){
        return new Jackson2JsonMessageConverter();
    }
    
    @Bean
    public DirectExchange orderDirectExchange() {
        return new DirectExchange(ORDER_DIRECT_EXCHANGE);
    }

    @Bean
    public Queue deliveryQueue() {
        return new Queue(DELIVERY_QUEUE);
    }

    @Bean
    public Binding deliveryBinding() {
        return BindingBuilder.bind(deliveryQueue()).to(orderDirectExchange()).with(ORDER_CREATED);
    }
}
