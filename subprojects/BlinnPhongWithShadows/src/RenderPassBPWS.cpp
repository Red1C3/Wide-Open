#include<RenderPassBPWS.h>
using namespace WideOpenBPWS;
RenderPassBPWS::RenderPassBPWS(){}
void RenderPassBPWS::setupAttachments(){

}
void RenderPassBPWS::setupSubPasses(){

}
void RenderPassBPWS::setupFramebuffers(){

}
RenderPassBPWS& RenderPassBPWS::instance(){
    static RenderPassBPWS instance;
    return instance;
}