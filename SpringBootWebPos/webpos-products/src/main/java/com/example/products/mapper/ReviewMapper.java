package com.example.products.mapper;

import java.util.Collection;

import org.mapstruct.Mapper;

import com.example.products.model.Review;
import com.example.webpos.rest.dto.ReviewDto;

@Mapper
public interface ReviewMapper {
    
    ReviewDto toReviewDto(Review review);
    Review toReview(ReviewDto reviewDto);
    Collection<ReviewDto> toReviewDtos(Collection<Review> reviews);
    Collection<Review> toReviews(Collection<ReviewDto> reviewDtos);
}
