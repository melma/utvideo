/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "utvideo.h"
#include "Format.h"

int UtVideoFormatToVCMFormat(DWORD *biCompression, WORD *biBitCount, utvf_t utvf)
{
	switch (utvf)
	{
	case UTVF_NFCC_BGR_BU:
		*biCompression = BI_RGB;
		*biBitCount    = 24;
		return 0;
	case UTVF_NFCC_BGRX_BU:
		*biCompression = BI_RGB;
		*biBitCount    = 32;
		return 0;
	case UTVF_NFCC_BGRA_BU:
		*biCompression = BI_RGB;
		*biBitCount    = 32;
		return 0;
	}

	switch (utvf)
	{
	case UTVF_ULRA:
	case UTVF_UMRA:
		*biBitCount = 32;
		break;
	case UTVF_ULRG:
	case UTVF_ULY4:
	case UTVF_ULY2:
	case UTVF_ULY0:
	case UTVF_ULH4:
	case UTVF_ULH2:
	case UTVF_ULH0:
	case UTVF_UMRG:
	case UTVF_UMY4:
	case UTVF_UMY2:
	case UTVF_UMH4:
	case UTVF_UMH2:
		*biBitCount = 24;
		break;
	case UTVF_UQRA:
		*biBitCount = 40;
		break;
	case UTVF_UQRG:
	case UTVF_UQY2:
	case UTVF_UQY0:
		*biBitCount = 30;
		break;

	case UTVF_YV24:
		*biBitCount = 24;
		break;
	case UTVF_YUY2:
	case UTVF_YUYV:
	case UTVF_YUNV:
	case UTVF_yuvs:
	case UTVF_UYVY:
	case UTVF_UYNV:
	case UTVF_2vuy:
	case UTVF_HDYC:
	case UTVF_YV16:
		*biBitCount = 16;
		break;
	case UTVF_YV12:
	case UTVF_NV12:
		*biBitCount = 12;
		break;
	case UTVF_v210:
		*biBitCount = 20;
		break;
	case UTVF_b64a:
		*biBitCount = 64;
		break;
	case UTVF_b48r:
		*biBitCount = 48;
		break;
	case UTVF_r210:
		*biBitCount = 30;
		break;
	case UTVF_YUV444P10LE:
	case UTVF_YUV444P16LE:
		*biBitCount = 48;
		break;
	case UTVF_P210:
	case UTVF_P216:
	case UTVF_YUV422P10LE:
	case UTVF_YUV422P16LE:
		*biBitCount = 32;
		break;
	case UTVF_P010:
	case UTVF_P016:
	case UTVF_YUV420P10LE:
	case UTVF_YUV420P16LE:
		*biBitCount = 24;
		break;

	default:
		return -1;
	}

	*biCompression = FCC(utvf);

	return 0;
}

int VCMFormatToUtVideoFormat(utvf_t *utvf, DWORD biCompression, WORD biBitCount)
{
	DWORD dwtmp;
	WORD wtmp;

	if (biCompression == BI_RGB)
	{
		switch (biBitCount)
		{
		case 24:
			*utvf = UTVF_NFCC_BGR_BU;
			return 0;
		case 32:
			*utvf = UTVF_NFCC_BGRX_BU;
			return 0;
		default:
			return -1;
		}
	}

	if (UtVideoFormatToVCMFormat(&dwtmp, &wtmp, (utvf_t)UNFCC(biCompression)) != 0)
		return -1;
	if (dwtmp != biCompression) // biBitCount �̓`�F�b�N���Ȃ�
		return -1;

	*utvf = UNFCC(biCompression);
	return 0;
}
