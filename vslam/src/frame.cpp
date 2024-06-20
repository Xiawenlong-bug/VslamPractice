/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-20 11:30:02
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-20 16:08:00
 * @FilePath: /VslamPractice/vslam/src/frame.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "frame.h"

namespace MYSLAM{
    Frame::Frame(unsigned long id, double time_stamp, const SE3 &pose, const Mat &left_img, const Mat &right_img) : 
    id_(id), time_stamp_(time_stamp), pose_(pose), left_img_(left_img), right_img_(right_img){};
    Frame::~Frame(){};

    void Frame::SetKeyFrame(){
        static long keyframe_factory_id = 0;
        is_keyframe_ = true;
        keyframe_id_ = keyframe_factory_id++;
    }

    Frame::Ptr Frame::CreateFrame(){
        static long factory_id = 0;
        Frame::Ptr new_frame(new Frame);
        new_frame->id_ = factory_id++;
        return new_frame;
    }
}
