#include <jojoba/jojoba.hpp>
#include <jojoba/instance.hpp>
#include "asset_manager.hpp"

JJInstance::JJInstance(unsigned int width, unsigned int height){
    instance.reset(new Jojoba::Core::Instance(width, height));
}

JJInstance::~JJInstance(){
    instance.reset();
}

bool JJInstance::shouldClose(){
    return instance->shouldClose();
}

void JJInstance::render(){
    instance->render();
}

unsigned int JJInstance::loadObject(const std::string& path){
    return instance->assetManager->loadObjectFromObjFile(path);
}