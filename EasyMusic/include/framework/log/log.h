#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#define ENABLE_DEBUG true
#define ENABLE_INFO  true
#define ENABLE_WARN  true
#define ENABLE_ERROR true

using namespace std;

namespace cobox {

     class Log {
     private:
          static const int TYPE_DEFAULT        = 0;
          static const int TYPE_BOLD           = 1;  // 粗体
          static const int TYPE_NON_BOLD       = 2;  // 非粗体
          static const int TYPE_UNDER_LINE     = 3;  // 下划线
          static const int TYPE_NON_UNDER_LINE = 4;  // 无下划线
          static const int TYPE_REVERSE        = 7;  // 反显
          static const int TYPE_NON_REVERSE    = 27; // 非反显

          static const int FG_BLACK            = 30; // 黑色
          static const int FG_RED              = 31; // 红色
          static const int FG_GREEN            = 32; // 绿色
          static const int FG_YELLOW           = 33; // 黄色
          static const int FG_BLUE             = 34; // 蓝色
          static const int FG_MAGENTA          = 35; // 洋红
          static const int FG_CYAN             = 36; // 青色
          static const int FG_WHITE            = 37; // 白色
          static const int BG_BLACK            = 40; // 黑色
          static const int BG_WHITE            = 47; // 白色

     public:
          static void d(string tag, string fmt, ...) {
     #if ENABLE_DEBUG
               va_list args;
               va_start(args, fmt);

               cout << "\033[0m" << tag << " D ";
               vprintf(fmt.c_str(), args);
               cout << "\033[0m" << endl;

               va_end(args);
     #endif
          }

          static void i(string tag, string fmt, ...) {
     #if ENABLE_INFO
               va_list args;
               va_start(args, fmt);

               cout << "\033[0;" << FG_GREEN << ";" << BG_BLACK << "m"
                    << tag << " I ";
               vprintf(fmt.c_str(), args);
               cout << "\033[0m" << endl;
               
               va_end(args);
     #endif
          }

          static void w(string tag, string fmt, ...) {
     #if ENABLE_WARN
               va_list args;
               va_start(args, fmt);

               cout << "\033[0;" << FG_YELLOW << ";" << BG_BLACK << "m"
                    << tag << " W ";
               vprintf(fmt.c_str(), args);
               cout << "\033[0m" << endl;
               
               va_end(args);
     #endif      
          }

          static void e(string tag, string fmt, ...) {
     #if ENABLE_ERROR
               va_list args;
               va_start(args, fmt);

               cout << "\033[0;" << FG_RED << ";" << BG_BLACK << "m"
                    << tag << " E ";
               vprintf(fmt.c_str(), args);
               cout << "\033[0m" << endl;
               
               va_end(args);
     #endif      
          }
     };

}