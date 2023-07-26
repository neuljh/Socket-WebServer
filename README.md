# Socket-WebServer 一键三连谢谢啦！
基于Socket编程的远程文件备份服务器,基于Socket编程的带身份认证功能的远程 数据备份服务器
# 基于Socket编程的时间服务器
![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/901aaa09-1e27-4428-b726-32354ca28936)

1.启动服务器

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/7a67a8b3-23f6-4a6e-be17-3c5fc1058db2)

2.启动客户端

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/3a48f530-2fda-4404-ab4e-205c928c35f0)

客户端main函数需要一个入口参数，即IP地址，若未输入则无法正常使用时间功能。上图为失败界面。下图为成功界面。

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/af206105-e291-425b-bf4b-d77c39f2daa2)

同时，服务端也会同步打印数据：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/7bf2857f-12fa-49a0-b99e-614dd159794b)


3.服务器循环监听客户端的连接请求
客户端：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/1663df69-e0bc-4739-84ba-0a83a2c020c8)

服务端：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/e950e5b2-c7c0-458c-b32f-db7300ddd816)

# 基于Socket编程的远程文件备份服务器

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/cecd0539-0c93-49d1-ab06-a4380ec9147a)

1.启动服务器

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/9dde2fb7-4ac6-4cca-92b7-85ce2f0f31ab)


2.启动客户端

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/a15834cc-bc55-4598-91fc-4b09883d7f61)

客户端main函数需要一个入口参数，即IP地址，若未输入则无法正常使用文件备份功能。上图为失败界面。下图为成功界面。

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/ae499aee-e7a9-40ac-b3ad-436d3ca32f2e)

服务端显示连接成功：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/ab152245-df39-495a-b0c2-edad2946813b)


3.文件备份功能
选择备份主目录下的timerecords文件，文件内容如下：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/05a226fe-9626-469d-ac79-bdab770c9531)

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/4341fdac-5f85-4984-82d1-b546becd8093)

客户端传输文件：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/1ce035c0-398c-47c6-9449-be36cb01b8d1)

服务端接收文件并保存文件：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/88653513-263d-4833-9a02-ef2eb46b9e50)

查看文件，发现文件备份成功！：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/1e71b393-d77b-4002-ac3b-5bdddc8c2f98)

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/8de867c9-a3f8-498c-8b53-c4565bb15fa6)

# 基于Socket编程的带身份认证功能的远程数据备份服务器
系统总流程：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/6848a411-38aa-4c2f-bf6b-8178d245da52)

功能详细流程图：
①RSA密钥的生成与发送

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/673252f0-fad4-4b09-8307-ecdaf05f8cf2)

②DES密钥的生成与发送

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/044576b1-e008-44d9-98b1-7a361a3a2941)

③注册功能的实现(含身份管理)

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/6c17b7cf-455f-4839-aafd-3fce609ad151)



④登录功能的实现(含身份认证）

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/99a07bac-f361-4d9e-b0d3-de212014ac31)


⑤数据备份功能

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/2467362d-89c6-452b-a0d4-1f0105403a0a)

运行结果：

1.启动服务器

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/bec6abb3-6bb6-49c6-af05-65c45aa158a3)

2.启动客户端

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/4c420f02-97c8-4c20-bb9e-5e6a707fdee6)

等待系统初始化，完成各项密钥的存储管理，发现客户端打印出相关数据，代表系统初始化完成：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/7e5f0a59-3b2d-4296-a3df-6445ef4b6907)

服务端也打印出相关数据：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/579edc18-d9c9-41d4-aa59-389122380552)

4.注册功能(含身份管理)
在客户端输入相关信息。
客户端：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/26eb9262-f73d-4fed-8444-68a22972258a)

服务端：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/4a15d81c-bd76-4fc1-ab97-38d879ef8e1d)


当注册完之后，服务端写入对应信息，然后程序结束。

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/acd7b8f0-63c1-43e3-a233-a866735b34bd)

最后一条为客户端注册而新加入的用户信息。

5.登录功能(含身份验证)
重新运行程序，客户端输入1选择登录功能。

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/ceb40230-5651-4f55-9658-5c4e293ba200)

输入刚才的注册信息：
客户端：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/0385a798-58f2-4c22-b680-6244c002df74)

服务端：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/89606858-3ec0-426f-a4f0-803c6befda83)

身份核验通过！

6.文件备份功能
选择备份文件timerecords,提前查看文件内容：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/d3130946-42d0-4e95-adaa-f441d4445b6b)

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/52992a34-5e95-4bcf-bde8-17627251491c)


客户端：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/f537e20c-3ee2-4c01-bf02-a0225f55a213)

服务端：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/279db4fc-a108-4a93-b195-4c81d862462b)

查看文件备份情况：

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/91b913f4-ab65-4797-bd59-2844a119721f)

![image](https://github.com/neuljh/Socket-WebServer/assets/132900799/0042f6fb-1d79-4449-86a8-e4c260e0b602)


文件创建成功并备份成功。
