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
// std::size_t, std::getenv
#include <cstdlib>
// open
#include <fcntl.h>
// file mode bits
#include <sys/stat.h>
// signal processing
#include <csignal>
#include <csetjmp>
// std::remove_if
#include <algorithm>

struct sigaction action;
const std::string STATE[3] = {"Running  ", "Done     ", "Suspended"};
struct job_t {
  size_t jobid;
  pid_t pid;
  size_t stateid;
  std::string name;
};
std::vector<job_t> jobs;
static sigjmp_buf env;
static volatile sig_atomic_t jmp_set;

void exec(std::string);
void exec_pipe(std::vector<std::string>);
void redirect(std::vector<std::string>&);
extern "C" void handler(int);
std::vector<std::string> split(std::string, const std::string&);
std::vector<std::string> get_args(const std::string&);
int st2i(const std::string&);
void print_job(const job_t&);
void clear_done_jobs();
void check_done_jobs();

int main() {
  // 不同步 iostream 和 cstdio 的 buffer
  std::ios::sync_with_stdio(false);

  // 用来存储读入的一行命令
  std::string cmd;

  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  if (sigsetjmp(env, 1)) std::cout << '\n';
  jmp_set = 1;

  while (true) {
    action.sa_handler = handler;
    if (sigaction(SIGINT, &action, nullptr) < 0) {
      std::cerr << "signaction error\n";
      return 1;
    }
    check_done_jobs();

    // 打印提示符
    std::cout << "# ";

    // 读入一行。std::getline 结果不包含换行符。
    std::getline(std::cin, cmd);
    if (std::cin.eof()) {
      std::cout << "exit\n";
      std::exit(0);
    }

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
      std::exit(0);
    }

    // std::string 转 int
    // std::stringstream code_stream(args[1]);
    // int code = 0;
    // code_stream >> code;
    // 转换失败
    // if (!code_stream.eof() || code_stream.fail()) {
    //   std::cerr << "exit: invalid exit code\n";
    //   return;
    // }
    try {
      std::exit(std::stoi(args[1]));
    } catch (...) {
      std::cerr << "exit: invalid exit code\n";
      return;
    }
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
    } else if (args.size() == 1) {
      if (chdir(std::getenv("HOME")))
        std::cerr << "cd: failed\n";
    } else {
      if (chdir(args[1].c_str()))
        std::cerr << "cd: no such file or directory: " << args[1] << '\n';
    }
    return;
  }

  if (args[0] == "jobs") {
    check_done_jobs();
    const auto jobs_size = jobs.size();
    for (size_t i = 0; i < jobs_size; ++i) {
      print_job(jobs[i]);
    }
    return;
  }

  if (args[0] == "wait") {
    for (auto& job : jobs) {
      if (job.stateid == 0) {
        int status;
        if (waitpid(job.pid, &status, 0) < 0) {
          std::cerr << "waitpid failed\n";
          return;
        }
        job.stateid = 1;
        print_job(job);
      }
    }
    clear_done_jobs();
    return;
  }

  if (args[0] == "fg") {
    if (args.size() > 2) {
      std::cerr << "fg: too many arguments\n";
    } else if (args.size() == 1) {
      std::cerr << jobs.back().pid << '\n';
      if (tcsetpgrp(0, jobs.back().pid) < 0) {
        std::cerr << "tcsetpgrp failed\n";
      } else {
        jobs.pop_back();
      }
    } else {
      // to be implemented
    }
    return;
  }

  // 处理外部命令
  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "fork failed\n";
    return;
  }
  if (pid == 0) {
    // 这里只有子进程才会进入
    if (args.back() == "&") {
      setpgid(0, 0);
      args.pop_back();
    }
    redirect(args);
    if(args.size() == 0) std::exit(0);

    // std::vector<std::string> 转 char **
    char *arg_ptrs[args.size() + 1];
    for (std::size_t i = 0; i < args.size(); ++i)
      arg_ptrs[i] = &args[i][0];
    // exec p 系列的 argv 需要以 nullptr 结尾
    arg_ptrs[args.size()] = nullptr;

    // execvp 会完全更换子进程接下来的代码，所以正常情况下 execvp 之后这里的代码就没意义了
    // 如果 execvp 之后的代码被运行了，那就是 execvp 出问题了
    execvp(args[0].c_str(), arg_ptrs);

    // 所以这里直接报错
    std::exit(255);
  }

  // 这里只有父进程（原进程）才会进入
  if (args.size() == 1 && args[0] == "./shell") {
    action.sa_handler = SIG_IGN;
    if (sigaction(SIGINT, &action, nullptr) < 0) {
      std::cerr << "signaction error\n";
      return;
    }
  }
  if (args.back() == "&") {
    setpgid(pid, pid);
    job_t new_job;
    new_job.jobid = 0;
    std::vector<bool> jobid_list(jobs.size() + 1); // initialized with zeros
    for (const auto job:jobs) jobid_list[job.jobid] = true;
    for (size_t i = 1; i < jobid_list.size(); ++i)
      if (!jobid_list[i]) {
        new_job.jobid = i;
      }
    if (new_job.jobid == 0) new_job.jobid = jobs.size() + 1;
    new_job.pid = pid;
    new_job.stateid = 0;
    new_job.name = cmd;
    jobs.push_back(new_job);
  } else {
    int status;
    if (waitpid(pid, &status, 0) < 0) {
      std::cerr << "waitpid failed\n";
      return;
    }
  }
}

void exec_pipe(std::vector<std::string> cmd_list) {
  const auto num = cmd_list.size();
  int last_fd0;
  for (std::size_t i = 0; i < num; ++i) {
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
      if (i > 0 && dup2(last_fd0, 0) < 0) {
        std::cerr << "dup2 failed\n";
        return;
      }
      if (i < num - 1 && dup2(fd[1], 1) < 0) {
        std::cerr << "dup2 failed\n";
        return;
      }
      exec(cmd_list[i]);
      std::exit(0);
    }
    int status;
    if (waitpid(pid, &status, 0) < 0) {
      std::cerr << "waitpid failed\n";
      return;
    }
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

void redirect(std::vector<std::string>& args) {
  int fd, fd2;
  for (auto it = args.begin(); it != args.end();) {
    auto i = it->find("<");
    if (i != std::string::npos) {
      if (i == 0) {
        fd2 = 0;
      } else if ((fd2 = st2i(it->substr(0, i))) == -1) {
        std::cerr << "redirect failed\n";
        args.clear();
        return;
      }
      const auto arg = it->substr(i+1);
      const auto arg_size = arg.size();
      if ((arg_size == 2 && arg[0] == '<' && arg[1] == '<') ||
          (arg_size == 1 && arg[0] == '<') || arg_size == 0) {
        it = args.erase(it);
        if (it == args.end()) {
          std::cerr << "redirect failed\n";
          args.clear();
          return;
        }
        if (arg_size) {
          char tf[] = "/tmp/XXXXXX";
          int tfd;
          if ((tfd = mkstemp(tf)) < 0) {
            std::cerr << "redirect failed\n";
            args.clear();
            return;
          }
          if (arg_size == 2) {
            it->push_back('\n');
            if (write(tfd, it->c_str(), it->size()) < 0) {
              std::cerr << "write failed\n";
              args.clear();
              return;
            }
          } else {
            std::string here;
            while (true) {
              std::getline(std::cin, here);
              if (here == *it) break;
              here.push_back('\n');
              if (write(tfd, here.c_str(), here.size()) < 0) {
                std::cerr << "write failed\n";
                args.clear();
                return;
              }
            }
          }
          if (close(tfd) < 0) {
            std::cerr << "close failed\n";
            args.clear();
            return;
          }
          if ((fd = open(tf, O_RDONLY)) < 0) {
            std::cerr << "open failed\n";
            args.clear();
            return;
          }
        } else {
          if ((fd = open(it->c_str(), O_RDONLY)) < 0) {
            std::cerr << "open failed\n";
            args.clear();
            return;
          }
        }
        it = args.erase(it);
      } else {
        std::cerr << "redirect failed\n";
        args.clear();
        return;
      }
      if (dup2(fd, fd2) < 0) {
        std::cerr << "dup2 failed\n";
        args.clear();
        return;
      }
      if (close(fd) < 0) {
        std::cerr << "close failed\n";
        args.clear();
        return;
      }
      continue;
    }
    i = it->find(">");
    if (i != std::string::npos) {
      if (i == 0) {
        fd2 = 1;
      } else if ((fd2 = st2i(it->substr(0, i))) == -1) {
        std::cerr << "redirect failed\n";
        args.clear();
        return;
      }
      const auto arg = it->substr(i+1);
      if ((arg.size() == 1 && arg[0] == '>') || arg.empty()) {
        it = args.erase(it);
        if (it == args.end()) {
          std::cerr << "redirect failed\n";
          args.clear();
          return;
        }
        if ((fd = open(it->c_str(), O_WRONLY | O_CREAT | (arg.size() == 1 ? O_APPEND : O_TRUNC),
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) { // -rw-r--r--
          std::cerr << "open failed\n";
          args.clear();
          return;
        }
        it = args.erase(it);
      } else {
        std::cerr << "redirect failed\n";
        args.clear();
        return;
      }
      if (dup2(fd, fd2) < 0) {
        std::cerr << "dup2 failed\n";
        args.clear();
        return;
      }
      if (close(fd) < 0) {
        std::cerr << "close failed\n";
        args.clear();
        return;
      }
      continue;
    }
    ++it;
  }
}

void handler(int signum) {
  if (jmp_set == 0) return;
  if (signum == SIGINT) {
    siglongjmp(env, 1);
  }
}

// 经典的 cpp string split 实现
// https://stackoverflow.com/a/14266139/11691878
std::vector<std::string> split(std::string s, const std::string& delimiter) {
  std::vector<std::string> res;
  std::size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    res.push_back(token);
    s = s.substr(pos + delimiter.length());
  }
  res.push_back(s);
  return res;
}

std::vector<std::string> get_args(const std::string& s) {
  std::vector<std::string> res;
  std::string buf;
  std::stringstream ss(s);
  while (ss >> buf) res.push_back(buf);
  return res;
}

int st2i(const std::string& s) {
  try {
    std::size_t pos;
    int num = std::stoi(s, &pos);
    if (pos == s.size()) return num;
    return -1;
  } catch (...) {
    return -1;
  }
}

void print_job(const job_t& job) {
  std::cout << '[' << job.jobid << "]  "
  << ((job.pid == jobs.back().pid) ? '+' : ((job.pid == jobs.end()[-2].pid) ? '-' : ' '))
  << ' ' << job.pid << ' ' << STATE[job.stateid] << "  " << job.name << std::endl;
}

void clear_done_jobs() {
  jobs.erase(std::remove_if(jobs.begin(), 
                          jobs.end(),
                          [](const job_t& job) { return job.stateid == 1; }),
            jobs.end());
}

void check_done_jobs() {
  for (auto& job : jobs) {
    if (job.stateid == 0) {
      int status;
      auto ret = waitpid(job.pid, &status, WNOHANG);
      if (ret < 0) {
        std::cerr << "waitpid failed\n";
        return;
      }
      if (ret) {
        // child process has finished
        job.stateid = 1;
        print_job(job);
      }
    }
  }
  clear_done_jobs();
}
