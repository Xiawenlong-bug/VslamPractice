/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-20 11:31:54
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-20 16:00:21
 * @FilePath: /VslamPractice/vslam/include/frame.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <opencv2/opencv.hpp>
#include "common_include.h"
namespace MYSLAM{
    //前向声明
    struct MapPoint;
    struct Feature;

    struct Frame{
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
            typedef std::shared_ptr<Frame> Ptr;
            unsigned long id_ = 0;
            unsigned long keyframe_id_ = 0;

            bool is_keyframe_=true;
            double time_stamp_;
            SE3 pose_;
            std::mutex pose_mutex_;
            Mat left_img_, right_img_;

            std::vector<std::shared_ptr<Feature>> features_left_;
            std::vector<std::shared_ptr<Feature>> features_right_;

            Frame(){}
            Frame(unsigned long id, double time_stamp, const SE3 &pose, const Mat &left_img, const Mat &right_img);

            SE3 Pose(){
                std::unique_lock<std::mutex> lck(pose_mutex_);
                return pose_;
            }

            void SetPose(const SE3 &pose){
                std::unique_lock<std::mutex> lck(pose_mutex_);
                pose_ = pose;
            }

            void SetKeyFrame();

            static std::shared_ptr<Frame> CreateFrame();
    };
}