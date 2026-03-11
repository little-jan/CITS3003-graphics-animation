#ifndef SHADER_HELPER_H
#define SHADER_HELPER_H

#include <string>
#include <optional>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <functional>

#include <glad/gl.h>

// A useful shorthand that not all compilers provide by default
using uint = unsigned int;

/// A shader helper that loads files, compiles and links for you. Also prints any errors.
class ShaderHelper {
    static const std::string SHADER_DIR;

public:
    static uint load_shader(const std::string& vertex_path, const std::string& fragment_path);
private:
    static std::optional<std::string> load_shader_file(const std::string& shader_path);

    static std::optional<uint> compile_shader_code(const std::string& shader_code, uint shader_type);

    static std::optional<uint> link_program(uint vertex_shader, uint fragment_shader);
};

#endif //SHADER_HELPER_H
