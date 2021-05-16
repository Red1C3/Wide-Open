#include<RenderPassCM.h>
using namespace WideOpenCM;
RenderPassCM::RenderPassCM(){}
RenderPassCM& RenderPassCM::instance(){
    static RenderPassCM rp;
    return rp;
}
void RenderPassCM::setupAttachments(){
    //TODO
}
void RenderPassCM::setupSubPasses(){
    //TODO
}
void RenderPassCM::setupFramebuffers(){
    //TODO
}