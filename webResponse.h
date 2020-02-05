#ifndef _RESPONSE_H_
#define _RESPONSE_H_
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
// #include "Buffer.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "disCription.h"
// #include "webRequest.h"
#include "FastCGI/FastCGI.h"

using namespace std;

const std::string Version = "HTTP/1.1";

class webResponse : public disCription {
 public:
  const std::string Ok = "ok";
  const std::string _400 = "Bad Requeest";
  const std::string _403 = "Forbidden";
  const std::string _404 = "Not Found";
  const std::string _500 = "Internal Error";
  HttpCode requestAction(void);
  void fileResponseAddHead(unique_ptr<Buffer>&buffer_, int length_);
  void fileResponseAddHead(unique_ptr<Buffer>&buffer_, std::string &cgiReply_);
  bool fileResponseAssembly(unique_ptr<Buffer>&buffer_);
  void setHttpCodeStatus(HttpCode &status) { httpcodestatus_ = status; }
  
  std::string getFileType();
  
  ~webResponse() {
    if (fileAddr) munmap(fileAddr, st_.st_size);
    flagsAddr = NULL;
  }
  static char *fileAddr;
  static char *flagsAddr;
  static int count_;
  static int tail_;

 private:
  // webRequest request_;
  char buf_[64];
  HttpCode httpcodestatus_;
  std::string title_;
  std::string cgiStatus_;
  std::string cgiContentType_;
  std::string cgiContent_;
};

#endif