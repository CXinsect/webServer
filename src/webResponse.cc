#include "webResponse.h"
struct stat disCription::st_;
std::string disCription::filename_ = std::string();
std::string disCription::cgiReply_ = std::string();

char *webResponse::fileAddr = NULL;
char *webResponse::flagsAddr = NULL;
int webResponse::count_ = 0;
int webResponse::tail_ = 0;

void webResponse::fileResponseAddHead(unique_ptr<Buffer>&buffer_, int length_) {
  memset(buf_, 0, sizeof(buf_));
  snprintf(buf_, sizeof(buf_), "Content-Type: %s\r\n", getFileType().c_str());
  buffer_->Append(buf_, strlen(buf_));
  filename_.erase(0,filename_.size());
  memset(buf_, 0, sizeof(buf_));
  snprintf(buf_, sizeof(buf_), "Content-Length: %d\r\n", length_);
  buffer_->Append(buf_, strlen(buf_));
  memset(buf_, 0, sizeof(buf_));
  snprintf(buf_, sizeof(buf_), "Connection: %s\r\n",
           (link_ == true) ? "keep-alive" : "close");
  buffer_->Append(buf_, strlen(buf_));
  memset(buf_, 0, sizeof(buf_));
  snprintf(buf_, sizeof(buf_), "\r\n");
  buffer_->Append(buf_, strlen(buf_));
}
void webResponse::fileResponseAddHead(unique_ptr<Buffer>&buffer_, std::string &cgiReply_) {
  int position;
  memset(buf_, 0, sizeof(buf_));
  if ((position = cgiReply_.find("Status:")) != std::string::npos) {
    cgiReply_ = cgiReply_.substr(position + 7, cgiReply_.size());
    int pos = cgiReply_.find_first_not_of(" ");
    cgiStatus_ = cgiReply_.substr(pos, 3);
  }
  snprintf(buf_, sizeof(buf_), "%s %d %s\r\n", Version.c_str(),
           200, "discription");
  buffer_->Append(buf_, strlen(buf_));
  memset(buf_, 0, sizeof(buf_));
  if ((position = cgiReply_.find("Content-type:")) != std::string::npos) {
    cgiReply_ = cgiReply_.substr(position + 13, cgiReply_.size());
    int pos = cgiReply_.find_first_not_of(" ");
    cgiContentType_ = cgiReply_.substr(pos, 24);
    snprintf(buf_, sizeof(buf_), "Content-Type: %s\r\n",
             cgiContentType_.c_str());
    buffer_->Append(buf_, strlen(buf_));
    memset(buf_, 0, sizeof(buf_));
    int pos2 = cgiReply_.find_first_of("\n");
    int pos3 = cgiReply_.find_last_of("\n");
    cgiContent_ = cgiReply_.substr(pos2 + 2, pos3);
    snprintf(buf_, sizeof(buf_), "Content-Length: %d\r\n",
             static_cast<int>(cgiContent_.size()));
    buffer_->Append(buf_, strlen(buf_));
    memset(buf_, 0, sizeof(buf_));
    snprintf(buf_, sizeof(buf_), "\r\n");
    buffer_->Append(buf_, strlen(buf_));
  }
}
bool webResponse::fileResponseAssembly(unique_ptr<Buffer>&buffer_) {
  std::cout << "fileresponse " << std::endl;
  switch (httpcodestatus_) {
    case InternalError: {
      memset(buf_, 0, sizeof(buf_));
      snprintf(buf_, sizeof(buf_), "%s %d %s\r\n", Version.c_str(), 500,
               _500.c_str());
      buffer_->Append(buf_, strlen(buf_));
      fileResponseAddHead(buffer_, _500.size());
      const std::string emptyFile = "<html><body></body></html>";
      buffer_->Append(emptyFile.c_str(), emptyFile.size());
      return true;
    }
    case BadRequest: {
      snprintf(buf_, sizeof(buf_), "%s %d %s\r\n", Version.c_str(), 400,
               _400.c_str());
      buffer_->Append(buf_, strlen(buf_));
      fileResponseAddHead(buffer_, _400.size());
      buffer_->Append(_400.c_str(), _400.size());
      return true;
    }
    case NoResource: {
      memset(buf_, 0, sizeof(buf_));
      snprintf(buf_, sizeof(buf_), "%s %d %s\r\n", Version.c_str(), 404,
               _404.c_str());
      buffer_->Append(buf_, strlen(buf_));
      fileResponseAddHead(buffer_, _404.size());
      buffer_->Append(_404.c_str(), _404.size());
      return true;
    }
    case ForbidenRequest: {
      memset(buf_, 0, sizeof(buf_));
      snprintf(buf_, sizeof(buf_), "%s %d %s\r\n", Version.c_str(), 403,
               _403.c_str());
      buffer_->Append(buf_, strlen(buf_));
      fileResponseAddHead(buffer_, _403.size());
      buffer_->Append(_403.c_str(), _403.size());
      return true;
    }
    case FileRequest: {
      do {
        std::cout << "FileRequest" << std::endl;
        if (cgiReply_.size() != 0) {
          std::cout << " FastCGI回复 :" << cgiReply_ << std::endl;
          fileResponseAddHead(buffer_, cgiReply_);
          buffer_->Append(cgiContent_.c_str(), cgiContent_.size());
          cgiReply_.erase(0,cgiReply_.size());

          break;
        }
        memset(buf_, 0, sizeof(buf_));
        snprintf(buf_, sizeof(buf_), "%s %d %s\r\n", Version.c_str(), 200,
                 Ok.c_str());
        std::string tmp = buf_;
        buffer_->Append(tmp.c_str(), tmp.size());
        if (st_.st_size != 0) {
          fileResponseAddHead(buffer_, st_.st_size);
          int n = 0;
          std::string tmp(fileAddr, st_.st_size);
          buffer_->Append(tmp.c_str(), tmp.size());
          st_.st_size = 0;
        } else {
          const std::string emptyFile = "<html><body></body></html>";
          buffer_->Append(emptyFile.c_str(), emptyFile.size());
          buffer_->Append(fileAddr, strlen(fileAddr));
        }
      } while (0);
      return true;
    }
    default: {
      std::cout << "webResponse::fileresponse Unknown option" << std::endl;
      return false;
    }
  }
}
std::string webResponse::getFileType() {
  const char *dot;
  dot = strrchr(filename_.c_str(), '.');
  if (dot == NULL) return "text/plain; charset=utf-8";
  if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
    return "text/html; charset=utf-8";
  if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
    return "image/jpeg";
  if (strcmp(dot, ".pdf") == 0) return "application/pdf";
  if (strcmp(dot, ".png") == 0) return "image/png";
  if (strcmp(dot, ".gif") == 0) return "image/gif";
  if (strcmp(dot, ".wav") == 0) return "audio/wav";
  if (strcmp(dot, ".mp4") == 0) return "audio/wav";
  if (strcmp(dot, ".avi") == 0) return "video/x-msvideo";
  if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
    return "video/quicktime";
  if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
    return "video/mpeg";
  if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
    return "model/vrml";
  if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
    return "audio/midi";
  if (strcmp(dot, ".mp3") == 0) return "audio/mpeg";
  if (strcmp(dot, ".pac") == 0) return "application/x-ns-proxy-autoconfig";

  return "text/plain; charset=utf-8";
}
