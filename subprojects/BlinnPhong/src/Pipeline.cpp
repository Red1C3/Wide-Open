#include<Pipeline.h>
using namespace WideOpenBP;
Pipeline::Pipeline(){}
Pipeline& Pipeline::instance(){
    static Pipeline pipeline;
    return pipeline;
}
void Pipeline::init(){
    
}
void Pipeline::terminate(){

}