## 作用  
文件夹下含两对可终端运行的程序  
第一对程序：在数据请求端和数据发布端之间传输单个数据包，用以熟悉NDN网络中的数据传输过程。  
第二对程序：将文件分块成多个数据包，并在数据请求端和数据发布端之间进行传输，用以验证拥塞控制机制的性能。  

## 使用
编译及使用，见下层文件夹内说明文件(README.md)  

## 注意  
每个节点都需为NDN节点（包括中间路由节点），即都需要装上ndn-cxx和NFD，并运行NFD

如果存在权限问题，可尝试在前面加上sudo运行。

在使用复杂拓扑传输前，可先在同一主机上的两个终端分别运行数据发布程序和数据请求程序，以验证功能。之后可在多个主机间配置好NDN网络，然后进行多点传输。
