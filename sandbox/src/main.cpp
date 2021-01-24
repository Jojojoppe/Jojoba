#include <jojoba/jojoba.hpp>

int main(int argc, char ** argv){
    JJInstance instance(600, 600);

    instance.loadObject("../sandbox/assets/cube2.obj", "cube1");

    while(instance.shouldClose()){
        instance.render();
    }
}