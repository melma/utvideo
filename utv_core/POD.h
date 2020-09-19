/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

/*
 * std::array �� std::pair �� std::tuple ���g���ƁA
 * �ꕔ�̃����o�֐����C�����C���W�J���ꂸ�ɕ��ʂ̊֐��Ƃ��Ď��̉������B
 *
 * ���̎��A AVX1 �ŃR���p�C�����������o�֐��̎��̂����L�����ƁA
 * SSSE3 �ŃR���p�C�������֐�����Ă΂�� illegal instruction �ƂȂ�B
 *
 * �ȉ��̂悤�ȃ����o�֐�����؎����Ȃ� POD (plain old data) �^���ƁA
 * ��L�̂悤�Ȗ��͔������Ȃ��B�i�R���p�C���Ɉˑ�����������������Ȃ����j
 */

template<typename T>
struct VECTOR_RGB
{
	T g, b, r;
};

template<typename T>
struct VECTOR_RGBA
{
	T g, b, r, a;
};

template<typename T>
struct VECTOR_YUV422
{
	T y0, y1, u, v;
};

template<typename T>
struct VECTOR_UV
{
	T u, v;
};

template<typename T>
struct VECTOR2
{
	T v0, v1;
};

template<typename T>
struct VECTOR3
{
	T v0, v1, v2;
};

template<typename T>
struct VECTOR4
{
	T v0, v1, v2, v3;
};
