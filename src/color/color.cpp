#include "color/color.hpp"

#include "color/extension_types.hpp"

const std::string tree::color::get_ansi_escape_code(const unsigned int color_type) {
    return "\033[" + std::to_string(color_type) + "m";
}

const std::string tree::color::get_format_sequences(const tree::color::type color_type) {
    std::string format_sequence;

    if (color_type & FG_BLOCK)
        format_sequence += get_ansi_escape_code((color_type & FG_BLOCK) >> 8);

    if (color_type & BG_BLOCK)
        format_sequence += get_ansi_escape_code((color_type & BG_BLOCK) >> 16);

    if (color_type & SGT_BLOCK) {
        for (unsigned int bit = 0; bit < 8; bit++) {
            if (color_type & (1 << bit)) format_sequence += get_ansi_escape_code(bit);
        }
    }

    return format_sequence;
}

inline bool tree::color::is_executable(const std::filesystem::directory_entry& entry) {
    using namespace std::filesystem;
    const perms exec = perms::owner_exec | perms::group_exec | perms::others_exec;

    return ((status(entry.path()).permissions() & exec) != perms::none);
}

const std::string tree::color::colorize_string(std::string string,
                                               const tree::color::type color_type) {
    return tree::color::get_format_sequences(color_type) + string +
           tree::color::get_format_sequences(tree::color::type::normal);
}

const std::string tree::color::colorize_entry(std::filesystem::directory_entry path,
                                              bool full_path) {
    std::string name;
    using type = tree::color::type;

    if (full_path == false) {
        name = path.path().filename().string();
    } else {
        name = path.path().string();
    }

    if (path.is_symlink()) {
        return colorize_string(name, type::symbolic_link);
    }

    if (path.is_directory()) {
        return colorize_string(name, type::directory);
    }

    if (path.is_fifo()) {
        return colorize_string(name, type::fifo);
    }

    if (path.is_socket()) {
        return colorize_string(name, type::socket);
    }

    if (path.is_block_file()) {
        return colorize_string(name, type::block_device_driver);
    }

    if (path.is_character_file()) {
        return colorize_string(name, type::character_device_driver);
    }

    try {
        return colorize_string(name,
                               tree::extension_types::map.at(path.path().extension().string()));
    } catch (const std::out_of_range& ex) {
    }

    if (is_executable(path)) {
        return colorize_string(name, type::executable);
    }

    return path.path().filename().string();
}