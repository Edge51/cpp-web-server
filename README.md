# cpp-web-server
web server implemented with C/C++.


client send message with socketFd instead of connectFd

### 开发日记

2024-08-07
Don't rush. Don't always think I should finish the task today or something like this. Just focus on what you are doing.
Came across this error:
```
terminate called without an active exception
```
reason: program terminated without joining or detaching all thread.


2024-07-31
My queue crash when running the simplest test case... vscode just skip my breakpoint and directly continue to the line where the segmentation fault take place.
1. After unique_ptr has been std::move, the original variable is null
``` cpp
std::unique_ptr<Node> newNode = std::make_unique<Node>(nullptr, nullptr);
    m_tail->data = data;
    m_tail->next = std::move(newNode);
    m_tail = newNode.get(); // m_tail == nullptr is true
```
2. uint32_t < -1 is wrong to check size, because the -1 will cast to uint32_t and compare, in which case -1 = FFFFFFFF.

2024-07-30
Continue to implement the queue. I found it hard to complete the task at one shot. So I decided to firstly implement a simple queue, which has interfaces just like std::queue. I think I need to learn more about template and smart pointer.
I still don't understand well why template implementation need place in header file.

2024-07-29
Today's task is implement the Thread-Unsafe version of the queue.

2024-07-23 晚上
学习要探索舒适区外的内容，要学会思想和更高层次的东西，比如设计模式，体会面向接口设计，依赖倒置等思想之后，一个一个设计模式的技法就慢慢理解了,并发编程也是，要细致地端到端地去分析问题。

2024-07-23 凌晨
想要添加日志系统，方便后续开发时定位问题，发现还是得先写一个可并发的Queue，今天着手定义了类的接口，后续还要将实现和UT完善起来。
``` mermaid
graph LR;
data,next-->dummyTail
```

Four crucial aspects for concurrent data structure design:
- at any time, client can only see an invariants status for the data structure;
- each interface of the data structure avoid race condition;
- exception safety, even if exception occur, it won't corrupt the data integation
- avoid dead lock

**minimized serialization and maximize the concurrency**

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