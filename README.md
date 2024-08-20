# Timer Resolution Tools

This tool allows you to query and set the global timer resolution. This will only work on Windows Server 2022+ and Windows 11+ according to The [Greate Rule Change](https://randomascii.wordpress.com/2020/10/04/windows-timer-resolution-the-great-rule-change/) article. 

This tool allows you to increase the precision of the `Sleep(DWORD dwMilliseconds)`. If you were to sleep for 1 millisecond you could actually sleep for 1.0ms - 16.625ms! Changing the resolution to the max can reduce this difference to 0.5ms - 1ms. This can greatly reduce input latency and increase responsiveness. 


# How to build 

This project is built with CMake. To install CMake see this [link](https://cmake.org/download/) or install it through winget (`winget install CMake`) in a Command Line instance.
