# cpp-web-server
web server implemented with C/C++.


client send message with socketFd instead of connectFd

### 开发日记


项目打算通过CMake编译，加了cmake后一直还没编译成功，今天看是本地编译环境没有安装好cmake,以及vscode没有配置好插件。

CMakeLists.txt拼错了，漏了s

将CMakeLists.txt修改好后，成功编译好二进制，并且引入的gtest体系也成功测试运行