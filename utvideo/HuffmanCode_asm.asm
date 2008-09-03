; 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ
; $Id$

; Ut Video Codec Suite
; Copyright (C) 2008  UMEZAWA Takeshi
; 
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
; 
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
; 
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
; 
; 
; Ut Video Codec Suite
; Copyright (C) 2008  梅澤 威志
; 
; このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフ
; トウェア財団によって発行された GNU 一般公衆利用許諾契約書(バージョ
; ン2か、希望によってはそれ以降のバージョンのうちどれか)の定める条件
; の下で再頒布または改変することができます。
; 
; このプログラムは有用であることを願って頒布されますが、*全くの無保
; 証* です。商業可能性の保証や特定の目的への適合性は、言外に示された
; ものも含め全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご
; 覧ください。
; 
; あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を
; 一部受け取ったはずです。もし受け取っていなければ、フリーソフトウェ
; ア財団まで請求してください(宛先は the Free Software Foundation,
; Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA)。

.686
.no87
.xmm

.model	flat

extrn	_memset:PROC


_TEXT_ASM	SEGMENT	page public flat 'CODE'

; void i686_HuffmanEncode_align1(BYTE *pDstBegin, const BYTE *pSrcBegin, const BYTE *pSrcEnd, const HUFFMAN_ENCODE_TABLE *pEncodeTable)
public	_i686_HuffmanEncode_align1
_i686_HuffmanEncode_align1	proc

	push		ebx
	push		esi
	push		edi
	push		ebp

	mov			esi, dword ptr [esp + 16 + 4 +  4]	; pSrcBegin
	mov			edi, dword ptr [esp + 16 + 4 +  0]	; pDstBegin
	mov			edx, dword ptr [esp + 16 + 4 +  8]	; pSrcEnd
	mov			ebp, dword ptr [esp + 16 + 4 + 12]	; pEncodeTable
	cmp			dword ptr [ebp], 0
	je			label3

	mov			bl, -32
	mov			cl, 0

	align		64
label1:
	shld		eax, ecx, cl
	cmp			esi, edx
	jnb			label4
	movzx		ecx, byte ptr [esi]
	inc			esi
	mov			ecx, dword ptr [ebp+ecx*4]
	add			bl, cl
	jnc			label1
	sub			cl, bl
	shld		eax, ecx, cl
	mov			dword ptr [edi], eax
	add			edi, 4
	add			cl, bl
	sub			bl, 32
	jmp			label1

label4:
	test		bl, 1fh
	jz			label3
	neg			bl
	mov			cl, bl
	shl			eax, cl
	mov			dword ptr [edi], eax
	add			edi, 4
label3:
	mov			eax, edi			; 返り値は eax に。
	sub			eax, dword ptr [esp + 16 + 4 +  0]	; pDstBegin

	pop			ebp
	pop			edi
	pop			esi
	pop			ebx
	ret

_i686_HuffmanEncode_align1	endp


HUFFMAN_DECODE	macro	procname, accum

; void procname(BYTE *pDstBegin, BYTE *pDstcEnd, const BYTE *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable)
public	&procname
&procname	proc

	push		ebx
	push		esi
	push		edi
	push		ebp

	mov			esi, dword ptr [esp + 16 + 4 +  8]	; pSrcBegin
	mov			edi, dword ptr [esp + 16 + 4 +  0]	; pDstBegin
	mov			ebx, dword ptr [esp + 16 + 4 + 12]	; pDecodeTable
	mov			edx, dword ptr [esi+4]
	mov			ch, -32
if &accum
	mov			byte ptr [esp - 4], 80h
endif

	cmp			byte ptr [ebx + 32+4*32+1], 0	; pDecodeTable->SymbolAndCodeLength[0].nCodeLength
	jne			label1

	; msmset(pDstBegin, pDecodeTable->dwSymbol[0], pDstEnd-pDstBegin);
	mov			eax, dword ptr [esp + 16 + 4 +  4]	; pDstEnd
	sub			eax, edi
	push		eax
	mov			eax, dword ptr [ebx + 32+4*32]		; pDecodeTable->SymbolAndCodeLength[0].bySymbol
if &accum
	add			eax, 80h
	movzx		eax, al
endif
	push		eax
	push		edi
	call		_memset
	add			esp, 4*3

	jmp			label2

	align		64
label1:
	cmp			edi, dword ptr [esp + 16 + 4 +  4]	; pDstEnd
	jae			label2
	mov			eax, dword ptr [esi]
	mov			cl, ch
	shld		eax, edx, cl
	or			eax, 1
	bsr			ebp, eax
	mov			cl, byte ptr [ebx + ebp]					; pDecodeTable->nCodeShift[ebp]
	shr			eax, cl
	mov			ebp, dword ptr [ebx + 32 + ebp*4]			; pDecodeTable->dwSymbolBase[ebp]
	add			ebp, eax
	mov			eax, dword ptr [ebx + 32+4*32 + ebp*4]		; pDecodeTable->SymbolAndCodeLength[ebp]
if &accum
	add			al, byte ptr [esp - 4]
	mov			byte ptr [esp - 4], al
endif
	mov			byte ptr [edi], al
	inc			edi
	add			ch, ah
	jnc			label1
	sub			ch, 32
	add			esi, 4
	mov			edx, dword ptr [esi+4]
	jmp			label1

label2:
	pop			ebp
	pop			edi
	pop			esi
	pop			ebx
	ret

&procname	endp

		endm

HUFFMAN_DECODE	_i686_HuffmanDecode_align1, 0
HUFFMAN_DECODE	_i686_HuffmanDecodeAndAccum_align1, 1


PUSH_SYMBOL	macro	seq, curoff
	shld		eax, ecx, cl
	movzx		ecx, byte ptr [esi + &curoff]
	shl			ecx, 24
	or			ecx, 8
	add			bl, cl
	jnc			label_loopnext_&seq
	sub			cl, bl
	shld		eax, ecx, cl
	mov			dword ptr [edi], eax
	add			edi, 4
	add			cl, bl
	sub			bl, 32
label_loopnext_&seq:
		endm

HUFFMAN_ENCODE_SYMBOL	macro	seq, lastseq, curoff, back, checkend, channel
	shld		eax, ecx, cl
IF &checkend
	cmp			esi, edx
	jnb			label_loopout
ENDIF
	movzx		ecx, byte ptr [esi + &curoff]
IF &back
	sub			cl, byte ptr [esi + &curoff - &back]
ENDIF
IF &lastseq
	add			esi, 4
ENDIF
	mov			ecx, dword ptr [ebp+ecx*4+&channel*1024]		; sizeof(HUFFMAN_ENCODE_TABLE) == 1024
	add			bl, cl
IF &lastseq
	jnc			label_loophead
ELSE
	jnc			label_loopnext_&seq
ENDIF
	sub			cl, bl
	shld		eax, ecx, cl
	mov			dword ptr [edi], eax
	add			edi, 4
	add			cl, bl
	sub			bl, 32
IF &lastseq
	jmp			label_loophead
ELSE
label_loopnext_&seq:
ENDIF
		endm


HUFFMAN_ENCODE_FIRST_RAW_WITH_DIFF_YUV422	macro	procname, yoff1, uoff, voff, diff

; void procname(BYTE *pDstBegin, const BYTE *pSrcBegin, const BYTE *pSrcEnd, const HUFFMAN_ENCODE_TABLE *pEncodeTable)
public	&procname
&procname	proc

IF &diff
yback=2
cback=4
ELSE
yback=0
cback=0
ENDIF
	push		ebx
	push		esi
	push		edi
	push		ebp

	mov			esi, dword ptr [esp + 16 + 4 +  4]	; pSrcBegin
	mov			edi, dword ptr [esp + 16 + 4 +  0]	; pDstBegin
	mov			edx, dword ptr [esp + 16 + 4 +  8]	; pSrcEnd
	mov			ebp, dword ptr [esp + 16 + 4 + 12]	; pEncodeTable
	cmp			dword ptr [ebp], 0
	je			label3

	mov			bl, -32
	mov			cl, 0

	PUSH_SYMBOL				4,    &yoff1
	PUSH_SYMBOL				5,    &uoff
	HUFFMAN_ENCODE_SYMBOL	6, 0, &yoff1+2, 2, 0, 0
	PUSH_SYMBOL				7,    &voff
	add			esi, 4

	align		64
label_loophead:
	HUFFMAN_ENCODE_SYMBOL	0, 0, &yoff1,   yback, 1, 0
	HUFFMAN_ENCODE_SYMBOL	1, 0, &uoff,    cback, 0, 1
	HUFFMAN_ENCODE_SYMBOL	2, 0, &yoff1+2, yback, 0, 0
	HUFFMAN_ENCODE_SYMBOL	3, 1, &voff,    cback, 0, 2

label_loopout:
	test		bl, 1fh
	jz			label3
	neg			bl
	mov			cl, bl
	shl			eax, cl
	mov			dword ptr [edi], eax
	add			edi, 4
label3:
	mov			eax, edi			; 返り値は eax に。
	sub			eax, dword ptr [esp + 16 + 4 +  0]	; pDstBegin

	pop			ebp
	pop			edi
	pop			esi
	pop			ebx
	ret

&procname	endp

		endm

HUFFMAN_ENCODE_FIRST_RAW_WITH_DIFF_YUV422	_i686_HuffmanEncodeFirstRawWithDiffYUY2, 0, 1, 3, 1

end
