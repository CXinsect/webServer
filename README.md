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

### 测试与反思
 - WebBench的使用
 ```
 webbench -c 客户端数 -t 时间　http://127.0.0.1:8888/home/insect/
 ```
 - 使用WebBench的测试结果：
 ![](https://github.com/CXinsect/webServer/blob/master/images/server1c5.png)
 ![](https://github.com/CXinsect/webServer/blob/master/images/server2c10.png)
 ![](https://github.com/CXinsect/webServer/blob/master/images/server3c15.png)
 ![](https://github.com/CXinsect/webServer/blob/master/images/server4c20.png)
 
 - 反思
   - 随着客户端数量的增加，服务器处理请求的数量也随之增加，但是每次增长的幅度并不是很大，主要原因在于服务端所采用的编程模型。主线程只负责建立新连接，
  但是连接上事件的处理则交由线程去处理，每处理一个请求的同时需要创建一个对象来管理事件，这样带来的开销对于短连接来说并不划算，系统的资源会部分的浪费在
  对象的创建上，这导致了连接的响应在一定程度上有所延迟，最终导致了性能瓶颈。
 
### 安装和运行
 运行前请安装cmake 1.5以上版本
 ```
  debian: sudo apt-get install cmake
  redHat: sudo yum install cmake
  Arch  : sudo pacman -S cmake
 
  mkdir build
  cd build
  cmake .
  make
  ./Server
 ```
 
 
