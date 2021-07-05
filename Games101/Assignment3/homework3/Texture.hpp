//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;

        int u0 = floor(u_img), u1 = ceil(u_img);
        int v0 = floor(v_img), v1 = ceil(v_img);
        float su = u_img - u0;
        float sv = v_img - v0;

        auto color00 = image_data.at<cv::Vec3b>(v0, u0);
        if (u1 < width)
        {
            auto color10 = image_data.at<cv::Vec3b>(v0, u1);
            color00 = color00 * (1 - su) + color10 * su;
        }
        if (v1 < height)
        {
            auto color01 = image_data.at<cv::Vec3b>(v1, u0);
            if (u1 < width)
            {
                auto color11 = image_data.at<cv::Vec3b>(v0, u1);
                color00 = color00 * (1 - su) + color11 * su;
            }

            color00 = color00 * (1 - sv) + color01 * sv;
        }

        return Eigen::Vector3f(color00[0], color00[1], color00[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
