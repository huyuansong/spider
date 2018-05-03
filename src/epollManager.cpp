#include <sys/epoll.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
//构造函数 初始化Epoll，得到句柄
EpollManager::EpollManager(int num)
{
    m_epollHandle = epoll_create(num);
    m_tasknum = num;
}
//释放句柄
EpollManager::~EpollManager()
{

}
//注册socket句柄
int EpollManager::registHandle(int handle)
{

}
//注销句柄
int EpollManager::unregistHandle(int handle)
{

}






