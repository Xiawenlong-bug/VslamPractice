/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-20 16:51:11
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-20 17:58:00
 * @FilePath: /VslamPractice/vslam/src/MapPoint.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "MapPoint.h"
#include "feature.h"
namespace MYSLAM{
    void MapPoint::RemoveObservations(std::shared_ptr<Feature> feat) {
        std::unique_lock<std::mutex> lock(data_mutex_);
        for(auto iter=observations_.begin(); iter!=observations_.end();iter++)
        {
            if(iter->lock() == feat)
            {
                //遍历observation，找到被对应异常点观察到的feature
                observations_.erase(iter);
                feat->map_point_.reset();
                observed_times_--;
                break;
            }
        }

    }

    MapPoint::Ptr CreateNewMapPoint(){
        static int factory_id = 0;
        MapPoint::Ptr new_mappoint(new MapPoint);
        new_mappoint->id_ = factory_id;
        factory_id++;
        return new_mappoint;
    }
}