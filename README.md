### 介绍
 - 这是一个由C++语言编写的基于事件驱动模型的Web服务器，实现了基本的GET、POST请求和动态网页的处理。
 
### 特点
 - 基于Reactor + one loop per thread + 非阻塞I/O模型设计
 - 使用状态机解析HTTP协议
 - 使用FastCGI实现对动态网页的请求
 
### 具体技术
 - 使用**mmap**读取本地文件，减少了不必要的数据拷贝操作，提高了IO效率
 - 使用**readv**读取输入缓冲区中的数据，减少了当网络中传输大量数据时触发回调的次数。
 - 使用Reactor + one loop per thread模型,主线程只负责建立连接，连接建立后会触发回调函数创建新的线程，同时在新的线程里面创建新的事件循环来处理各种IO事件
 - 使用智能指针管理对象的生命周期。
 
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
 
 
