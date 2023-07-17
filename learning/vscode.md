# vscode task配置文件

* label
标签名，别人运行该程序的入口

* options —— todo
运行该配置文件前，会首先运行该指令
他放到tasks里面还是外面有什么区别呢

* type —— TODO
指令类型，目前还不是很清楚该怎么配置，但是能感觉到如果是普通命令行的话就是"shell"
如果是编译的话，目前看要配置成"cppbuild"

* command
需要运行的指令

* args
指令的参数

# vscode launch配置文件

* preLaunchTask
运行该任务前的前置任务

* program
调试的依赖程序

* miDebuggerPath
感觉像是调试工具的具体路径

# 格式化
* C_Cpp.clang_format_sortIncludes
  改为false，从而解决头文件按照字典序重排的问题

* C_Cpp.clang_format_style
  ```
  { BasedOnStyle: LLVM, UseTab: Never, IndentWidth: 4, TabWidth: 4, BreakBeforeBraces: Linux, AllowShortIfStatementsOnASingleLine: false, IndentCaseLabels: false, ColumnLimit: 0, AccessModifierOffset: -4, NamespaceIndentation: All, FixNamespaceComments: false }
  ```
  解决{另起一行的问题