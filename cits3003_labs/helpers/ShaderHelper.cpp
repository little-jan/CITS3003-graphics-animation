#include "ShaderHelper.h"

#include <vector>

const std::string ShaderHelper::SHADER_DIR = "res/shaders";

uint ShaderHelper::load_shader(const std::string &vertex_path, const std::string &fragment_path) {
    auto vertex_code = load_shader_file(SHADER_DIR + "/" + vertex_path).value(); // Will throw exception on failure
    auto fragment_code = load_shader_file(SHADER_DIR + "/" + fragment_path).value(); // Will throw exception on failure

    auto vertexShader = compile_shader_code(vertex_code, GL_VERTEX_SHADER).value(); // Will throw exception on failure
    auto fragmentShader = compile_shader_code(fragment_code, GL_FRAGMENT_SHADER).value(); // Will throw exception on failure

    auto program_id = link_program(vertexShader, fragmentShader).value(); // Will throw exception on failure

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program_id;
}

std::optional<std::string> ShaderHelper::load_shader_file(const std::string& shader_path) {
    std::string shader_code;
    std::ifstream shader_file;

    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shader_file.open(shader_path);
        std::stringstream shader_stream;

        shader_stream << shader_file.rdbuf();

        shader_file.close();

        shader_code = shader_stream.str();
    } catch (std::ifstream::failure&) {
        std::cout << "Failed to load shader file: " << shader_path << std::endl;
        return {};
    }

    return shader_code;
}

static inline std::string format_info_log(const std::string& shader_code, const std::string& info_log) {
    std::vector<std::string> lines;
    std::istringstream code_line_reader(shader_code);
    for (std::string line; std::getline(code_line_reader, line);) {
        lines.push_back(line);
    }

    std::stringstream formatted_info_log;
    std::istringstream info_log_reader(info_log);
    for (std::string line; std::getline(info_log_reader, line);) {
        formatted_info_log << line << '\n';
        if (line.substr(0, 2) == "0(") {
            auto end = line.find(')');
            if (end != std::string::npos) {
                auto num = line.substr(2, end - 2);
                try {
                    auto i = std::stoi(num) - 1;
                    if (i >= 0 && i < (int) lines.size()) {
                        if (i - 1 >= 0) {
                            formatted_info_log << "\t[ ]: " << lines[i - 1] << '\n';
                        }
                        formatted_info_log << "\t[>]: " << lines[i] << '\n';
                        if (i + 1 < (int) lines.size()) {
                            formatted_info_log << "\t[ ]: " << lines[i + 1] << '\n';
                        }
                    }
                } catch (const std::exception&) {}
            }
        }
    }

    return formatted_info_log.str();
}

std::optional<uint>ShaderHelper::compile_shader_code(const std::string& shader_code, uint shader_type) {
    uint shader = glCreateShader(shader_type);

    const char* source_c_str = shader_code.c_str();
    glShaderSource(shader, 1, &source_c_str, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int msg_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msg_len);
        std::string info_log;
        info_log.resize(msg_len - 1);

        glGetShaderInfoLog(shader, msg_len, nullptr, info_log.data());
        std::cerr << "Failed to compile " << (shader_type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader\n"
                  << format_info_log(shader_code, info_log) << std::endl;
        return {};
    }

    return shader;
}

std::optional<uint> ShaderHelper::link_program(uint vertex_shader, uint fragment_shader) {
    uint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // print linking errors if any
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int msg_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &msg_len);
        std::string info_log;
        info_log.resize(msg_len - 1);


        glGetProgramInfoLog(program, msg_len, nullptr, info_log.data());
        std::cerr << "Failed to link shader program" << "\n" << format_info_log("", info_log) << std::endl;
        return {};
    }

    return program;
}
