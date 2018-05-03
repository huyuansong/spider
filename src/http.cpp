#include<string>


using namespace std;


Http::Http()
{

}
Http::~Http()
{

}
//解析http头，参数：下载的全部数据
int Http::parseHead(void* buf /*out*/)
{
    Header *h = (Header *)data;
    int i = 0;

    /* skip if not 2xx */
    if (h->status_code < 200 || h->status_code >= 300)
        return MODULE_ERR;

    if (h->content_type != NULL) {
        if (strstr(h->content_type, "text/html") != NULL)
            return MODULE_OK;

        for (i = 0; i < g_conf->accept_types.size(); i++) {
            if (strstr(h->content_type, g_conf->accept_types[i]) != NULL)
                return MODULE_OK;
        }
        return MODULE_ERR;
    }

    return MODULE_OK;
}

int Http::creatHead()
{

}
//得到下载的内容 传入一个缓冲区地址，需要预先申请
int Http::getContent( void* buf )
{

}
int Http::getUrl(URL* url)
{

}
int Http::init()
{

}
int Http::reinit()
{

}

/*把从http头中获得的信息进行更新*/
int Http::updateUrl(URL* url)
{

}

