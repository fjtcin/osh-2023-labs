#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/epoll.h>

#define BIND_IP_ADDR "127.0.0.1"
#define BIND_PORT 8000
#define MAX_RECV_LEN 1048576
#define MAX_CONN 256
#define MAX_EVENTS 256

#define HTTP_STATUS_200 "200 OK"
#define HTTP_STATUS_404 "404 Not Found"
#define HTTP_STATUS_500 "500 Internal Server Error"

#define errExit(msg) do {fputs(msg, stderr); fputc('\n', stderr); exit(EXIT_FAILURE);} while (0)
#define perrExit(msg) do {perror(msg); exit(EXIT_FAILURE);} while (0)

void handle_clnt(int);
int parse_request(char*, size_t, char*);
void write_s(int, char*, size_t);

int main(void) {
  int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serv_sock < 0) perrExit("socket() failed");

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(BIND_PORT);
  serv_addr.sin_addr.s_addr = inet_addr(BIND_IP_ADDR);

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) perrExit("bind() failed");
  if(listen(serv_sock, MAX_CONN) < 0) perrExit("listen() failed");

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size = sizeof(clnt_addr);

  struct epoll_event event, *events = malloc(sizeof(struct epoll_event) * MAX_EVENTS);
  if (events == NULL) perrExit("malloc() failed");
  int epfd = epoll_create1(0);
  if (epfd < 0) perrExit("epoll_create1() failed");
  while (1) {
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock < 0) perrExit("accept() failed");
    event.data.fd = clnt_sock;
    event.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event) < 0) perrExit("epoll_ctl() failed");
    int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
    if (n < 0) perrExit("epoll_wait() failed");
    for (int i = 0; i < n; ++i) handle_clnt(events[i].data.fd);
  }
}

void handle_clnt(int clnt_sock) {
  char *req_buf = malloc(MAX_RECV_LEN);
  if (req_buf == NULL) perrExit("malloc() failed");
  size_t req_len = 0;
  req_buf[0] = '\0';
  char *buffer = malloc(MAX_RECV_LEN);
  if (buffer == NULL) perrExit("malloc() failed");
  while (1) {
    ssize_t len = read(clnt_sock, buffer, MAX_RECV_LEN);
    if (len < 0) perrExit("read() failed");
    strcat(req_buf, buffer);
    req_len = req_len + len;
    if (req_len >= 4 && !strncmp(req_buf + req_len - 4, "\r\n\r\n", 4)) break;
  }

  char *file_path = malloc(PATH_MAX);
  if (file_path == NULL) perrExit("malloc() failed");
  int fd;
  int status = 0;
  struct stat stats;
  if (parse_request(req_buf, req_len, file_path)) {
    status = 500;
  } else {
    if (access(file_path, F_OK) < 0) {
      status = 404;
    } else {
      if (stat(file_path, &stats) < 0) {
        status = 500;
      } else if (!S_ISREG(stats.st_mode)) {
        status = 500;
      } else {
        fd = open(file_path, O_RDONLY);
        if (fd < 0) {
          status = 500;
        } else {
          status = 200;
        }
      }
    }
  }

  char* response = malloc(64);
  if (response == NULL) perrExit("malloc() failed");
  switch (status) {
    case 200: {
      sprintf(response, "HTTP/1.0 %s\r\nContent-Length: %zu\r\n\r\n", HTTP_STATUS_200, stats.st_size);
      size_t response_len = strlen(response);
      write_s(clnt_sock, response, response_len);
      off_t offset = 0;
      while (offset < stats.st_size) {
        if (sendfile(clnt_sock, fd, &offset, stats.st_size - offset) < 0)
          perrExit("sendfile() failed");
      }
      break;
    }
    case 404: {
      sprintf(response, "HTTP/1.0 %s\r\n\r\n", HTTP_STATUS_404);
      size_t response_len = strlen(response);
      write_s(clnt_sock, response, response_len);
      break;
    }
    default:  {
      sprintf(response, "HTTP/1.0 %s\r\n\r\n", HTTP_STATUS_500);
      size_t response_len = strlen(response);
      write_s(clnt_sock, response, response_len);
    }
  }

  if (close(clnt_sock)) perrExit("close() failed");
  free(req_buf);
  free(buffer);
  free(file_path);
  free(response);
}

int parse_request(char* req, size_t req_len, char* path) {
  if (strncmp(req, "GET /", 5)) return -1;
  path[0] = '.';
  size_t s2 = 1;
  for (; 3 + s2 < req_len && req[3 + s2] != ' '; ++s2) {
    if (s2 == PATH_MAX - 1) return -1;
    path[s2] = req[3 + s2];
  }
  path[s2] = '\0';
  return 0;
}

void write_s(int fd, char* buf, size_t len) {
  if (!len) return;
  if (write(fd, buf, len) != len) perrExit("write() failed");
}
