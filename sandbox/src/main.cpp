#include <jojoba/jojoba.hpp>

int main(int argc, char ** argv){
    JJInstance instance(600, 600);

    unsigned int cube1 = instance.loadObject("../sandbox/assets/cube2.obj");

    while(instance.shouldClose()){
        instance.render();
    }
}