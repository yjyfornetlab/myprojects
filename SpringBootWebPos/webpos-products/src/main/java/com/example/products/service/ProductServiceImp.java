package com.example.products.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;

import com.example.products.model.Product;
import com.example.products.model.Review;
import com.example.products.repository.ProductRepository;
import com.example.products.repository.ReviewRepository;

import java.util.List;
import java.util.Optional;

@Component
// @EnableDiscoveryClient
@Service
public class ProductServiceImp implements ProductService {

    // private PosDB posDB;
    private ProductRepository productRepository;
    private ReviewRepository reviewRepository;
    // private CartRepository cartRepository;
    // private ItemRepository itemRepository;


    // @Autowired
    // public void setPosDB(PosDB posDB) {
    //     this.posDB = posDB;
    // }
    // public PosServiceImp(){
    
    // }

    @Autowired
    public void setProductRepository(ProductRepository productRepository){
        this.productRepository = productRepository;
        
    }

    @Autowired
    public void setReviewRepository(ReviewRepository reviewRepository){
        this.reviewRepository = reviewRepository;
    }
    // @Autowired
    // public void setCartRepository(CartRepository cartRepository){
    //     this.cartRepository = cartRepository;
    // }

    // @Autowired
    // public void setItemRepository(ItemRepository itemRepository){
    //     this.itemRepository = itemRepository;
    // }

    // @Override
    // public void saveCart(Cart cart){
    //     this.cartRepository.save(cart);
    // }

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

    // @Override
    // public Cart findCartById(Integer cartId){
    //     Optional<Cart> cartOp = cartRepository.findById(cartId);
    //     if(cartOp.isPresent()){
    //         return cartOp.get();
    //     }
    //     return null;
    // }

    // @Override
    // public List<Cart> carts(){
    //     return cartRepository.findAll();
    // }

    // @Override
    // public Cart newCart() {
    //     // return posDB.saveCart(new Cart());
    //     return cartRepository.save(new Cart());
    // }

    // @Override
    // public void checkout() {
    //     this.emptyCart();
    // }

    // @Override
    // public void total(Cart cart) {
    //     if(cart==null){
    //         cart = this.getCart();
    //     }
    //     cart.updateTotal();
    //     cartRepository.save(cart);
    // }

    // @Override
    // public boolean add(Product product, int amount) {
    //     return false;
    // }

    // @Override
    // public boolean add(String productId, int amount) {

    //     // Product product = posDB.getProduct(productId);
    //     Optional<Product> productOp = productRepository.findById(productId);
    //     // if (product == null)
    //     //     return false;
    //     if(!productOp.isPresent()){
    //         return false;
    //     }

    //     Cart cart = this.getCart();
    //     cart.addItem(new Item(productOp.get(), amount,cart));
    //     this.total(cart);
    //     return true;
    
    // }

    // @Override
    // public double getCartTotal(Integer cartId){
    //     Cart cart = this.findCartById(cartId);
    //     return cart.calculateTotal();
    // }

    // @Override
    // public boolean add(Item item, Integer cartId){
    //     Cart cart = this.findCartById(cartId);
    //     if(cart==null){
    //         return false;
    //     }
    //     cart.addItem(item);
    //     return true;
    // }

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
    
    // @Override
    // public boolean deleteItem(Item item,Integer cartId){
    //     Optional<Item> itemOp = this.itemRepository.findById(item.getId());
    //     Optional<Cart> cartOp = this.cartRepository.findById(cartId);
    //     if(itemOp.isPresent() && cartOp.isPresent()){
    //         Cart cart = cartOp.get();
    //         cart.removeItem(itemOp.get());
    //         itemRepository.deleteById(item.getId());
    //         // this.total(cart);
    //         cartRepository.save(cart);
    //         return true;
    //     }
    //     return false;
    // }

    @Override
    public List<Product> products() {
        // return posDB.getProducts();
        // List<Product> products = productRepository.findAll();
        // System.out.println(products.get(0));
        return productRepository.findAll();
    }

    @Override
    public Product findProductById(Integer productId){
        Optional<Product> productOp = productRepository.findById(productId);
        if(productOp.isPresent()){
            return productOp.get();
        }
        return null;
    }

    @Override
    public List<Review> getReviews(Integer productId){
        Product product = this.findProductById(productId);
        List<Review> reviews = reviewRepository.findByParentAsin(product.getParentAsin());
        // List<Review> reviews = reviewRepository.findAll();
        return reviews;
    }
}
