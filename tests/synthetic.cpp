#include "calibration.hpp"
#include <opencv2/calib3d.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

static void check(bool yes,const char* message) {
    if (!yes) throw std::runtime_error(message);
}
int main(int argc,char** argv) {
    try {
        stereo::Dataset d;
        d.image_size={1280,960}; d.board_size={9,6}; d.square_size=30; d.unit="mm";
        const cv::Mat K=(cv::Mat_<double>(3,3)<<460,0,640,0,465,480,0,0,1);
        const cv::Mat D=(cv::Mat_<double>(4,1)<<-0.04,0.005,-0.001,0.0001);
        cv::Mat trueR; cv::Rodrigues(cv::Vec3d(0.01,-0.035,0.008),trueR);
        const cv::Mat trueT=(cv::Mat_<double>(3,1)<<-120,3,4);
        auto board=stereo::boardPoints(d);
        for (int i=0;i<24;++i) {
            cv::Mat R1,R2,r2;
            cv::Vec3d r1(0.35*std::sin(i*0.7),0.4*std::cos(i*0.9),0.15*std::sin(i*0.4));
            cv::Mat t1=(cv::Mat_<double>(3,1)<<-120+140*std::sin(i*1.1),-75+100*std::cos(i*0.8),430+9*i);
            cv::Rodrigues(r1,R1); R2=trueR*R1; cv::Rodrigues(R2,r2);
            cv::Mat t2=trueR*t1+trueT;
            std::vector<cv::Point2d> a,b;
            cv::fisheye::projectPoints(board,a,r1,t1,K,D);
            cv::fisheye::projectPoints(board,b,r2,t2,K,D);
            d.left.push_back(a); d.right.push_back(b);
        }
        if (argc==2) {
            cv::FileStorage fs(argv[1],cv::FileStorage::WRITE);
            check(fs.isOpened(),"cannot write synthetic request");
            stereo::writeDataset(fs,d);
        }
        auto r=stereo::calibrate(d);
        check(cv::norm(r.R-trueR)<1e-4,"rotation recovery failed");
        check(cv::norm(r.T-trueT)<0.02,"translation recovery failed");
        check(std::abs(r.baseline-cv::norm(trueT))<0.02,"baseline recovery failed");
        check(r.rms_stereo<1e-3,"unexpected synthetic residual");
        check(cv::norm(r.R*r.camera2_center_in_camera1+r.T)<1e-8,"inverse pose convention failed");
        auto scaled=d; scaled.square_size*=0.001; scaled.unit="m";
        auto s=stereo::calibrate(scaled);
        check(cv::norm(s.R-r.R)<1e-4,"unit conversion changed rotation");
        check(cv::norm(s.T-r.T*0.001)<0.00002,"metric scale conversion failed");
        cv::FileStorage memory(".json",cv::FileStorage::WRITE | cv::FileStorage::MEMORY | cv::FileStorage::FORMAT_JSON);
        stereo::writeDataset(memory,d);
        cv::FileStorage read(memory.releaseAndGetString(),cv::FileStorage::READ | cv::FileStorage::MEMORY);
        auto roundtrip=stereo::readDataset(read.root());
        check(roundtrip.left.size()==d.left.size() && cv::norm(roundtrip.left[0][0]-d.left[0][0])<1e-9,
              "request serialization failed");
        auto bad=d; bad.left[0].pop_back();
        bool rejected=false;
        try { stereo::calibrate(bad); } catch (const std::invalid_argument&) { rejected=true; }
        check(rejected,"bad point count accepted");
        bad=d; bad.square_size=0; rejected=false;
        try { stereo::validate(bad); } catch (const std::invalid_argument&) { rejected=true; }
        check(rejected,"zero scale accepted");
        std::cout << "PASS: synthetic rotation, translation, baseline, metric scale, inverse pose, JSON, invalid inputs.\n"
                  << "Expected baseline=" << cv::norm(trueT) << " mm; recovered=" << r.baseline
                  << " mm; RMS=" << r.rms_stereo << " px\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
