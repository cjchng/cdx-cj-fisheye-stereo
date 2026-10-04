#include "calibration.hpp"
#include "httplib.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <stdexcept>

static std::string quote(const std::string& text) {
    std::ostringstream out; out << '"';
    for (unsigned char c : text) {
        if (c=='"' || c=='\\') out << '\\' << c;
        else if (c<0x20) out << "\\u00" << std::hex << std::setw(2) << std::setfill('0') << int(c);
        else out << c;
    }
    out << '"'; return out.str();
}
static void error(httplib::Response& res,int status,const char* code,const std::string& message) {
    res.status=status;
    res.set_content("{\"error\":{\"code\":"+quote(code)+",\"message\":"+quote(message)+"}}","application/json");
}
int main(int argc,char** argv) {
    try {
        int port=8080;
        if (argc>2) throw std::invalid_argument("usage: fisheye_server [port]");
        if (argc==2) {
            size_t end=0; port=std::stoi(argv[1],&end);
            if (end!=std::string(argv[1]).size()) throw std::invalid_argument("invalid port");
        }
        if (port<1024 || port>65535) throw std::invalid_argument("port must be 1024..65535");
        const char* bind_env=std::getenv("FISHEYE_BIND_ADDRESS");
        const std::string bind_address=bind_env ? bind_env : "127.0.0.1";
        if (bind_address!="127.0.0.1" && bind_address!="0.0.0.0")
            throw std::invalid_argument("FISHEYE_BIND_ADDRESS must be 127.0.0.1 or 0.0.0.0");
        httplib::Server server;
        std::mutex calibration_mutex;
        server.set_payload_max_length(4*1024*1024);
        server.set_read_timeout(30,0);
        server.set_error_handler([](const auto&, auto& res) {
            if (res.body.empty()) error(res,res.status,"HTTP_ERROR","request rejected");
        });
        server.Get("/health",[](const auto&, auto& res) {
            res.set_content("{\"status\":\"ok\",\"schema_version\":1}","application/json");
        });
        server.Post("/v1/calibrate",[&](const auto& req, auto& res) {
            const auto type=req.get_header_value("Content-Type");
            if (type!="application/json" && type.find("application/json;")!=0) {
                error(res,415,"UNSUPPORTED_MEDIA_TYPE","use application/json"); return;
            }
            std::unique_lock<std::mutex> lock(calibration_mutex,std::try_to_lock);
            if (!lock.owns_lock()) { error(res,429,"BUSY","one calibration at a time; retry later"); return; }
            stereo::Dataset d;
            try {
                cv::FileStorage fs(req.body,cv::FileStorage::READ | cv::FileStorage::MEMORY | cv::FileStorage::FORMAT_JSON);
                if (!fs.isOpened()) throw std::invalid_argument("invalid JSON");
                d=stereo::readDataset(fs.root());
            } catch (const std::exception& e) { error(res,400,"INVALID_INPUT",e.what()); return; }
            try {
                auto r=stereo::calibrate(d);
                res.set_content(stereo::resultJson(d,r),"application/json");
            } catch (const cv::Exception& e) {
                error(res,422,"CALIBRATION_FAILED",e.what());
            } catch (const std::exception& e) {
                error(res,500,"INTERNAL_ERROR",e.what());
            }
        });
        std::cout << "Listening on http://" << bind_address << ':' << port << " (Ctrl-C to stop)" << std::endl;
        if (!server.listen(bind_address,port)) throw std::runtime_error("cannot bind port");
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
