## 编译安装  
### 环境准备  
确保已经安装ndn-cxx、NFD及它们的依赖库。  

安装libpcap：sudo apt-get install libpcap-dev  

### 编译安装
仍然使用waf的编译方式

./waf configure
./waf
sudo ./waf install


### 运行
* [peek](tools/peek): transmit a single packet between a consumer and a producer
* [chunks](tools/chunks): segmented file transfer between a consumer and producer
