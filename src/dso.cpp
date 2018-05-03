#include <dlfcn.h>
#include <malloc.h>
#include <stdlib.h>
#include "dso.h"
#include "spider.h" 
#include "qstring.h"

ModuleManager::ModuleManager()
{

}

ModuleManager::~ModuleManager()
{

}


int ModuleManager::load(string path, string name )
{
   void *rv = NULL;
    void *handle = NULL;
    Module *module = NULL;

    char * npath = strcat2(3, path, name, ".so");

    if ((handle = dlopen(npath, RTLD_GLOBAL | RTLD_NOW)) == NULL) {	
        SPIDER_LOG(SPIDER_LEVEL_ERROR, "Load module fail(dlopen): %s", dlerror());
        return MODULE_ERROR;
    }
    if ((rv = dlsym(handle, name)) == NULL) {
        SPIDER_LOG(SPIDER_LEVEL_ERROR, "Load module fail(dlsym): %s", dlerror());
        return MODULE_ERROR;
    }
    module = (Module *)rv;
    module->init(module);

    this->modules.insert(name, module);
    return MODULE_OK;
}

MODULE *ModuleManager::getModule(string)
{
    

}



