#include<stdio.h>

class ConfigParser
{
public:
    //ConfigParser();
    int loder(char* conf_filepath);
    // for single
    static ConfigParser* instance();
    static void release();

    //opratons
    int getJobNum();
    char getUrlSeed();
    int getDeep();
    int getLogLevel();
    char* getmodulePath();
    list<string> getModules();
    list<string> getFileTypes();
private: //for single
    Configparser();
    static ConfigParser* __self;

private:
    int job_num;
    char* seed;
    int depth;
    int log_level;
    char* module_path;
    list<string> modules;
    list<string> file_type;
};
ConfigParser::__self = NULL;
