#pragma once

#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>

#include <vermilion/glm/glm.hpp>

namespace Jojoba{
namespace Core{

class ImGui_vermilion;
class AssetManager;

class Instance{
    public:
        std::shared_ptr<AssetManager> assetManager;

        Instance(unsigned int width, unsigned int height);
        ~Instance();

        bool shouldClose();
        void render();

        // VmInstance callback functions
        static void resize(VmInstance * instance, void * userPointer, int width, int height);
        static void mouseButton(VmInstance * instance, void * userPointer, Vermilion::Core::WindowMouseButton btn, Vermilion::Core::WindowMouseAction action);
        static void mousePos(VmInstance * instance, void * userPointer, double x, double y);
        static void mouseEnter(VmInstance * instance, void * userPointer, bool enter);
        static void scroll(VmInstance * instance, void * userPointer, double x, double y);

    private:
        std::shared_ptr<VmInstance> vmInstance;
        VmRenderTarget defaultTarget;

        std::shared_ptr<ImGui_vermilion> imgui;

        glm::mat4 model, view, projection;

    friend class ::JJInstance;
};

}}