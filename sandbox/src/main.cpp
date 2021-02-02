#include <jojoba/jojoba.hpp>

int main(int argc, char ** argv){
    JJInstance instance(600, 600);

    unsigned int cube1 = instance.loadObject("../sandbox/assets/cube2.obj");

    JJScene scene = instance.createScene();

    unsigned int cubeEntity = scene.createEntity();
    scene.entityAddObject(cubeEntity, cube1);
    // will add object and mesh component to entity and add Material:NoMaterial component to entity and add Transform component

    unsigned int camera = scene.createEntity();
    scene.entityAddCamera(camera); //, {camera_options});
    // will add camera component (projection matrix and empty view) and transform to it. When starting to render it calculates the view matrix
    scene.useCamera(camera);

    while(instance.shouldClose()){
        instance.render(scene);
    }
}