12306
=====

我要回家过年

### 使用说明
1. 下载并安装Virtual Box 4.2: https://www.virtualbox.org/wiki/Downloads
2. 下载并安装Vagrant: http://vagrantup.com/
3. 打开命令行，执行（如果是windows平台，在下面的命令后面加.bat后缀）：

    vagrant box add base http://files.vagrantup.com/lucid32.box
    vagrant init
    vagrant up

4. 如果是windows平台，用putty登录：127.0.0.1:2222，用户名/密码：vagrant/vagrant
5. 如果是非Windows平台

    vagrant ssh

6. 进入工作主目录 - 这个虚拟机上的主目录是你宿主机执行vagrant命令的目录

    cd /vagrant
    cd server-core/cn12306
    mvn compile test

7. 顺利的话,你应该可以看到BUILD SUCCESS的字样,如有任何问题,欢迎到下面的链接上报BUG:
   https://github.com/shiyimin/12306/issues