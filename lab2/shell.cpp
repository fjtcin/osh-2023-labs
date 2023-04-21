// IO
#include <iostream>
// std::string
#include <string>
// std::vector
#include <vector>
// std::string 转 int
#include <sstream>
// PATH_MAX 等常量
#include <climits>
// POSIX API
#include <unistd.h>
// wait
#include <sys/wait.h>
//getenv
#include <stdlib.h>

void exec(std::string);
void exec_pipe(std::vector<std::string>);
std::vector<std::string> split(std::string, const std::string&);
std::vector<std::string> get_args(std::string);

int main() {
  // 不同步 iostream 和 cstdio 的 buffer
  std::ios::sync_with_stdio(false);

  // 用来存储读入的一行命令
  std::string cmd;
  while (true) {
    // 打印提示符
    std::cout << "# ";

    // 读入一行。std::getline 结果不包含换行符。
    std::getline(std::cin, cmd);

    std::vector<std::string> cmd_list = split(cmd, "|");
    if (cmd_list.size() > 1) {
      exec_pipe(cmd_list);
    } else {
      exec(cmd_list[0]);
    }
  }
}

void exec(std::string cmd) {
  // 按空格分割命令为单词
  std::vector<std::string> args = get_args(cmd);

  // 没有可处理的命令
  if (args.empty()) {
    return;
  }

  // 退出
  if (args[0] == "exit") {
    if (args.size() <= 1) {
      exit(0);
    }

    // std::string 转 int
    std::stringstream code_stream(args[1]);
    int code = 0;
    code_stream >> code;

    // 转换失败
    if (!code_stream.eof() || code_stream.fail()) {
      std::cerr << "exit: invalid exit code\n";
      return;
    }

    exit(code);
  }

  if (args[0] == "pwd") {
    if (args.size() > 1) {
      std::cerr << "pwd: too many arguments\n";
      return;
    }
    char buf[PATH_MAX];
    if (getcwd(buf, PATH_MAX)) {
      std::cout << buf << '\n';
    } else {
      std::cerr << "pwd: failed\n";
    }
    return;
  }

  if (args[0] == "cd") {
    if (args.size() > 2) {
      std::cerr << "cd: too many arguments\n";
      return;
    }
    if (args.size() <= 1) {
      if (chdir(getenv("HOME")))
        std::cerr << "cd: failed\n";
    } else {
      if (chdir(args[1].c_str()))
        std::cerr << "cd: no such file or directory: " << args[1] << '\n';
    }
    return;
  }

  // 处理外部命令
  pid_t pid = fork();

  // std::vector<std::string> 转 char **
  char *arg_ptrs[args.size() + 1];
  for (auto i = 0; i < args.size(); i++) {
    arg_ptrs[i] = &args[i][0];
  }
  // exec p 系列的 argv 需要以 nullptr 结尾
  arg_ptrs[args.size()] = nullptr;

  if (pid == 0) {
    // 这里只有子进程才会进入
    // execvp 会完全更换子进程接下来的代码，所以正常情况下 execvp 之后这里的代码就没意义了
    // 如果 execvp 之后的代码被运行了，那就是 execvp 出问题了
    execvp(args[0].c_str(), arg_ptrs);

    // 所以这里直接报错
    exit(255);
  }

  // 这里只有父进程（原进程）才会进入
  if (wait(nullptr) < 0) std::cerr << "wait failed\n";
}

void exec_pipe(std::vector<std::string> cmd_list) {
  int num = cmd_list.size(), last_fd0;
  for (int i = 0; i < num; i++) {
    int fd[2];
    if (i < num - 1 && pipe(fd) < 0) {
      std::cerr << "pipe failed\n";
      return;
    }
    pid_t pid = fork();
    if (pid < 0) {
      std::cerr << "fork failed\n";
      return;
    }
    if (pid == 0) {
      if (i > 0) dup2(last_fd0, 0);
      if (i < num - 1) dup2(fd[1], 1);
      exec(cmd_list[i]);
      exit(0);
    }
    if (wait(nullptr) < 0) std::cerr << "wait failed\n";
    if (i > 0 && close(last_fd0)) {
      std::cerr << "close failed\n";
      return;
    }
    if (i < num - 1) {
      if (close(fd[1])) {
        std::cerr << "close failed\n";
        return;
      }
      last_fd0 = fd[0];
    }
  }
}

// 经典的 cpp string split 实现
// https://stackoverflow.com/a/14266139/11691878
std::vector<std::string> split(std::string s, const std::string &delimiter) {
  std::vector<std::string> res;
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    res.push_back(token);
    s = s.substr(pos + delimiter.length());
  }
  res.push_back(s);
  return res;
}

std::vector<std::string> get_args(std::string s) {
  std::vector<std::string> res;
  std::string buf;
  std::stringstream ss(s);
  while (ss >> buf) res.push_back(buf);
  return res;
}
