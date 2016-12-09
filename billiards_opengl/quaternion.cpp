#include "billiards_header.h"

Quaternion::Quaternion() {
	init_value[0] = 1.0;
	quat[0] = 1.0;
	for (size_t i = 1; i < 4; i++) {
		init_value[i] = 0.0;
		quat[i] = 0.0;
	}
	quat_transmat();
}

Quaternion::~Quaternion() {}

void Quaternion::integration() {
	quat[0] = trans_quat[0] * init_value[0] - trans_quat[1] * init_value[1] - trans_quat[2] * init_value[2] - trans_quat[3] * init_value[3];
	quat[1] = trans_quat[0] * init_value[1] + trans_quat[1] * init_value[0] + trans_quat[2] * init_value[3] - trans_quat[3] * init_value[2];
	quat[2] = trans_quat[0] * init_value[2] - trans_quat[1] * init_value[3] + trans_quat[2] * init_value[0] + trans_quat[3] * init_value[1];
	quat[3] = trans_quat[0] * init_value[3] + trans_quat[1] * init_value[2] - trans_quat[2] * init_value[1] + trans_quat[3] * init_value[0];
}

void Quaternion::quat_transmat() {
	double x2 = quat[1] * quat[1] * 2.0;
	double y2 = quat[2] * quat[2] * 2.0;
	double z2 = quat[3] * quat[3] * 2.0;
	double xy = quat[1] * quat[2] * 2.0;
	double yz = quat[2] * quat[3] * 2.0;
	double zx = quat[3] * quat[1] * 2.0;
	double xw = quat[1] * quat[0] * 2.0;
	double yw = quat[2] * quat[0] * 2.0;
	double zw = quat[3] * quat[0] * 2.0;

	trans_mat[0] = 1.0 - y2 - z2;
	trans_mat[1] = xy + zw;
	trans_mat[2] = zx - yw;
	trans_mat[4] = xy - zw;
	trans_mat[5] = 1.0 - z2 - x2;
	trans_mat[6] = yz + xw;
	trans_mat[8] = zx + yw;
	trans_mat[9] = yz - xw;
	trans_mat[10] = 1.0 - x2 - y2;
	trans_mat[3] = trans_mat[7] = trans_mat[11] = trans_mat[12] = trans_mat[13] = trans_mat[14] = 0.0;
	trans_mat[15] = 1.0;
}

void Quaternion::set_trans_quat(GLdouble *trans) {
	for (size_t i = 0; i < 4; i++) trans_quat[i] = trans[i];
}

void Quaternion::save_init() {
	for (size_t i = 0; i < 4; i++) init_value[i] = quat[i];
}