//
//  ModuleTypes.h
//  RubetekEvo
//
//  Created by Andrey Verizhnikov on 27.08.15.
//
//
#pragma once

#include <string>

#ifndef RubetekEvo_ModuleTypes_h
#define RubetekEvo_ModuleTypes_h

struct ModuleDevice {
    std::string did;
    std::string name;
    std::string module;
    std::string model;
    std::string devType;
    std::string parentId;
};

#endif
