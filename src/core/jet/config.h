#pragma once
#include "jet.h"
#include "packet.h"
//#include "array.h"
#include "pad.h"

//typedef struct {
//} jconf_core_t;

namespace jet {

class config
{
public:
    config();
    ~config();

private:
    std::vector<std::string> registry_paths;
    std::string cwd;

};


} // namespace jet

//typedef struct {
//    //jconf_core_t core;
//    jpad_seginfo_t def_seginfo;
//
//} jconf_t;
//
//int
//jconf_init();
//jconf_t *
//jconf_data();

