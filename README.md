## 作用
安装ndn-cxx和NFD，运行文件传输程序。

## 支持平台

测试过的平台:
> Ubuntu 16.04 (amd64, armhf, i386)  
> Ubuntu 18.04 (amd64)  
> Ubuntu 19.10 (amd64)  
> macOS 10.13  
> macOS 10.14  
> macOS 10.15  
> CentOS 7 (with gcc 7 and boost 1.58.0)  

应该可以运行但还未测试的平台:
> Debian >= 9  
> Fedora >= 24  
> Gentoo Linux  
> Raspbian >= 2017-08-16  
> FreeBSD 11.2  

下文针对ndn支持的ubuntu平台，如在安装过程中遇到其它平台或库的问题，需自行解决。

## 准备环境
> GCC >= 5.3, or clang >= 3.6  
> python2 >= 2.7, or python3 >= 3.4  
> Boost libraries >= 1.58  
> pkg-config  
> SQLite 3.x  
> OpenSSL >= 1.0.2  

命令：  
**sudo apt install build-essential libboost-all-dev libssl-dev libsqlite3-dev pkg-config python-minimal**

> doxygen  
> graphviz  
> python-sphinx  
> sphinxcontrib-doxylink  

命令：  
**sudo apt install doxygen graphviz python3-pip  
sudo pip3 install sphinx sphinxcontrib-doxylink**
