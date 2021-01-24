#include <jojoba/jojoba.hpp>

int main(int argc, char ** argv){
    JJInstance instance(600, 600);
    
    while(instance.shouldClose()){
        instance.render();
    }
}