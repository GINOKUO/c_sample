1 源码下载
LIVE555官网：http://www.live555.com/

LIVE555源码FTP下载地址：http://www.live555.com/liveMedia/public/

从中选择一个较新版本下载，这里下载live.2017.09.12.tar.gz，如下图：



2 源码编译
将下载的源码放到linux环境下，解压源码包

tar xvzf live.2017.09.12.tar.gz

得到live文件夹，进入live目录下

cd live/

生成makefile

./genMakefiles linux

编译

make


3 RTSP推流体验
编译后在live/mediaServer下生成live555MediaServer，拷贝一个.264测试视频文件到/live/mediaServer下

开启LIVE555服务器

./live555MediaServer



4 WINDOWS下打开VLC播放
连接上网线，打开VLC播放器，媒体->打开网络串流，输入rtsp://192.168.2.212:8554/slamtv60.264，如下图

 

注意：192.168.2.212为ubuntu系统IP，slamtv60.264为测试文件

点击播放，播放成功，如下图

 
--------------------- 
作者：yqwung 
来源：CSDN 
原文：https://blog.csdn.net/wang3141128/article/details/80483287 
版权声明：本文为博主原创文章，转载请附上博文链接！