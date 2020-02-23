#include "TcpConnection.h"

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

TcpConnection::TcpConnection(EventLoop* loop, const std::string name,
                             int sockfd, const Address& localAddr,
                             const Address& peerAddr)
    : loop_(loop),
      reading_(false),
      name_(name),
      socket_(new Socket(sockfd)),
      channel_(new Channel(loop, sockfd)),
      localAddr_(localAddr),
      peerAddr_(peerAddr) {
  channel_->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
  channel_->setWriteCallBack(std::bind(&TcpConnection::handWrite, this));
  channel_->setCloseCallBack(std::bind(&TcpConnection::handClose, this));
  channel_->setErrCallBack(std::bind(&TcpConnection::handClose, this));
}
void TcpConnection::handleRead() {
  loop_->assertNotInLoopThread();
  int n = static_cast<int>(inputBuffer_.readFd(channel_->getFd()));

  if (n > 0) {
    messageCallBack_(shared_from_this(), &inputBuffer_);
  } else if (n == 0) {
    connectionClose();
  } else {
    std::cout << "TcpConnection Error:: handRead" << std::endl;
  }
}
void TcpConnection::handWrite() {
  loop_->assertNotInLoopThread();
  if (channel_->isWriteing()) {
    ssize_t n = ::write(channel_->getFd(), outputBuffer_.peek(),
                        outputBuffer_.getReadableBytes());
    if (n > 0) {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.getReadableBytes() == 0) {
        channel_->disableWriteing();
        // if (state_ == Connected) Close();
      }
    }
  } else {
    std::cout << "Connection is Down" << std::endl;
  }
}
void TcpConnection::handClose() {
  loop_->assertNotInLoopThread();
  assert(state_ == Connected);
  channel_->disableAll();
  connectionCallBack_(shared_from_this());
}
void TcpConnection::handError() {
  cout << "Error: " << strerror(errno) << endl;
}
void TcpConnection::connectionClose() {
  loop_->assertNotInLoopThread();
  // assert(state_ == Connected);
  setState(Disconnceted);
  channel_->disableAll();
  connectionCallBack_(shared_from_this());
  loop_->removeChannel(channel_.get());
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
  loop_->assertNotInLoopThread();
  ssize_t nwrite = 0;
  if (!channel_->isWriteing() && outputBuffer_.getReadableBytes() == 0) {
    nwrite = ::write(channel_->getFd(), message.c_str(), message.size());
    if (nwrite >= 0) {
      if (static_cast<size_t>(nwrite) < message.size()) {
        // std::cout << "waiting for be sent " << std::endl;
      }
    } else {
      nwrite = 0;
      if (errno != EWOULDBLOCK) {
        // std::cout << "TcpConnection::sendInLoop: " << std::endl;
      }
    }
  }
  assert(nwrite >= 0);
  if (static_cast<size_t>(nwrite) < message.size()) {
    outputBuffer_.Append(message.c_str() + nwrite, message.size() - nwrite);
    if (!channel_->isWriteing()) {
      channel_->enableWriteing();
    }
  }
}
void TcpConnection::Close() {
  if (state_ == Connected) {
    setState(Disconnceted);
    loop_->runInLoop(std::bind(&TcpConnection::safeClose, this));
  }
}
void TcpConnection::safeClose() {
  loop_->assertNotInLoopThread();
  if (!channel_->isWriteing()) close(channel_->getFd());
}
void TcpConnection::connectEstablished() {
  // cout << "established------" << endl;
  loop_->assertNotInLoopThread();
  setState(Connected);
  channel_->tie(shared_from_this());
  channel_->enableReading();
  connectionCallBack_(shared_from_this());
}
