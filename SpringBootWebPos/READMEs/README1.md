# aw06
我的webpos项目中共有4个微服务webpos-carts、webpos-products、webpos-counter和webpos-orders，分别表示购物车服务、商品服务、柜台服务和订单服务。除了这4个微服务外，还有用于服务注册发现的webpos-discovery-server以及API网关webpos-api-gateway。

启动顺序：先启动webpos-discovery-server，再启动API网关以及4个微服务。

对4个微服务的介绍：
1. webpos-carts提供了查看所有购物车、根据购物车Id查看某一个购物车、创建一个购物车、查看某个购物车中物品的价格总和、向购物车中添加物品、从购物车中删除物品和对某个购物车进行结账这几个服务。
2. webpos-products提供了查看所有商品和根据商品Id查看某一个商品这两个服务。
3. webpos-counter提供了计算一个购物车中物品的价格总和的服务。当向webpos-carts发送查看某个购物车中物品的价格总和的服务请求时，webpos-carts会向webpos-counter发送服务请求，由webpos-counter计算出购物车中物品的价格总和这一结果，并将结果返回给webpos-carts。
4. webpos-orders提供了生成订单、查看所有订单和根据订单Id查看某一个订单这三个服务。当向webpos-carts发送对某个购物车进行结账的服务请求时，webpos-carts会向webpos-orders发送服务请求，由webpos-orders生成相应的订单，并将订单返回给webpos-carts。
5. webpos-carts在向webpos-counter和webpos-orders发送服务请求时，使用了RestTemplate进行负载均衡和断路器CircuitBreaker，具体代码可见webpos-carts/src/main/java/com/example/carts/service/CartServiceImp.java中的getCartTotal函数和checkout函数。