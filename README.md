# cpp-web-server
web server implemented with C/C++.


client send message with socketFd instead of connectFd

### 开发日记

2025-07-08
近期重新开始接着写cpp server项目，跟着30dayscppwebserver项目写，已经写到了第11天，感觉跟着写要比想象中的简单许多。
今天增加ThreadPool遇到的问题主要是线程获取任务的时候，判断可以获取任务的条件写错了，应该是如果没有在运行了或者task队列非空，再往下走获取任务，而不是运行中或者队列非空。
同时，还要端到端仔细推敲，当ThreadPool停止运行之后会执行notify_all，之后所有线程都会来尝试从队列获取任务，此时，需要判断队列如果空，就不能再直接获取任务了，否则拿到空任务，程序就发生异常了。

随后将ThreadPool的AddTask函数改成模板函数，并通过std::future获取返回值。
学习收获：增加ThreadPool写的时候要仔细审视清楚功能和逻辑，真正弄懂了再动手，而不是囫囵吞枣，照猫画虎。C++11后的新特性std::result_of的语法也不熟悉，需要多写写。

今天觉得，应该白天学习清楚晚上具体要开发什么，等到晚上写的时候，尽量不要参考代码完成实现，这样才是比较好的学习，因为会了应该是可以真正写出来才是会了，而不是参考着写出来。
做事情慢一步，那么慢一步慢下来，是需要做什么样的思考呢？是否应该在做事情之前，就想清楚，我是想要学到什么东西而去做这个事情。

2024-08-26
because the concurrent test throws exception, seems like the pop should be change to try pop and wait and pop.

2024-08-15
fix BUG20240815001 and start a BUG record.
- map iterator returns pair
```
    EXPECT_TRUE([&]()-> bool{
        return std::all_of(items.begin(), items.end(), [](std::pair<int, int> item){
            return item.second == 1;
        });
    });
```
generating compile error:
```
[build] /home/Edge51/Projects/cpp-web-server/build/_deps/googletest-src/googletest/include/gtest/gtest-assertion-result.h:161:9: 错误：cannot convert ‘const ConcurrentQueueTest_MultiThread_Test::TestBody()::<lambda()>’ to ‘bool’ in initialization
[build]   161 |       : success_(success) {}
[build]       |         ^~~~~~~~~~~~~~~~~
[build] make[3]: *** [lib/ConcurrentQueue/CMakeFiles/ConcurrentQueueTest.dir/build.make:76：lib/ConcurrentQueue/CMakeFiles/ConcurrentQueueTest.dir/test/ConcurrentQueueTest.cpp.o] 错误 1
```

2024-08-14
Continue to finish this concurrent queue, with reading *C++ concurrency in Action*, I learn about that generally the concurrent version of queue don't implement Top and Pop seperately. If the queue have both Top and Pop interfacec provided to user, the user probably will get into race condition without care. So first thing I need do in order to improve the queue is to redefine the interfaces of the queue and then modify to make the test pass.
 
things to figure out:
- exception defining
- smart pointer
- 

exception define by inherit the std::exception interface:
``` cpp
class EmptyQueue : std::exception {
public:
    const char* what() const throw() { return "Empty Queue!";};
};
```

2024-08-08
Didn't get it at first why unit test failed. But I print the log and get it right finally. It turns out that there is some mistake in the unit test, look at the code below:
``` C++
std::map<int, int> items;
std::vector<std::thread> consumerList;
std::vector<int> popingLog;
for (int i =  0; i < consumerNum; i++) {
    consumerList.emplace_back([&] (ConcurrentQueue<int>& queue, std::map<int, int>& items) {
        for (int j  = 0; j < producerNum * 2 / consumerNum; j++) {
            int top = queue.Top();  // top is read and the mutex is released
            items[top]++; // in between Top() and Pop(), maybe some other thread read top and add top in items
            queue.Pop(); // maybe not poping top
        }
    }, std::ref(queue), std::ref(items));
}
```

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