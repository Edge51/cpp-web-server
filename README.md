# cpp-web-server
web server implemented with C/C++.


client send message with socketFd instead of connectFd

### 开发日记


2024-05-12
搜到这篇文章学习了一下，cmake项目目录结构的建议[How to structure your project](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)
把logger放到了子目录下


2024-05-11
将logger配置到了gtest里面，计划明天把logger改成一个子cmake目录。


2024-05-10
项目打算通过CMake编译，加了cmake后一直还没编译成功，今天看是本地编译环境没有安装好cmake,以及vscode没有配置好插件。

CMakeLists.txt拼错了，漏了s

将CMakeLists.txt修改好后，成功编译好二进制，并且引入的gtest体系也成功测试运行