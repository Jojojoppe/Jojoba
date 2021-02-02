#include <jojoba/jojoba.hpp>
#include <jojoba/instance.hpp>
#include "asset_manager.hpp"

// -----

JJInstance::JJInstance(unsigned int width, unsigned int height){
    instance.reset(new Jojoba::Core::Instance(width, height));
}

JJInstance::~JJInstance(){
    instance.reset();
}

bool JJInstance::shouldClose(){
    return instance->shouldClose();
}

void JJInstance::render(JJScene& scene){
    instance->render(scene);
}

unsigned int JJInstance::loadObject(const std::string& path){
    return instance->assetManager->loadObjectFromObjFile(path);
}

// -----

JJScene JJInstance::createScene(){
    JJScene s;
    s.ID = instance->assetManager->createScene();
    s.instance = std::weak_ptr<Jojoba::Core::Instance>(this->instance);
    return s;
}

unsigned int JJScene::createEntity(){
    return instance.lock()->assetManager->assetScenes[ID].scene->createEntity();
}

void JJScene::entityAddObject(unsigned int entity, unsigned int object){

}

void JJScene::entityAddCamera(unsigned int entity){

}

void JJScene::useCamera(unsigned int entity){

}

// -----