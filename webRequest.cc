#include "webRequest.h"
class FastCGI;

webRequest::LineStatus webRequest::parseLine() {
  char temp;
  checkIndex_ = buffer_.getDateBegin(-1);
  std::cout << "RequestBuffer: " << requeseBuffer_.size() << std::endl;
  assert(requeseBuffer_.size() != 0);
  messageLength_ = requeseBuffer_.size() + 1;
  for (; checkIndex_ < messageLength_; checkIndex_++) {
    temp = requeseBuffer_[checkIndex_];
    if (temp == '\r') {
      if (checkIndex_ + 1 == messageLength_)
        return LineStatusOpen;
      else if (requeseBuffer_[checkIndex_ + 1] == '\n') {
        requeseBuffer_[checkIndex_++] = '\0';
        requeseBuffer_[checkIndex_++] = '\0';
        return LineStatusOk;
      }
      return LineStatusBad;
    } else if (temp == '\n') {
      if (checkIndex_ > 1 && requeseBuffer_[checkIndex_ - 1] == '\r') {
        requeseBuffer_[checkIndex_ - 1] = '\0';
        requeseBuffer_[checkIndex_++] = '\0';
        return LineStatusOk;
      }
      return LineStatusBad;
    }
  }
  return LineStatusOpen;
}
webRequest::HttpCode webRequest::parseRequestLine(std::string& text) {
  std::cout << "webRequest::parseRequestLine coming in" << std::endl;
  int pos = text.find_first_of(" \t");
  std::string method = text.substr(0, pos);
  std::cout << "webRequest::method " << method << std::endl;
  methodcgi = method.substr(0, method.size());
  if (method == "GET") {
    method_ = GET;
  } else if (method == "POST")
    method = POST;
  else
    return BadRequest;
  url_ = text.substr(pos + 1, text.size());
  text = text.substr(pos + 1, text.size());
  int pos2 = url_.find_first_of(" ");
  version_ = url_.substr(pos2 + 1, url_.size());
  text = text.substr(pos2 + 1, text.size());
  url_ = url_.substr(0, pos2);
  cgiUrl_ = url_;
  if (url_.size() == 0) return BadRequest;
  version_ = version_.substr(0, 8);
  text = text.substr(8 + 2, text.size());
  if (version_.size() == 0) return BadRequest;
  if (version_ != "HTTP/1.1") return BadRequest;
  if (strncasecmp(url_.c_str(), "http://", 7) == 0) {
    url_ = url_.substr(6, url_.size());
  }
  if (url_.size() == 0) return BadRequest;
  checkstate_ = CheckStateHeader;

  return NoRequest;
}
webRequest::HttpCode webRequest::parseHeader(std::string& text) {
  int position;
  if (text[0] == '0') {
    if (contentLength_ != 0) {
      checkstate_ = CheckStateContent;
      return NoRequest;
    }
    return GetRequest;
  }
  if ((position = text.find("Host:")) != std::string::npos) {
    text = text.substr(position+5, text.size());
    int pos = text.find_first_not_of(' ');
    int pos2 = text.find("Connection");
    std::string tmp = text.substr(pos, pos2 - 3);
    text = text.substr(pos2, text.size());
    host_ = tmp;

    std::cout << "Request::host_: " << host_ << std::endl;
  }
  if ((position = text.find("Content-Length:")) != std::string::npos) {
    text = text.substr(position + 15, text.size());
    int pos = text.find_first_not_of(' ');
    int pos2 = text.find_first_of('\r');
    std::string tmpcontent = text.substr(pos, pos2 - 1);
    text = text.substr(pos2 + 2, text.size());
    contentlen_ = tmpcontent;
    contentLength_ = atol(tmpcontent.c_str());
    std::cout << "Post::Content-Length: " << contentLength_ << std::endl;
  }
  if ((position = text.find("Connection:")) != std::string::npos) {
    text = text.substr(position+11, text.size());
    int pos = text.find_first_not_of(' ');
    std::string tmpcontent = text.substr(pos, pos + 9);
    text = text.substr(pos + 12, text.size());
    if (strcasecmp(tmpcontent.c_str(), "keep-alive") == 0) link_ = true;
  } else {
    std::cout << "Temporarily unprocessed" << std::endl;
  }
  checkstate_ = CheckStateContent;
  return NoRequest;
}
webRequest::HttpCode webRequest::parseContext(std::string& text) {
  if (buffer_.getReadableBytes() == 0) {
    std::cout << "处理body" << std::endl;
    int pos = text.find_last_of('\n');
    postContent_ = text.substr(pos + 1, text.size());
    //默认采取禁止访问暂取消验证
    // if(postContent_.size() != 0)
    return GetRequest;
  }
  return NoRequest;
}
webRequest::HttpCode webRequest::requestAction() {
  filePath = url_;
  int pos = url_.find_last_of('/');
  int pos2 = url_.find_first_of('?');
  if (pos2 != std::string::npos) {
    cgiUrl_ = cgiUrl_.substr(0, pos2);
    cgiGetMessage_ = url_.substr(pos2 + 1, url_.size());
    url_ = url_.substr(0, pos2);
  }
  filename_ = url_.substr(pos + 1, url_.size());
  std::cout << "Filename: " << filename_ << std::endl;
  if (filename_.find(".php") != string::npos) {
    signal(SIGPIPE, SIG_IGN);
    fastcgi_.setRequestId(1);
    fastcgi_.startConnect();
    fastcgi_.sendStartRequestRecord();
    std::cout << "test" << fastcgi_.getSockfd() << std::endl;
    std::cout << "cgiUrl_" << cgiUrl_ << std::endl;
    fastcgi_.sendParams(const_cast<char*>("SCRIPT_FILENAME"),
                        const_cast<char*>(cgiUrl_.c_str()));
    fastcgi_.sendParams(const_cast<char*>("REQUEST_METHOD"),
                        const_cast<char*>(methodcgi.c_str()));
    if (methodcgi == "GET")
      fastcgi_.sendParams(const_cast<char*>("CONTENT_LENGTH"),
                          const_cast<char*>("0"));
    else
      fastcgi_.sendParams(const_cast<char*>("CONTENT_LENGTH"),
                          const_cast<char*>(contentlen_.c_str()));
    fastcgi_.sendParams(const_cast<char*>("CONTENT_TYPE"),
                        const_cast<char*>("application/x-www-form-urlencoded"));
    if (methodcgi == "GET")
      fastcgi_.sendParams(const_cast<char*>("QUERY_STRING"),
                          const_cast<char*>(cgiGetMessage_.c_str()));
    fastcgi_.sendEndRequestRecord();
    if (contentLength_ != 0) {
      FCGI_Header begin = fastcgi_.makeHeader(
          FCGI_STDIN, fastcgi_.getrequestId(), contentLength_, 0);
      int ret = send(fastcgi_.getSockfd(), &begin, sizeof(begin), 0);

      send(fastcgi_.getSockfd(), const_cast<char*>(contentlen_.c_str()),
           contentLength_, 0);
      FCGI_Header end =
          fastcgi_.makeHeader(FCGI_STDIN, fastcgi_.getrequestId(), 0, 0);
      send(fastcgi_.getSockfd(), &end, sizeof(end), 0);
    }
    disCription::cgiReply_ = fastcgi_.readFromPhp();
    fastcgi_.FastCgi_destory();
    std::cout << "FastCGI has been sent " << cgiGetMessage_ << std::endl;
    return FileRequest;
  }
  if (stat(filePath.c_str(), &st_) < 0) return NoResource;
  if (!(st_.st_mode & S_IROTH)) return ForbidenRequest;
  if (S_ISDIR(st_.st_mode)) {
    //处理目录请求
    return DirRequest;
  }
  int fd = open(filePath.c_str(), O_RDONLY);
  assert(fd != -1);
  webResponse::fileAddr =
      (char*)mmap(NULL, st_.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  assert(webResponse::fileAddr != MAP_FAILED);
  ::close(fd);
  return FileRequest;
}
webRequest::HttpCode webRequest::eventProcess(FastCGI& fastcgi) {
  LineStatus linestatus = LineStatusOk;
  disCription::HttpCode httpcode = NoRequest;
  std::string text = std::string();
  fastcgi_ = fastcgi;
  while (((checkstate_ == CheckStateContent) && (linestatus == LineStatusOk)) ||
         ((linestatus = parseLine()) == LineStatusOk)) {
    cout << "Coming in eventProcess" << std::endl;
    startLine_ = buffer_.getDateBegin(-1);
    std::cout << "The date in Readbuffer: " << requeseBuffer_ << std::endl;
    switch (checkstate_) {
      case CheckStateRequestLine: {
        std::cout << "First Comming" << std::endl;
        httpcode = parseRequestLine(requeseBuffer_);
        if (httpcode == BadRequest) return BadRequest;
        break;
      }
      case CheckStateHeader: {
        httpcode = parseHeader(requeseBuffer_);
        if (httpcode == BadRequest)
          return BadRequest;
        else if (httpcode == GetRequest)
          ;
        //处理请求
        break;
      }
      case CheckStateContent: {
        checkstate_ = CheckStateRequestLine;
        httpcode = parseContext(requeseBuffer_);
        if (httpcode == GetRequest)
          return requestAction();
        else if (httpcode == ForbidenRequest)
          return ForbidenRequest;
        linestatus = LineStatusOpen;
        break;
      }
      default:
        return InternalError;
    }
  }
  return NoRequest;
}