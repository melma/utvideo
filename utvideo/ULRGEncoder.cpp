/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */
/*
 * Ut Video Codec Suite
 * Copyright (C) 2008  UMEZAWA Takeshi
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * 
 * Ut Video Codec Suite
 * Copyright (C) 2008  �~�V �Ўu
 * 
 * ���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t
 * �g�E�F�A���c�ɂ���Ĕ��s���ꂽ GNU ��ʌ��O���p�����_��(�o�[�W��
 * ��2���A��]�ɂ���Ă͂���ȍ~�̃o�[�W�����̂����ǂꂩ)�̒�߂����
 * �̉��ōĔЕz�܂��͉��ς��邱�Ƃ��ł��܂��B
 * 
 * ���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖���
 * ��* �ł��B���Ɖ\���̕ۏ؂����̖ړI�ւ̓K�����́A���O�Ɏ����ꂽ
 * ���̂��܂ߑS�����݂��܂���B�ڂ�����GNU ��ʌ��O���p�����_�񏑂���
 * �����������B
 * 
 * ���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU ��ʌ��O���p�����_�񏑂̕�������
 * �ꕔ�󂯎�����͂��ł��B�����󂯎���Ă��Ȃ���΁A�t���[�\�t�g�E�F
 * �A���c�܂Ő������Ă�������(����� the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA)�B
 */

#include "StdAfx.h"
#include "utvideo.h"
#include "ULRGEncoder.h"
#include "Predict.h"
#include "resource.h"

const CPlanarEncoder::INPUTFORMAT CULRGEncoder::m_infmts[1] = {
	{ BI_RGB, 24, FALSE },
};

CULRGEncoder::CULRGEncoder(void)
{
}

CULRGEncoder::~CULRGEncoder(void)
{
}

CEncoder *CULRGEncoder::CreateInstance(void)
{
	return new CULRGEncoder();
}

void CULRGEncoder::CalcPlaneSizes(const BITMAPINFOHEADER *pbihIn)
{
	m_dwFrameStride = ROUNDUP(pbihIn->biWidth * 3, 4);
	m_dwFrameSize = m_dwFrameStride * m_dwNumStrides;

	m_dwPlaneStride[0] = pbihIn->biWidth;
	m_dwPlaneSize[0]   = m_dwPlaneStride[0] * m_dwNumStrides;

	m_dwPlaneStride[1] = m_dwPlaneStride[0];
	m_dwPlaneSize[1]   = m_dwPlaneSize[0];

	m_dwPlaneStride[2] = m_dwPlaneStride[0];
	m_dwPlaneSize[2]   = m_dwPlaneSize[0];
}

void CULRGEncoder::ConvertToPlanar(DWORD nBandIndex)
{
	DWORD dwStrideBegin = m_dwNumStrides *  nBandIndex      / m_dwDivideCount;
	DWORD dwStrideEnd   = m_dwNumStrides * (nBandIndex + 1) / m_dwDivideCount;
	BYTE *g, *b, *r;
	const BYTE *pSrcBegin, *pSrcEnd, *pStrideBegin, *p;

	pSrcBegin = ((BYTE *)m_icc->lpInput) + dwStrideBegin * m_dwFrameStride;
	pSrcEnd   = ((BYTE *)m_icc->lpInput) + dwStrideEnd   * m_dwFrameStride;
	g = m_pCurFrame->GetPlane(0) + dwStrideBegin * m_dwPlaneStride[0];
	b = m_pCurFrame->GetPlane(1) + dwStrideBegin * m_dwPlaneStride[1];
	r = m_pCurFrame->GetPlane(2) + dwStrideBegin * m_dwPlaneStride[2];

	for (pStrideBegin = pSrcEnd - m_dwFrameStride; pStrideBegin >= pSrcBegin; pStrideBegin -= m_dwFrameStride)
	{
		const BYTE *pStrideEnd = pStrideBegin + m_icc->lpbiInput->biWidth * 3;
		for (p = pStrideBegin; p < pStrideEnd; p += 3)
		{
			*g++ = *(p+1);
			*b++ = *(p+0) - *(p+1) + 0x80;
			*r++ = *(p+2) - *(p+1) + 0x80;
		}
	}
}