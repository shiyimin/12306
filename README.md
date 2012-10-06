12306
=====

我要回家过年

### 使用说明
1. 下载并安装Virtual Box 4.2: https://www.virtualbox.org/wiki/Downloads
2. 下载并安装Vagrant: http://vagrantup.com/
3. 第一次运行时,打开命令行，进入源代码的根目录,执行（如果是windows平台，在下面的命令后面加.bat后缀）：
   
   vagrant box add base http://files.vagrantup.com/lucid32.box

   vagrant up
   
4. 下次就只需要执行下面的命令就可以使用:
   
   vagrant up

5. 如果是windows平台，用putty登录：127.0.0.1:2222，用户名/密码：vagrant/vagrant
6. 如果是非Windows平台
   
   vagrant ssh
   
7. 进入工作主目录 - 这个虚拟机上的主目录是你宿主机执行vagrant命令的目录
   
   cd /vagrant

   cd server-core/cn12306

   mvn compile test
   
8. 顺利的话,你应该可以看到BUILD SUCCESS的字样,如有任何问题,欢迎到下面的链接上报BUG:
   https://github.com/shiyimin/12306/issues

English
=======
### Getting Start
1. Download and install latest virtual box: https://www.virtualbox.org/wiki/Downloads
2. Download and install vagrant: http://vagrantup.com/
3. If you are run this project for the first time, change you cmd(Windows) or shell(Linux/Mac) to the root directory of this project, and run (this will create a virtual machine and setup the dev environment for you automatically):
   
   vagrant box add base http://files.vagrantup.com/lucid32.box

   vagrant up

4. next time, you just cd to the root directory of this project, and run:
   
   vagrant up

5. For Windows platform, please login to the virtual machine by putty, below are putty settings:
   Host name - 127.0.0.1:2222
   User name/Password - vagrant/vagrant

6. For Non Windows platform, run:
   
   vagrant ssh
   
7. Now you should log on the virtual machine shell, build the project source on the virtual machine by run following:
   
   cd /vagrant

   cd server-core/cpp

   make



   