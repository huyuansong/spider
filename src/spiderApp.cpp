#include <stdio.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include "spiderApp.h"
#include "spider.h"
#include "confparser.h"
SpiderApp::SpiderApp()
{

}

SpiderApp::~SpiderApp()
{


}

int SpiderApp::initApp(int argc, char* argv[])
{
	/*处理参数*/
	if(initArgv(argc, argv));

	/*读取配置文件*/
	Confparser::instance()->loader(CONF_PATH );
	
	/*处理是否以守护进程运行逻辑*/
	if( isDeamon == 1)
	{
		this->deamon();
	}

	/*载入所有的模块*/
	//等待完成
	
	
}

int SpiderApp::run()
{
	//等待相关管理类完成

}

//private:
int initArgv(int argc, char* argv[])
{
    /* 解析命令行参数 */
    while ((ch = getopt(argc, (char* const*)argv, "vhd")) != -1) {
        switch(ch) {
            case 'v':
                version();
                break;
            case 'd':
                daemonized = 1;
                break;
            case 'h':
            case '?':
            default:
                usage();
        }
    }
}
int SpiderApp::attach_epoll_task()
{
    struct epoll_event ev;
    int sock_rv;
    int sockfd;
    Url * ourl = pop_ourlqueue();
    if (ourl == NULL) {
        SPIDER_LOG(SPIDER_LEVEL_WARN, "Pop ourlqueue fail!");
        return -1;
    }

    /* connect socket and get sockfd */
    if ((sock_rv = build_connect(&sockfd, ourl->ip, ourl->port)) < 0) {
        SPIDER_LOG(SPIDER_LEVEL_WARN, "Build socket connect fail: %s", ourl->ip);
        return -1;
    }

    set_nonblocking(sockfd);

    if ((sock_rv = send_request(sockfd, ourl)) < 0) {
        SPIDER_LOG(SPIDER_LEVEL_WARN, "Send socket request fail: %s", ourl->ip);
        return -1;
    } 

    evso_arg * arg = (evso_arg *)calloc(1, sizeof(evso_arg));
    arg->fd = sockfd;
    arg->url = ourl;
    ev.data.ptr = arg;
    ev.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(g_epfd, EPOLL_CTL_ADD, sockfd, &ev) == 0) {/* add event */
        SPIDER_LOG(SPIDER_LEVEL_DEBUG, "Attach an epoll event success!");
    } else {
        SPIDER_LOG(SPIDER_LEVEL_WARN, "Attach an epoll event fail!");
        return -1;
    }

    g_cur_thread_num++; 
    return 0;
}

