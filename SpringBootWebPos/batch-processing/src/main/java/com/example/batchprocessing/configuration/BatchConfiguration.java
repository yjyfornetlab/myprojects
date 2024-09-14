package com.example.batchprocessing.configuration;

import org.springframework.batch.core.Job;
import org.springframework.batch.core.Step;
import org.springframework.batch.core.job.builder.JobBuilder;
import org.springframework.batch.core.launch.support.RunIdIncrementer;
import org.springframework.batch.core.repository.JobRepository;
import org.springframework.batch.core.step.builder.StepBuilder;
import org.springframework.batch.item.file.FlatFileItemReader;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.ClassPathResource;
import org.springframework.core.task.SimpleAsyncTaskExecutor;
import org.springframework.core.task.TaskExecutor;
import org.springframework.orm.jpa.JpaTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;

import com.example.batchprocessing.mapper.ProductJsonLineMapper;
import com.example.batchprocessing.mapper.ReviewJsonLineMapper;
import com.example.batchprocessing.mapper.UserJsonLineMapper;
import com.example.batchprocessing.model.Product;
import com.example.batchprocessing.model.Review;
import com.example.batchprocessing.model.User;
import com.example.batchprocessing.processor.ProductItemProcessor;
import com.example.batchprocessing.processor.ReviewItemProcessor;
import com.example.batchprocessing.processor.UserItemProcessor;
import com.example.batchprocessing.writer.ProductWriter;
import com.example.batchprocessing.writer.ReviewWriter;
import com.example.batchprocessing.writer.UserWriter;

@Configuration
public class BatchConfiguration {

    @Bean
    public FlatFileItemReader<Review> reviewReader1() {
        FlatFileItemReader<Review> reader = new FlatFileItemReader<>();
        reader.setResource(new ClassPathResource("Subscription_Boxes.jsonl"));
        reader.setLineMapper(new ReviewJsonLineMapper());
        return reader;
    }

    @Bean
    public FlatFileItemReader<Product> productReader1() {
        FlatFileItemReader<Product> reader = new FlatFileItemReader<>();
        reader.setResource(new ClassPathResource("meta_Subscription_Boxes.jsonl"));
        reader.setLineMapper(new ProductJsonLineMapper());
        return reader;
    }

    @Bean
    public FlatFileItemReader<User> userReader1(){
        FlatFileItemReader<User> reader = new FlatFileItemReader<>();
        reader.setResource(new ClassPathResource("Subscription_Boxes.jsonl"));
        reader.setLineMapper(new UserJsonLineMapper());
        return reader;
    }

    @Bean
    public FlatFileItemReader<Review> reviewReader2() {
        FlatFileItemReader<Review> reader = new FlatFileItemReader<>();
        reader.setResource(new ClassPathResource("Magazine_Subscriptions.jsonl"));
        reader.setLineMapper(new ReviewJsonLineMapper());
        return reader;
    }

    @Bean
    public FlatFileItemReader<Product> productReader2() {
        FlatFileItemReader<Product> reader = new FlatFileItemReader<>();
        reader.setResource(new ClassPathResource("meta_Magazine_Subscriptions.jsonl"));
        reader.setLineMapper(new ProductJsonLineMapper());
        return reader;
    }

    @Bean
    public FlatFileItemReader<User> userReader2(){
        FlatFileItemReader<User> reader = new FlatFileItemReader<>();
        reader.setResource(new ClassPathResource("Magazine_Subscriptions.jsonl"));
        reader.setLineMapper(new UserJsonLineMapper());
        return reader;
    }

    @Bean
    public ReviewItemProcessor reviewItemProcessor() {
        return new ReviewItemProcessor();
    }

    @Bean
    public ProductItemProcessor productItemProcessor() {
        return new ProductItemProcessor();
    }

    @Bean
    public UserItemProcessor userItemProcessor(){
        return new UserItemProcessor();
    }

    @Bean
    public ReviewWriter reviewWriter() {
        return new ReviewWriter();
    }

    @Bean
    public ProductWriter productWriter() {
        return new ProductWriter();
    }

    @Bean
    public UserWriter userWriter(){
        return new UserWriter();
    }

    @Bean
    public Job importDataJob(JobRepository jobRepository,Step step1,Step step2, Step step3, Step step4,Step step5, Step step6) {
        return new JobBuilder("importDataJob",jobRepository).incrementer(new RunIdIncrementer()).start(step1).next(step2).next(step3).next(step4).next(step5).next(step6)
                .build();
    }

    @Bean
    public Step step1(JobRepository jobRepository,JpaTransactionManager transactionManager){
        return new StepBuilder("step1", jobRepository)
        .<Review,Review> chunk(10, transactionManager)
        .reader(reviewReader1())
        .processor(reviewItemProcessor())
        .writer(reviewWriter())
        .build();
    }

    @Bean
    public Step step2(JobRepository jobRepository,JpaTransactionManager transactionManager){
        return new StepBuilder("step2", jobRepository)
        .<Product,Product> chunk(10, transactionManager)
        .reader(productReader1())
        .processor(productItemProcessor())
        .writer(productWriter())
        .build();
    }

    @Bean
    public Step step3(JobRepository jobRepository,JpaTransactionManager transactionManager){
        return new StepBuilder("step3", jobRepository)
        .<User,User> chunk(10, transactionManager)
        .reader(userReader1())
        .processor(userItemProcessor())
        .writer(userWriter())
        .build();
    }

    @Bean
    public Step step4(JobRepository jobRepository,JpaTransactionManager transactionManager){
        return new StepBuilder("step4", jobRepository)
        .<Review,Review> chunk(10, transactionManager)
        .reader(reviewReader2())
        .processor(reviewItemProcessor())
        .writer(reviewWriter())
        .build();
    }

    @Bean
    public Step step5(JobRepository jobRepository,JpaTransactionManager transactionManager){
        return new StepBuilder("step5", jobRepository)
        .<Product,Product> chunk(10, transactionManager)
        .reader(productReader2())
        .processor(productItemProcessor())
        .writer(productWriter())
        .build();
    }

    @Bean
    public Step step6(JobRepository jobRepository,JpaTransactionManager transactionManager){
        return new StepBuilder("step6", jobRepository)
        .<User,User> chunk(10, transactionManager)
        .reader(userReader2())
        .processor(userItemProcessor())
        .writer(userWriter())
        .build();
    }
}
