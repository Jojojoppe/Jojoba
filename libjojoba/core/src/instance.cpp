#include <jojoba/instance.hpp>
#include <vermilion/instance.hpp>

Jojoba::Core::Instance::Instance(unsigned int width, unsigned int height){
    int hintTypes[]={
        Vermilion::Core::HintType::HINT_TYPE_WINDOW_WIDTH,
        Vermilion::Core::HintType::HINT_TYPE_WINDOW_HEIGHT,
        Vermilion::Core::HintType::HINT_TYPE_LOGLEVEL,
    0};
    int hintValues[]={
        (int)width,
        (int)height,
        VMCORE_LOGLEVEL_DEBUG,
    0};
    vmInstance.reset(new VmInstance(hintTypes, hintValues, this, Vermilion::Core::WindowCallbackFunctions{}));
}

Jojoba::Core::Instance::~Instance(){
    vmInstance.reset();
}

bool Jojoba::Core::Instance::shouldClose(){
    return vmInstance->shouldClose();
}

void Jojoba::Core::Instance::render(){
    vmInstance->startRender();
    vmInstance->endRender({});
}