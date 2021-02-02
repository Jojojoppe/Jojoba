#include <jojoba/instance.hpp>
#include <vermilion/instance.hpp>
#include <vermilion/glm/glm.hpp>
#include <vermilion/glm/gtc/matrix_transform.hpp>
#include "imgui/imgui_vermilion.hpp"
#include "imgui/imgui.h"

#include "asset_manager.hpp"


Jojoba::Core::Instance::Instance(unsigned int width, unsigned int height){
    int hintTypes[]={
        Vermilion::Core::HintType::HINT_TYPE_WINDOW_WIDTH,
        Vermilion::Core::HintType::HINT_TYPE_WINDOW_HEIGHT,
        Vermilion::Core::HintType::HINT_TYPE_LOGLEVEL,
        Vermilion::Core::HintType::HINT_TYPE_RENDER_PLATFORM,
    0};
    int hintValues[]={
        (int)width,
        (int)height,
        VMCORE_LOGLEVEL_DEBUG,
        Vermilion::Core::RenderPlatform::RENDER_PLATFORM_OPENGL,
    0};
    vmInstance.reset(new VmInstance(hintTypes, hintValues, this, Vermilion::Core::WindowCallbackFunctions{resize, mouseButton, mousePos, mouseEnter, scroll}));
    vmInstance->getDefaultRenderTarget(defaultTarget);

    imgui.reset(new Jojoba::Core::ImGui_vermilion(vmInstance, width, height));
    assetManager.reset(new Jojoba::Core::AssetManager(vmInstance));
}

Jojoba::Core::Instance::~Instance(){
    assetManager.reset();
    imgui.reset();
    vmInstance.reset();
}

bool Jojoba::Core::Instance::shouldClose(){
    return vmInstance->shouldClose();
}

void Jojoba::Core::Instance::render(){
    vmInstance->startRender();
        defaultTarget.start(0.0, 0.0, 0.0, 1.0);

            // Add gui
            ImGui::ShowDemoWindow();
            imgui->render();
        defaultTarget.end();
    vmInstance->endRender({});
}

// ------------------------------------------------------------------------

void Jojoba::Core::Instance::resize(VmInstance * instance, void * userPointer, int width, int height){
	Jojoba::Core::Instance * app = (Jojoba::Core::Instance*) userPointer;
	app->imgui->resize(width, height);
}

void Jojoba::Core::Instance::mouseButton(VmInstance * instance, void * userPointer, Vermilion::Core::WindowMouseButton btn, Vermilion::Core::WindowMouseAction action){
	Jojoba::Core::Instance * app = (Jojoba::Core::Instance*) userPointer;
	app->imgui->mouseButton(btn, action);
}

void Jojoba::Core::Instance::mousePos(VmInstance * instance, void * userPointer, double x, double y){
	Jojoba::Core::Instance * app = (Jojoba::Core::Instance*) userPointer;
	app->imgui->mousePos(x, y);
}

void Jojoba::Core::Instance::mouseEnter(VmInstance * instance, void * userPointer, bool enter){
}

void Jojoba::Core::Instance::scroll(VmInstance * instance, void * userPointer, double x, double y){
	Jojoba::Core::Instance * app = (Jojoba::Core::Instance*) userPointer;
	app->imgui->scroll(x, y);
}