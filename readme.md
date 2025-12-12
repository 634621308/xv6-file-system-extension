本次实验（文件系统实用命令扩展）涉及的代码文件目录结构：

xv6-labs-2025/
│
├── kernel/
│   ├── fcntl.h              # 新增 O_APPEND 标志
│   ├── stat.h                # 新增 T_SYMLINK 类型
│   ├── defs.h                # 新增 namei_nofollow 函数声明
│   ├── syscall.h             # 新增 SYS_symlink、SYS_lstat 系统调用号
│   ├── syscall.c             # 注册新系统调用
│   ├── sysfile.c             # 实现 O_APPEND 处理、sys_symlink、sys_lstat
│   └── fs.c                  # 实现软链接解析、namei_nofollow
│
├── user/
│   ├── sh.c                  # 解析 >> 重定向运算符
│   ├── ln.c                  # 添加 -s 选项支持软链接
│   ├── grep.c                # 添加 -i 选项支持大小写不敏感
│   ├── stat.c                # 【新增】stat 命令实现
│   ├── find.c                # 【新增】find 命令实现
│   ├── user.h                # 添加 symlink、lstat 函数声明
│   └── usys.pl               # 添加系统调用入口
│
└── Makefile                  # 添加 _find、_stat 到 UPROGS