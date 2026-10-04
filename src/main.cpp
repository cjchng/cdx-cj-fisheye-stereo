#include "calibration.hpp"
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace fs=std::filesystem;

static cv::FileStorage input(const std::string& path) {
    cv::FileStorage f(path,cv::FileStorage::READ);
    if (!f.isOpened()) throw std::invalid_argument("cannot read "+path);
    return f;
}
static void unused(const fs::path& path) {
    if (fs::exists(path)) throw std::invalid_argument("output already exists: "+path.string());
}
static stereo::Dataset detect(const std::string& config,const fs::path& previews) {
    auto f=input(config);
    stereo::Dataset d;
    f["board_cols"] >> d.board_size.width; f["board_rows"] >> d.board_size.height;
    f["square_size"] >> d.square_size; f["unit"] >> d.unit;
    if (d.board_size.width<3 || d.board_size.width>30 || d.board_size.height<3 || d.board_size.height>30)
        throw std::invalid_argument("board size must be 3..30 INNER corners per dimension");
    if ((int)f["schema_version"]!=1) throw std::invalid_argument("schema_version must be 1");
    auto pairs=f["pairs"];
    if (!pairs.isSeq() || pairs.size()<6 || pairs.size()>100)
        throw std::invalid_argument("provide 6..100 image pairs");
    unused(previews); fs::create_directories(previews);
    const auto base=fs::absolute(config).parent_path();
    int index=0;
    for (auto pair : pairs) {
        for (const char* side : {"left","right"}) {
            std::string file; pair[side] >> file;
            if (file.empty()) throw std::invalid_argument("missing image path");
            cv::Mat gray=cv::imread((base/file).string(),cv::IMREAD_GRAYSCALE);
            if (gray.empty()) throw std::invalid_argument("cannot read image: "+file);
            if (d.image_size.empty()) d.image_size=gray.size();
            if (gray.size()!=d.image_size) throw std::invalid_argument("all images must have the same dimensions");
            std::vector<cv::Point2f> corners;
            if (!cv::findChessboardCornersSB(gray,d.board_size,corners,
                    cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_EXHAUSTIVE | cv::CALIB_CB_ACCURACY))
                throw std::invalid_argument("board detection failed: "+file+"; remove the whole pair or improve capture");
            auto reverse=pair[std::string("reverse_")+side];
            if (!reverse.isInt() || ((int)reverse!=0 && (int)reverse!=1))
                throw std::invalid_argument("every pair needs reverse_left and reverse_right, each 0 or 1");
            if ((int)reverse) std::reverse(corners.begin(),corners.end());
            std::vector<cv::Point2d> points(corners.begin(),corners.end());
            (std::string(side)=="left" ? d.left : d.right).push_back(points);
            cv::Mat overlay; cv::cvtColor(gray,overlay,cv::COLOR_GRAY2BGR);
            cv::drawChessboardCorners(overlay,d.board_size,corners,true);
            for (size_t j=0;j<corners.size();++j)
                cv::putText(overlay,std::to_string(j),corners[j],cv::FONT_HERSHEY_SIMPLEX,0.35,{0,0,255},1);
            if (!cv::imwrite((previews/(std::to_string(index)+"_"+side+".png")).string(),overlay))
                throw std::runtime_error("cannot write preview");
        }
        ++index;
    }
    stereo::validate(d);
    return d;
}
int main(int argc,char** argv) {
    try {
        if (argc==2 && std::string(argv[1])=="--help") {
            std::cout << "detect: fisheye_stereo detect pairs.yml observations.json previews_dir\n"
                         "calibrate: fisheye_stereo calibrate observations.json result.json\n";
            return 0;
        }
        if (argc==5 && std::string(argv[1])=="detect") {
            unused(argv[3]);
            auto d=detect(argv[2],argv[4]);
            cv::FileStorage out(argv[3],cv::FileStorage::WRITE);
            if (!out.isOpened()) throw std::runtime_error("cannot write observations");
            stereo::writeDataset(out,d); out.release();
            std::cout << "Detected " << d.left.size() << " pairs. Inspect matching corner IDs in previews BEFORE calibration.\n";
            return 0;
        }
        if (argc==4 && std::string(argv[1])=="calibrate") {
            unused(argv[3]);
            auto f=input(argv[2]); auto d=stereo::readDataset(f.root());
            auto r=stereo::calibrate(d);
            cv::FileStorage out(argv[3],cv::FileStorage::WRITE);
            if (!out.isOpened()) throw std::runtime_error("cannot write result");
            stereo::writeResult(out,d,r); out.release();
            std::cout << "Baseline: " << r.baseline << ' ' << d.unit << "\nR:\n" << r.R
                      << "\nT:\n" << r.T << "\nStereo training RMS: " << r.rms_stereo << " px\n";
            return 0;
        }
        throw std::invalid_argument("use --help for commands");
    } catch (const std::invalid_argument& e) {
        std::cerr << "INPUT_ERROR: " << e.what() << '\n'; return 2;
    } catch (const cv::Exception& e) {
        std::cerr << "OPENCV_ERROR: " << e.what() << '\n'; return 3;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n'; return 4;
    }
}
