#include "TcpConnection.h"
#include "Channel.h"
#include "Socket.h"

TcpConnection::TcpConnection(EventLoop *loop,const std::string name,int sockfd,
                              const Address &localAddr,const Address &peerAddr) 
                        :loop_(loop),
                        reading_(false),
                        name_(name),
                        socket_(new Socket(sockfd)),
                        channel_(new Channel(loop,sockfd)),
                        localAddr_(localAddr),
                        peerAddr_(peerAddr) 
{
    channel_->setReadCallBack(
        std::bind(&TcpConnection::handleRead,this));
    channel_->setWriteCallBack(boost::bind(&TcpConnection::handWrite,this));
    channel_->setCloseCallBack(boost::bind(&TcpConnection::handClose,this));
    channel_->setErrCallBack(boost::bind(&TcpConnection::handClose,this));
    channel_->setForceCloseCallBack(boost::bind(&TcpConnection::forceClose,this));
}
void TcpConnection::handleRead() {
  std::cout << "TcpConnnection handleread" << std::endl;
  int n = static_cast<int>(inputBuffer_.readFd(channel_->getFd()));

  if (n > 0) {
    messageCallBack_(shared_from_this(), &inputBuffer_);
  } else if (n == 0) {
    std::cout << "client is over" << std::endl;
    handClose();
  } else {
    std::cout << "TcpConnection Error:: handRead" << std::endl;
  }
}
void TcpConnection::handWrite() {
  if (channel_->isWriteing()) {
    std::cout << "writeing" << std::endl;
    ssize_t n = ::write(channel_->getFd(), outputBuffer_.peek(),
                        outputBuffer_.getReadableBytes());
    std::cout << "writing Bytes: " << n <<std::endl;
    if (n > 0) {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.getReadableBytes() == 0) {
        channel_->disableWriteing();
        // busy Loop
        // loop_->quit
        // if (state_ == Disconnceted) Close();
      }
    }
  } else {
    std::cout << "Connection is Down" << std::endl;
  }
}
void TcpConnection::handClose() {
  std::cout << "TcpConnection handclose " << channel_->getFd() << std::endl;
  assert(state_ == Connected);
  setState(Disconnceted);
  // if(state_ == Connected) exit(0);
  channel_->disableAll();
  connectionCallBack_(shared_from_this());
}
void TcpConnection::forceClose () {
   assert(state_ == Connected);
   setState(Disconnceted);
   channel_->disableAll();
   channel_->remove();
  ::close(channel_->getFd());
}
void TcpConnection::connectionClose() {
  assert(state_ == Connected);
  channel_->disableAll();
  connectionCallBack_(shared_from_this());
  //update 2/4
  channel_->remove();
}
void TcpConnection::send(const std::string& message) {
  if (state_ == Connected) {
    sendInLoop(message);
  }
}
void TcpConnection::send(Buffer* buffer) {
  if (state_ == Connected) {
    sendInLoop(buffer->retrieveAllAsString());
  }
}
void TcpConnection::sendInLoop(const std::string& message) {
  ssize_t nwrite = 0;
  std::cout << "Sendinloop:: Messgae : " << message << std::endl;
  if (!channel_->isWriteing() && outputBuffer_.getReadableBytes() == 0) {
    nwrite = ::write(channel_->getFd(), message.c_str(), message.size());
    if (nwrite >= 0) {
      if (boost::implicit_cast<size_t>(nwrite) < message.size()) {
        std::cout << "More Datas are going to be Writen" << std::endl;
      }
    } else {
      nwrite = 0;
      if (errno != EWOULDBLOCK) {
        std::cout << "TcpConnection::sendInLoop: " << std::endl;
      }
    }
  }
  assert(nwrite >= 0);
  if (boost::implicit_cast<size_t>(nwrite) < message.size()) {
    outputBuffer_.Append(message.c_str() + nwrite, message.size() - nwrite);
    if (!channel_->isWriteing()) {
      channel_->enableWriteing();
      std::cout << "More Data Senting Has Been Started" << std::endl;
    }
  }
}
void TcpConnection::Close() {
  if (state_ == Connected) {
    setState(Disconnceted);
     if (!channel_->isWriteing()) socket_->Close();
  }
}
void TcpConnection::connectEstablished() {
  setState(Connected);
  channel_->enableReading();
  connectionCallBack_(shared_from_this());
}
