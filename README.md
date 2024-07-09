# cpp-web-server
web server implemented with C/C++.


client send message with socketFd instead of connectFd

### 开发日记


2024-07-10
今天把原来的Client.cpp和Server.cpp做了一点简单的重构，把main函数的流程直接整个搬出来一个函数，方便gtest用例调用，这样就有测试用例去保障重构不出错了，下一步就是把原来的过程式代码重构成几个类，把socket封装成类，而不是用原生的socket去写过程式的代码。
有几个点后续需要搞懂，测试用例里面，为什么thread.detach才能正常运行，socket需要读manual学习
测试用例依赖了源文件，不知道这样是否符合测试用例的原则

2024-06-27
将最初的server.cpp调试成功了，之前一直server.cpp运行之后，client.cpp可以收发成功，但是浏览器无法访问，报错connection refused，最后google发现有人遇到相似问题，是端口号绑定时需要用htons转换成网络序
2024-05-12
搜到这篇文章学习了一下，cmake项目目录结构的建议[How to structure your project](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)
把logger放到了子目录下


2024-05-11
将logger配置到了gtest里面，计划明天把logger改成一个子cmake目录。


2024-05-10
项目打算通过CMake编译，加了cmake后一直还没编译成功，今天看是本地编译环境没有安装好cmake,以及vscode没有配置好插件。

CMakeLists.txt拼错了，漏了s

将CMakeLists.txt修改好后，成功编译好二进制，并且引入的gtest体系也成功测试运行