* 执行流程
新开启的时候需要先cmake config一下 —— TODO 这是做了什么事情？
修改根目录下的CmakeLists.txt包含的工程号并保存
然后在按照普通操作进行即可



* 操作命令

  ```
  git reset --hard && git clean -d -fx
  cd build
  cmake -G "MinGW Makefiles" ..
  mingw32-make.exe
  ./test/unit_tests.exe
  ```

  

​       