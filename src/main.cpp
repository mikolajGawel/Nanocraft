#include "Game.hpp"
#include <memory>
int main(){
    std::unique_ptr<Nanocraft> nanocraft = std::make_unique<Nanocraft>(1280,720,10,true,true);
    nanocraft->run();

}
