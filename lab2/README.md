# Lab 2

## 目录导航

- 正确实现了基本的 `pwd` 内建命令
  - 实现了 [POSIX 标准](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/pwd.html) 中只有 1 个参数的情形。
- 正确实现了基本的 `cd` 内建命令
  - 实现了 [POSIX 标准](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/cd.html) 中 `cd [directory]` 的情形。
  - **\<选做\>** 在没有第二个参数时，默认进入家目录。

`pwd` 和 `cd` 都必须是内建命令（[理由](https://unix.stackexchange.com/questions/38808/why-is-cd-not-a-program)）。

## 管道

- 支持两条命令的管道
- 支持任意多条命令的管道

> - `|` 符号的两边可以没有空格字符
> - *不支持*含有内建命令的管道

## 重定向

- 支持 `>` 重定向
- 支持 `<` 重定向
- 支持 `>>` 重定向
- **\<选做\>** 支持数字文件描述符
- **\<选做\>** 支持文本重定向
  - 程序会在 `/tmp/` 目录中读写，需要有权限。
- **\<选做\>** 支持 EOF 重定向
  - 程序会在 `/tmp/` 目录中读写，需要有权限。
  - 此重定向是 [POSIX 标准](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_07_04)。未实现标准中 `word` 含有 \$(), ``, \${} 或双引号等字符的情况。

> - *不支持*内建命令的重定向
> - *不支持* `stderr` 的重定向
> - 重定向符号的两边*总是*各有至少一个空格字符
> - *不支持*含有 "<" 和 ">" 符号的文件名

### 信号处理

- `CTRL+C` 正确终止正在运行的进程
- `CTRL+C` 在 shell 嵌套时也能正确终止正在运行的进程
- `CTRL+C` 可以丢弃命令

### 前后台进程

- 后台程序运行。
  - 类似 bash，会有 *\<job-number\>* 和 *\<process ID\>* 等信息的输出。运行完成后也会有输出。
- 实现 `wait` 命令。
  - 实现了 [POSIX 标准](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/wait.html) 中只有 1 个参数的情形。
- **\<选做\>** 部分实现 `fg` 命令。只支持 1 个参数的情况，且实现*不*完全正确。

此外，我实现了 `jobs` 命令，用它可以看到后台程序。

- 由于未实现 `CTRL+Z` 信号的处理，永远不会有 `suspended` 状态的程序。
- `jobs` 只支持 1 个参数，实现的实际上是 [POSIX 标准](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/jobs.html) 中的 `jobs -l`。

在大部分情况下我实现的 `fg` 不能正确执行。目前尚未找到解决方案。
