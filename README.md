## 作用
安装ndn-cxx和NFD，测试ndn环境。

## 支持平台

测试过的平台:
> Ubuntu 16.04 (amd64, armhf, i386)  
> Ubuntu 18.04 (amd64)  
> Ubuntu 19.10 (amd64)  
> macOS 10.13  
> macOS 10.14  
> macOS 10.15  
> CentOS 7 (with gcc 7 and boost 1.58.0)  

应该可以运行但未测试的平台:
> Debian >= 9  
> Fedora >= 24  
> Gentoo Linux  
> Raspbian >= 2017-08-16  
> FreeBSD 11.2  

下文针对ndn支持的ubuntu平台，如在安装过程中遇到其它平台或库的问题，需自行解决。  
运行指令时，如需输入密码，为主机开机密码。

## 准备环境
> GCC >= 5.3, or clang >= 3.6  
> python2 >= 2.7, or python3 >= 3.4  
> Boost libraries >= 1.58  
> pkg-config  
> SQLite 3.x  
> OpenSSL >= 1.0.2  

命令：  
`sudo apt install build-essential libboost-all-dev libssl-dev libsqlite3-dev pkg-config python-minimal`

> doxygen  
> graphviz  
> python-sphinx  
> sphinxcontrib-doxylink  

命令：  
&#160; &#160; &#160; &#160; **sudo apt install doxygen graphviz python3-pip**  
&#160; &#160; &#160; &#160; **sudo pip3 install sphinx sphinxcontrib-doxylink**

&#160; &#160; &#160; &#160; **sudo apt-get install build-essential pkg-config libboost-all-dev libsqlite3-dev libssl-dev libpcap-dev**  
&#160; &#160; &#160; &#160; **sudo apt-get install doxygen graphviz python-sphinx**

## 下载和编译安装
命令：  
&#160; &#160; &#160; &#160; **git clone git@github.com:tpgithubact/transfer-real.git**  
&#160; &#160; &#160; &#160; **cd transfer-real/**  
&#160; &#160; &#160; &#160; **git checkout remotes/origin/ndn-cxx**  
&#160; &#160; &#160; &#160; **cd 0.6.2/**  
&#160; &#160; &#160; &#160; **./waf configure**  
&#160; &#160; &#160; &#160; **./waf**  
&#160; &#160; &#160; &#160; **sudo ./waf install**  

&#160; &#160; &#160; &#160; **cd ../**  
&#160; &#160; &#160; &#160; **git checkout remotes/origin/NFD**  
&#160; &#160; &#160; &#160; **cd 0.6.2/**  
&#160; &#160; &#160; &#160; **./waf configure**  
&#160; &#160; &#160; &#160; **./waf**  
&#160; &#160; &#160; &#160; **sudo ./waf install**  

## 测试
命令：  
&#160; &#160; &#160; &#160; **nfd-start**  
&#160; &#160; &#160; &#160; **nfd-stop**
