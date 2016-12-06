#include "billiards_header.h"

Texture::Texture() {}
Texture::~Texture() {}

GLuint Texture::create_texture(GLuint tex_id, char *texture) {
	glBindTexture(GL_TEXTURE_2D, tex_id);

	cv::Mat img = cv::imread(texture);
	cv::flip(img, img, 0);
	cv::cvtColor(img, img, CV_BGR2RGB);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img.size().width, img.size().height, GL_RGB, GL_UNSIGNED_BYTE, img.data);

	/* �e�N�X�`�����g��E�k��������@�̎w�� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* �e�N�X�`���̌J��Ԃ����@�̎w�� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/* �e�N�X�`���� */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/* �A���t�@�e�X�g�̔��ʊ֐� */
	glAlphaFunc(GL_GREATER, 0.5);

	img.release();
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex_id;
}