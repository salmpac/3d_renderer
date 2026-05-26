#pragma once

#include <app/random.h>
#include <world/mesh.h>

#include <filesystem>
#include <fstream>

namespace renderer {

class ObjReader {
   public:
    explicit ObjReader(std::filesystem::path path);

    Mesh read();
    ObjReader& operator>>(Mesh& mesh);

   private:
    std::filesystem::path path_;
    std::ifstream file_;
    Random random_;
};

}  // namespace renderer
