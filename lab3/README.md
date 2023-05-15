# Lab 3

## Compulsory part (必做部分)

For the compulsory part, we completed `server.c`, which employs the POSIX threads (pthreads) execution model.

### Concurrent request handling (并发请求处理)

We designed a thread pool with the `pthread.h` library,
which will be introduced in the [Optional part](#optional-part-选做部分) section.

### Parse HTTP header (解析和检验 HTTP 头)

The `parse_request()` function implements this functionality.
It first compares the first five characters with `"GET /"`, since HTTP method names (GET, POST, etc.) are case-sensitive ([source](https://datatracker.ietf.org/doc/html/rfc9110#section-9.1)).
Then, it extracts the characters between the first two spaces as the path of the requested file.
If the length of path is greater than PATH_MAX, it will return -1 (error).

### Handle request (实现读取请求资源内容)

The `handle_clnt()` function implements this functionality.
Basically, it can be divided into three individual components.

#### `read()` request

The `read()` function is called repeatedly until we get a `"\r\n\r\n"` in the string.

#### Try to `open()` file

We first test for the existence of the required file,
and then examine whether a [directory traversal attack](https://en.wikipedia.org/wiki/Directory_traversal_attack) is made.
Server will respond `404 Not Found` if either is failed.
If both two tests passed, and the file is a regular file, we will `open()` the file with `O_RDONLY` mode.

#### `write()` respond and `sendfile()`

We first `write()` the respond corresponding to the status code, *without* the `O_NONBLOCK` flag.
[POSIX standard](https://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html) guarantees a *complete* write.
If the status is `200 OK`, we also need to output the content of the requested files,
and this is done by `sendfile()`, which are faster than `read()` from the source and then `write()` to the target.

> Please note that `sendfile()` can transfer at most 0x7ffff000 (2,147,479,552) bytes, which is about 2 Gigabyte.
> Use `ssize_t sendfile64(int out_fd, int in_fd, off64_t * offset, size_t count)` if larger files need to be handled.

**注意，`sendfile()` 最多只能传输大约 2 GB，如果需要处理更大的文件，请使用 `sendfile64()` 函数。**（可能需要 64 位 Linux）。

### Error handling (实现错误和异常处理)

We define two function-like macros to handle errors.
The first is `errExit()`, it will `fputs()` the error message to stderr and teminate the server.
The second is `perrExit()`, it will `perror()` the error message and terminate the server, it is called when errno is set.

## Optional part (选做部分)

## Testing (测试)

### `server.c`

### `server_optional.c`
