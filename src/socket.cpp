#include <stdio.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include<sys/socket.h>
#include "socket.h"


Socket::Socket()
{
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (!inet_aton(ip, &(server_addr.sin_addr))) {
        return -1;
    }

    if ((*m_socketHandle = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    if (connect(*m_socketHandle, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0) {
        close(*m_socketHandle);
        return -1;
    }
}
Socket::~Socket()
{
    close(m_socketHandle);
}
 //发送请求
int Socket::request(URL *url)
{


    return 0;
    int need, begin, n;
    char request[1024] = {0};
    Url *url = (Url *)arg;

    sprintf(request, "GET /%s HTTP/1.0\r\n"
            "Host: %s\r\n"
            "Accept: */*\r\n"
            "Connection: Keep-Alive\r\n"
            "User-Agent: Mozilla/5.0 (compatible; Qteqpidspider/1.0;)\r\n"
            "Referer: %s\r\n\r\n", url->path, url->domain, url->domain);

    need = strlen(request);
    begin = 0;
    while(need) {
        n = write(m_socketHandle, request+begin, need);
        if (n <= 0) {
            if (errno == EAGAIN) { //write buffer full, delay retry
                usleep(1000);
                continue;
            }
            SPIDER_LOG(SPIDER_LEVEL_WARN, "Thread %lu send ERROR: %d", pthread_self(), n);
            free_url(url);
            close(m_socketHandle);
            return -1;
        }
        begin += n;
        need -= n;
    }
    return 0;
}
//接受反馈
int Socket::respounce()
{

}
//设置socket
int Socket::setSocket()
{
    int flag;
    if ((flag = fcntl(m_socketHandle, F_GETFL)) < 0) {
        SPIDER_LOG(SPIDER_LEVEL_ERROR, "fcntl getfl fail");
    }
    flag |= O_NONBLOCK;
    if ((flag = fcntl(m_socketHandle, F_SETFL, flag)) < 0) {
        SPIDER_LOG(SPIDER_LEVEL_ERROR, "fcntl setfl fail");
    }
}
//链接操作
int Socket::connect()
{

}
//断开链接操作
int Socket::disconnect()
{

}
void* Socket::getData()
{
    begin_thread();

    int i, n, trunc_head = 0, len = 0;
    char * body_ptr = NULL;
    evso_arg * narg = (evso_arg *)arg;
    Response *resp = (Response *)malloc(sizeof(Response));
    resp->header = NULL;
    resp->body = (char *)malloc(HTML_MAXLEN);
    resp->body_len = 0;
    resp->url = narg->url;

    regex_t re;
    if (regcomp(&re, HREF_PATTERN, 0) != 0) {/* compile error */
        SPIDER_LOG(SPIDER_LEVEL_ERROR, "compile regex error");
    }

    SPIDER_LOG(SPIDER_LEVEL_INFO, "Crawling url: %s/%s", narg->url->domain, narg->url->path);

    while(1) {
        /* what if content-length exceeds HTML_MAXLEN? */
        n = read(narg->fd, resp->body + len, 1024);
        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) { 
                /**
                 * TODO: Why always recv EAGAIN?
                 * should we deal EINTR
                 */
                //SPIDER_LOG(SPIDER_LEVEL_WARN, "thread %lu meet EAGAIN or EWOULDBLOCK, sleep", pthread_self());
                usleep(100000);
                continue;
            } 
            SPIDER_LOG(SPIDER_LEVEL_WARN, "Read socket fail: %s", strerror(errno));
            break;

        } else if (n == 0) {
            /* finish reading */
            resp->body_len = len;
            if (resp->body_len > 0) {
                extract_url(&re, resp->body, narg->url);
            }
            /* deal resp->body */
            for (i = 0; i < (int)modules_post_html.size(); i++) {
                modules_post_html[i]->handle(resp);
            }

            break;

        } else {
            //SPIDER_LOG(SPIDER_LEVEL_WARN, "read socket ok! len=%d", n);
            len += n;
            resp->body[len] = '\0';

            if (!trunc_head) {
                if ((body_ptr = strstr(resp->body, "\r\n\r\n")) != NULL) {
                    *(body_ptr+2) = '\0';
                    resp->header = parse_header(resp->body);
                    if (!header_postcheck(resp->header)) {
                        goto leave; /* modulues filter fail */
                    }
                    trunc_head = 1;

                    /* cover header */
                    body_ptr += 4;
                    for (i = 0; *body_ptr; i++) {
                        resp->body[i] = *body_ptr;
                        body_ptr++;
                    }
                    resp->body[i] = '\0';
                    len = i;
                } 
                continue;
            }
        }
    }

leave:
    close(narg->fd); /* close socket */
    free_url(narg->url); /* free Url object */
    regfree(&re); /* free regex object */
    /* free resp */
    free(resp->header->content_type);
    free(resp->header);
    free(resp->body);
    free(resp);

    end_thread();
    return NULL;
}



