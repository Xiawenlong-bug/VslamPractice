/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-20 16:23:04
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-20 16:49:46
 * @FilePath: /VslamPractice/vslam/include/feature.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEi
 */
#pragma once

#include "memory"
#include "opencv2/features2d.hpp"
#include "common_include.h"

namespace MYSLAM{
    struct Frame;
    struct MapPoint;

    struct Feature
    {
        /* data */
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
            typedef std::shared_ptr<Feature> ptr;

            std::weak_ptr<Feature> frame_;
            std::weak_ptr<MapPoint> map_point_;

            cv::KeyPoint position_;
            bool is_outlier_=false;
            bool is_on_left_image_ = true;

            Feature(){};
            Feature(std::weak_ptr<Feature> frame, const cv::KeyPoint& kp):frame_(frame), position_(kp){};

            
    };
}
