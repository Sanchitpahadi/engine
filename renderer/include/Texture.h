#pragma once

class Texture
{
private:
    unsigned int texture;
    int width, height, nrChannels;
public:
    Texture(const char* path);
    ~Texture();
    void Bind(unsigned int slot) const;
    void Unbind() const;

    unsigned int GetID() const { return texture; } // needed so Material (raw GLuint) can use this
};