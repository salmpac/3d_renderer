#include <geometry/point3d.h>
#include <geometry/triangle.h>
#include <io/obj_reader.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace renderer {

namespace {
int parse_vertex_index(const std::string& token) {
    auto slash = token.find('/');
    std::string number = (slash == std::string::npos) ? token : token.substr(0, slash);
    return std::stoi(number);
}
}  // namespace

ObjReader::ObjReader(std::filesystem::path path) : path_(std::move(path)), file_(path_) {
    if (!file_.is_open()) {
        throw std::runtime_error("ObjReader: failed to open " + path_.string());
    }
}

Mesh ObjReader::read() {
    Mesh mesh;
    *this >> mesh;
    return mesh;
}

ObjReader& ObjReader::operator>>(Mesh& mesh) {
    std::vector<Point3D> vertices;
    std::string line;
    while (std::getline(file_, line)) {
        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;
        if (prefix == "v") {
            FloatType x = 0;
            FloatType y = 0;
            FloatType z = 0;
            stream >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        } else if (prefix == "f") {
            std::vector<int> indices;
            std::string token;
            while (stream >> token) {
                int idx = parse_vertex_index(token);
                if (idx < 0) {
                    idx = static_cast<int>(vertices.size()) + idx;
                } else {
                    idx -= 1;
                }
                indices.push_back(idx);
            }
            if (indices.size() < 3) {
                continue;
            }
            Color color = random_.uniform_color();
            for (std::size_t i = 1; i + 1 < indices.size(); ++i) {
                mesh.add(Triangle{vertices[indices[0]], vertices[indices[i]], vertices[indices[i + 1]], color});
            }
        }
    }
    return *this;
}

}  // namespace renderer
