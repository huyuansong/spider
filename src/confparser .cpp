#include<stdio.h>
#define INF 0x7FFFFFFF
ConfigParser::ConfigParser()
{
    job_num = 10;
    seed = NULL;
    deeps = 0;
    log_level = 0;
    Module_path = NULL;
}
int ConfigParser::loder(char* conf_filepath)
{
    FILE *fp = NULL;
    char buf[MAX_CONF_LEN+1];
    int argc = 0;
    char **argv = NULL;
    int linenum = 0;
    char *line = NULL;
    const char *err = NULL;

    if ((fp = fopen(CONF_FILE, "r")) == NULL) {
        SPIDER_LOG(SPIDER_LEVEL_ERROR, "Can't load conf_file %s", CONF_FILE);
	return 1;
    } 


    while (fgets(buf, MAX_CONF_LEN+1, fp) != NULL) {
        linenum++;
        line = strim(buf);

        if (line[0] == '#' || line[0] == '\0') continue;

        argv = strsplit(line, '=', &argc, 1);
        if (argc == 2) {
            if (strcasecmp(argv[0], "max_job_num") == 0) {
                this->job_num = atoi(argv[1]);
            } else if (strcasecmp(argv[0], "seeds") == 0) {
                this->seeds = strdup(argv[1]);
            } else if (strcasecmp(argv[0], "module_path") == 0) {
                this->module_path = strdup(argv[1]);
            } else if (strcasecmp(argv[0], "load_module") == 0) {
                this->modules.push_back(strdup(argv[1]));
            } else if (strcasecmp(argv[0], "log_level") == 0) {
                this->log_level = atoi(argv[1]);
            } else if (strcasecmp(argv[0], "max_depth") == 0) {
                this->depth = atoi(argv[1]);
            } else if (strcasecmp(argv[0], "accept_types") == 0) {
                this->file_type.push_back(strdup(argv[1]));
            } else {
                err = "Unknown directive"; goto conferr;
            }
        } else {
            err = "directive must be 'key=value'"; goto conferr;
        }

    }
    return 0;
}
// for single
ConfigParser* ConfigParser::instance()
{
    if(__self == NULL)
    {
        __self = new ConfigParser();
    }

}
static void ConfigParser::release()
{
	delete __self;
}
int ConfigParser::getJobNum()
{

}
char ConfigParser::getUrlSeed()
{

}
int ConfigParser::getDeep()
{

}
int ConfigParser::getLogLevel()
{

}
char* ConfigParser::getmodulePath()
{

}
list<string> ConfigParser::getModules()
{

}
list<string> ConfigParser::getFileTypes()
{

}

