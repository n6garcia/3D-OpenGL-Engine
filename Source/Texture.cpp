#include"Texture.h"

Texture::Texture(const char* image, std::string texType, GLuint slot)
{
	//sets c_str texType to c_str type
	type = texType;

	//sets path to filepath input
	path = std::string(image);

	int widthImg, heightImg, numColCh;

	//flip image setting
	stbi_set_flip_vertically_on_load(true);
	//load bytes in and widthImg, heightImg and numColCh
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	//generate 1 new texture ID and set to ID
	glGenTextures(1, &ID);
	//select active texture unit GL_TEXTURE(Slot)
	glActiveTexture(GL_TEXTURE0 + slot);
	//set texture "unit" to slot
	unit = slot;
	//bind texture to its target
	glBindTexture(GL_TEXTURE_2D, ID);

	//additional texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//load bytes into shader depending number of color channels
	if (numColCh == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");
	
	//generate mip map (what does this do?)
	glGenerateMipmap(GL_TEXTURE_2D);

	//free bytes
	stbi_image_free(bytes);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);

	shader.Activate();

	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}