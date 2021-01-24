#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>

#include <cstdio>

void Jojoba::Core::test(){
    VmInstance instance(nullptr, nullptr, nullptr, Vermilion::Core::WindowCallbackFunctions{});
    while(instance.shouldClose()){
        instance.startRender();
        instance.endRender({});
    }
}