
#include "ResourceManager.h"
#include "Logger.h"

extern "C" {
#include <upng.h>
}

/// upng must have already been populated using either upng_new_from_file or upng_new_from_bytes
/// filename is just for debug
static bool loadTexture(GLuint tex, upng_t *upng, const std::string & filename, GLenum internalformat) {
	upng_decode(upng);

	upng_error errcode = upng_get_error(upng);
	if (errcode != UPNG_EOK) {
		std::string err;
		switch (errcode) {
		case UPNG_EOK:
			err = "No error (success)";
			break;
		case UPNG_ENOMEM:
			err = "Out of memory";
			break;
		case UPNG_ENOTFOUND:
			err = "Resource not found";
			break;
		case UPNG_ENOTPNG:
			err = "Invalid file header (not a PNG image)";
			break;
		case UPNG_EMALFORMED:
			err = "PNG image data does not follow spec and is malformed";
			break;
		case UPNG_EUNSUPPORTED:
			err = "PNG image data is well-formed but not supported by uPNG";
			break;
		}
		ERR_LOG << "Unable to load image " << filename << ": " << err;
		upng_free(upng);
		return false;
	}

	GLsizei width = static_cast<GLsizei>(upng_get_width(upng));
	GLsizei height = static_cast<GLsizei>(upng_get_height(upng));

	GLenum format;
	switch (upng_get_components(upng)) {
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	glTextureStorage2D(tex, 1, internalformat, width, height);
	glTextureSubImage2D(tex, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, upng_get_buffer(upng));

	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateTextureMipmap(tex);

	upng_free(upng);
	return true;
}

bool ResourceManager::loadTexture(GLuint tex, const std::string & filename, GLenum internalformat) {
	upng_t *upng = upng_new_from_file(filename.c_str());
	if (NULL == upng) {
		ERR_LOG << "Unable to load image " << filename << ": unknown error";
		return false;
	}
	return ::loadTexture(tex, upng, filename, internalformat);
}

bool ResourceManager::loadTexture(GLuint tex, const unsigned char* data, size_t length, GLenum internalformat) {
	upng_t *upng = upng_new_from_bytes(data, static_cast<unsigned long>(length));
	if (NULL == upng) {
		return false;
	}
	return ::loadTexture(tex, upng, "<internal resource>", internalformat);
}
