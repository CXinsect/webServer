### 介绍
 - 这是一个由C++语言编写的基于事件驱动模型的Web服务器，实现了基本的GET、POST请求和动态网页的处理。
 
### 特点
 - 基于Reactor + 多线程 + 非阻塞I/O模型设计。
 - 线程模型采用one loop per thread的思想，即每一个线程维护了一个事件循环，负责IO的处理，一定程度上减轻了主线程的压力。
 - 维护状态机解析HTTP协议。
 - 使用FastCGI实现对动态网页的请求。
 
### 具体技术
 - 使用**mmap**读取本地文件，减少了不必要的数据拷贝操作，提高了IO效率。
 - 使用**readv**读取输入缓冲区中的数据，减少了当网络中传输大量数据时触发回调的次数。
 - 使用智能指针管理对象的生命周期。避免内存泄露和空闲指针情况的发生。

### 运行环境
 ![](https://github.com/CXinsect/webServer/blob/master/images/hardware.png)
### 性能测试
 - WebBench的使用
 ```
 webbench -c 客户端数 -t 时间　http://127.0.0.1:8888/home/insect/
 ```
 - 使用WebBench的测试结果：
   - one client
   
       Appache: 
       ![](https://github.com/CXinsect/webServer/blob/master/images/appache1c10.png)
       
       Server:
       ![](https://github.com/CXinsect/webServer/blob/master/images/server1c10.png)
   
   - two clients
   
      Appache:
      ![](https://github.com/CXinsect/webServer/blob/master/images/appache4c10.png)
    
      Server:
      ![](https://github.com/CXinsect/webServer/blob/master/images/server4c10.png)
   
   - eight clients
   
      Appache:
      ![](https://github.com/CXinsect/webServer/blob/master/images/appache100c10.png)
      
      Server:
      ![](https://github.com/CXinsect/webServer/blob/master/images/server8c10.png)
    
   - 100 clients
   
      Appache:
      ![](https://github.com/CXinsect/webServer/blob/master/images/http100c10.png)
    
      Server:
      ![](https://github.com/CXinsect/webServer/blob/master/images/server100c10.png)
   
- 绘制成图表如下
 ![](https://github.com/CXinsect/webServer/blob/master/images/servercompare.png)
 
### 安装和运行
 运行前请安装cmake 1.5以上版本
 ```
  debian: sudo apt-get install cmake
  redHat: sudo yum install cmake
  Arch  : sudo pacman -S cmake
 
  cmake .
  make
  ./a.out
 ```
 
 
