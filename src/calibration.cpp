#include "calibration.hpp"
#include <opencv2/calib3d.hpp>
#include <cmath>
#include <stdexcept>

namespace stereo {
static void require(bool yes, const char* message) {
    if (!yes) throw std::invalid_argument(message);
}
void validate(const Dataset& d) {
    require(d.image_size.width > 0 && d.image_size.height > 0 &&
            d.image_size.width <= 32768 && d.image_size.height <= 32768,
            "image dimensions must be in 1..32768");
    require(d.board_size.width >= 3 && d.board_size.height >= 3 &&
            d.board_size.width <= 30 && d.board_size.height <= 30,
            "board dimensions must be in 3..30 inner corners");
    require(std::isfinite(d.square_size) && d.square_size > 0,
            "square_size must be finite and positive");
    require(d.unit == "mm" || d.unit == "cm" || d.unit == "m", "unit must be mm, cm or m");
    require(d.left.size() >= 6 && d.left.size() <= 100 && d.left.size() == d.right.size(),
            "provide 6..100 paired views (application policy, not a mathematical minimum)");
    for (const auto* side : {&d.left, &d.right}) for (const auto& points : *side) {
        require(points.size() == static_cast<size_t>(d.board_size.area()), "wrong corner count");
        for (const auto& p : points)
            require(std::isfinite(p.x) && std::isfinite(p.y) && p.x >= 0 && p.y >= 0 &&
                    p.x < d.image_size.width && p.y < d.image_size.height,
                    "corners must be finite and inside the image");
    }
}
std::vector<cv::Point3d> boardPoints(const Dataset& d) {
    std::vector<cv::Point3d> points;
    for (int y=0; y<d.board_size.height; ++y)
        for (int x=0; x<d.board_size.width; ++x)
            points.emplace_back(x*d.square_size, y*d.square_size, 0);
    return points;
}
static double number(const cv::FileNode& root, const char* key) {
    auto n = root[key];
    require(n.isInt() || n.isReal(), "missing or nonnumeric required field");
    double v = static_cast<double>(n);
    require(std::isfinite(v), "nonfinite number");
    return v;
}
static int integer(const cv::FileNode& root, const char* key, int lo, int hi) {
    double v = number(root, key);
    require(v >= lo && v <= hi && v == std::floor(v), "integer field outside allowed range");
    return static_cast<int>(v);
}
Dataset readDataset(const cv::FileNode& root) {
    require(root.isMap(), "request must be an object");
    require(integer(root, "schema_version", 1, 1)==1, "unsupported schema");
    Dataset d;
    d.image_size={integer(root,"image_width",1,32768),integer(root,"image_height",1,32768)};
    d.board_size={integer(root,"board_cols",3,30),integer(root,"board_rows",3,30)};
    d.square_size=number(root,"square_size");
    require(root["unit"].isString(), "unit must be a string");
    root["unit"] >> d.unit;
    auto views=root["views"];
    require(views.isSeq() && views.size()>=6 && views.size()<=100, "views must contain 6..100 pairs");
    for (auto view : views) {
        require(view.isMap(), "view must be an object");
        for (const char* key : {"left", "right"}) {
            auto list=view[key];
            require(list.isSeq() && list.size()==static_cast<size_t>(d.board_size.area()),
                    "each side needs board_cols * board_rows [x,y] pairs");
            std::vector<cv::Point2d> points;
            for (auto p : list) {
                require(p.isSeq() && p.size()==2, "point must be [x,y]");
                require((p[0].isReal() || p[0].isInt()) && (p[1].isReal() || p[1].isInt()),
                        "point coordinates must be numeric");
                points.emplace_back(static_cast<double>(p[0]),static_cast<double>(p[1]));
            }
            (std::string(key)=="left" ? d.left : d.right).push_back(points);
        }
    }
    validate(d);
    return d;
}
void writeDataset(cv::FileStorage& fs, const Dataset& d) {
    validate(d);
    fs << "schema_version" << 1 << "image_width" << d.image_size.width
       << "image_height" << d.image_size.height << "board_cols" << d.board_size.width
       << "board_rows" << d.board_size.height << "square_size" << d.square_size << "unit" << d.unit;
    fs << "views" << "[";
    for (size_t i=0;i<d.left.size();++i) {
        fs << "{";
        for (const char* key : {"left","right"}) {
            fs << key << "[";
            for (auto p : (std::string(key)=="left" ? d.left[i] : d.right[i]))
                fs << "[:" << p.x << p.y << "]";
            fs << "]";
        }
        fs << "}";
    }
    fs << "]";
}
Result calibrate(const Dataset& d) {
    validate(d);
    std::vector<std::vector<cv::Point3d>> object(d.left.size(),boardPoints(d));
    Result r;
    r.K1=cv::Mat::eye(3,3,CV_64F); r.K2=r.K1.clone();
    r.D1=cv::Mat::zeros(4,1,CV_64F); r.D2=r.D1.clone();
    std::vector<cv::Vec3d> rotations, translations;
    const int flags=cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC | cv::fisheye::CALIB_CHECK_COND |
                    cv::fisheye::CALIB_FIX_SKEW;
    const cv::TermCriteria criteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 150, 1e-9);
    r.rms_left=cv::fisheye::calibrate(object,d.left,d.image_size,r.K1,r.D1,rotations,translations,flags,criteria);
    r.rms_right=cv::fisheye::calibrate(object,d.right,d.image_size,r.K2,r.D2,rotations,translations,flags,criteria);
    r.rms_stereo=cv::fisheye::stereoCalibrate(object,d.left,d.right,r.K1,r.D1,r.K2,r.D2,
        d.image_size,r.R,r.T,cv::fisheye::CALIB_FIX_INTRINSIC,criteria);
    for (auto* m : {&r.K1,&r.K2,&r.D1,&r.D2,&r.R,&r.T})
        if (!cv::checkRange(*m)) throw std::runtime_error("solver returned nonfinite parameters");
    if (!std::isfinite(r.rms_stereo) || !std::isfinite(r.rms_left) || !std::isfinite(r.rms_right))
        throw std::runtime_error("solver returned nonfinite RMS");
    r.baseline=cv::norm(r.T);
    r.camera2_center_in_camera1=-r.R.t()*r.T;
    return r;
}
// Plain row-major arrays make the JSON independent of OpenCV's matrix encoding.
static void matrix(cv::FileStorage& fs, const char* key, const cv::Mat& m) {
    fs << key << "[";
    for (int i=0;i<m.rows;++i) {
        fs << "[:";
        for (int j=0;j<m.cols;++j) fs << m.at<double>(i,j);
        fs << "]";
    }
    fs << "]";
}
void writeResult(cv::FileStorage& fs, const Dataset& d, const Result& r) {
    fs << "schema_version" << 1 << "model" << "opencv_fisheye_kb4"
       << "transform" << "X_right = R * X_left + T" << "unit" << d.unit
       << "image_width" << d.image_size.width << "image_height" << d.image_size.height
       << "board_cols" << d.board_size.width << "board_rows" << d.board_size.height
       << "square_size" << d.square_size << "views_used" << static_cast<int>(d.left.size())
       << "baseline" << r.baseline << "rms_left_px" << r.rms_left
       << "rms_right_px" << r.rms_right << "rms_stereo_px" << r.rms_stereo;
    matrix(fs,"K_left",r.K1); matrix(fs,"D_left",r.D1);
    matrix(fs,"K_right",r.K2); matrix(fs,"D_right",r.D2);
    matrix(fs,"R",r.R); matrix(fs,"T",r.T);
    matrix(fs,"camera2_center_in_camera1",r.camera2_center_in_camera1);
}
std::string resultJson(const Dataset& d,const Result& r) {
    cv::FileStorage fs(".json",cv::FileStorage::WRITE | cv::FileStorage::MEMORY | cv::FileStorage::FORMAT_JSON);
    writeResult(fs,d,r);
    return fs.releaseAndGetString();
}
}
