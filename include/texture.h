#pragma once

#include <string>
#include <optional>

struct Texture{
    void load(const char *filename);
    void bind() const;
    uint32_t id;
    std::string type;
    std::string path;
};

bool load_texture_make_id(const char *filename, uint32_t *id);
