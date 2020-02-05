#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_
#include "modelHead.h"
#include "Address.h"
#include "Buffer.h"

class webRequest;
class Socket;
class EventLoop;
class Channel;

using namespace std;

class TcpConnection : 
            public std::enable_shared_from_this<TcpConnection>
{
    public:     
        TcpConnection(EventLoop *loop,const std::string name,int sockfd,
                              const Address &localAddr,const Address &peerAddr);
        void setConnectionCallBack(ConnectionCallBack &cb) {
            connectionCallBack_ = cb;
        }
        void setMessageCallBack(const MessageCallBack &cb) {
            messageCallBack_ = cb;
        }
        void setCloseCallBack(const CloseCallBack &cb) {
            closeCallBack_ = cb;
        }
        void setContext (boost::any const& context) {
            context_ = context;
        }
        const boost::any& getContext() const {
            return context_;
        }
        const std::string & getName() { return name_; }
        bool isConnected() { return state_ == Connected; }
        bool isDisconnected() { return state_ == Disconnceted; }
        void send(const std::string& message);
        void send(Buffer *buffer);
        void Close();
        void startRead();
        bool reading() { return reading_; }
        void stopRead();
        void connectEstablished();
        EventLoop* getLoop() { return loop_;}
        void handClose();
        void forceClose();
        void connectionClose();
        void setRequest(std::shared_ptr<webRequest>request) { 
            request_ = request;
        }
        std::shared_ptr<webRequest> getRequest()  { return request_; }
    private:
        enum State { Connecting,Connected,Disconnceted};
        EventLoop *loop_;
        bool reading_;
        std::string name_;
        State state_;
        std::unique_ptr<Socket> socket_;
        std::unique_ptr<Channel> channel_;
        Address localAddr_;
        Address peerAddr_;
        ConnectionCallBack connectionCallBack_;
        MessageCallBack messageCallBack_;
        CloseCallBack closeCallBack_;
        CloseCallBack forceCloseCallBack_;
        Buffer inputBuffer_;
        Buffer outputBuffer_;
        std::shared_ptr<webRequest>request_;   
        boost::any context_;
        // ErrCallBack errCallBack_;
        void setState(State s) { state_ = s;}
        void handleRead();
        void handWrite();
        void sendInLoop(const std::string& message);
        // void Shutdown();
        void shutdownInLoop();
        

      
};
#endif