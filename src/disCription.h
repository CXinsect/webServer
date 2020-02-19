#ifndef _DISCRIPTION_H_
#define _DISCRIPTION_H_
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

class disCription {
 public:
  enum Method { GET = 0, POST, HEAD, PUT };
  enum CheckState {
    CheckStateRequestLine = 0,
    CheckStateHeader,
    CheckStateContent
  };
  enum HttpCode {
    NoRequest,
    GetRequest,
    BadRequest,
    NoResource,
    ForbidenRequest,
    FileRequest,
    DirRequest,
    InternalError,
    ClosedConnection
  };
  enum LineStatus { LineStatusOk, LineStatusBad, LineStatusOpen };

  const std::string defaultFilePath = "/home/insect";
  std::string filePath;
  static std::string filename_;
  bool link_ = false;
  static struct stat st_;
  static std::string cgiReply_;
  const int BuffSize = 4096;
};
#endif