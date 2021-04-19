#include<RendererBPWS.h>
using namespace WideOpenBPWS;
RendererBPWS::RendererBPWS(){}
RendererBPWS& RendererBPWS::instance(){
    static RendererBPWS instance;
    return instance;
}
void RendererBPWS::createDescriptorPool(){
    //TODO implement
}