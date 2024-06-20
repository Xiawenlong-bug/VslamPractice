/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-20 16:51:11
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-20 17:37:25
 * @FilePath: /VslamPractice/vslam/include/MapPoint.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "common_include.h"

namespace MYSLAM {
    struct Frame;
    struct Feature;

    struct MapPoint {
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
            typedef std::shared_ptr<MapPoint> Ptr;
            unsigned long id_ = 0;
            Vec3 pos_=Vec3::Zero();
            bool is_outlier_ = false;
            std::mutex data_mutex_;
            std::list<std::weak_ptr<Feature>> observations_;
            int observed_times_ = 0;

            MapPoint(){};
            MapPoint(unsigned long id, Vec3 pos):id_(id),pos_(pos){};

            Vec3 Pos(){
                std::unique_lock<std::mutex> lock(data_mutex_);
                return pos_;
            }

            void SetPos(Vec3 pos){
                std::unique_lock<std::mutex> lock(data_mutex_);
                pos_ = pos;
            }

            void AddObservation(std::shared_ptr<Feature> feature){
                std::unique_lock<std::mutex> lock(data_mutex_);
                observations_.push_back(feature);
                observed_times_++;
            }

            void RemoveObservations(std::shared_ptr<Feature> feat);

            static MapPoint::Ptr CreateNewMappoint();
    };
}