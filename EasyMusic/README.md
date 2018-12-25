# 应用框架说明

### 应用框架组成部分
 - 消息机制

### 应用框架最小系统
可运行应用框架的最小系统的核心为消息机制，包含下列组件：
 - include/app/application.h
 - include/message/handler.h
 - include/message/looper.h
 - include/message/message.h
 - include/system/system.h
 - src/app/application.cpp
 - src/message/handler.cpp
 - src/message/looper.cpp
 - src/message/message.cpp
 - src/system/system.cpp