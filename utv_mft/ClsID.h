/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */


#define DEFINE_MFTCODEC_CLSID(fcc, fccname) \
	DEFINE_GUID(CLSID_##fccname##MFTEncoder, fcc, 0xE992, 0x460D, 0x84, 0x0B, 0xC1, 0xC6, 0x49, 0x74, 0x57, 0xEF); \
	DEFINE_GUID(CLSID_##fccname##MFTDecoder, fcc, 0xD992, 0x460D, 0x84, 0x0B, 0xC1, 0xC6, 0x49, 0x74, 0x57, 0xEF)

DEFINE_MFTCODEC_CLSID(FCC('ULRA'), ULRA);
DEFINE_MFTCODEC_CLSID(FCC('ULRG'), ULRG);
DEFINE_MFTCODEC_CLSID(FCC('ULY0'), ULY0);
DEFINE_MFTCODEC_CLSID(FCC('ULY2'), ULY2);
DEFINE_MFTCODEC_CLSID(FCC('ULY4'), ULY4);
DEFINE_MFTCODEC_CLSID(FCC('ULH0'), ULH0);
DEFINE_MFTCODEC_CLSID(FCC('ULH2'), ULH2);
DEFINE_MFTCODEC_CLSID(FCC('ULH4'), ULH4);

DEFINE_MFTCODEC_CLSID(FCC('UQY2'), UQY2);
DEFINE_MFTCODEC_CLSID(FCC('UQRG'), UQRG);
DEFINE_MFTCODEC_CLSID(FCC('UQRA'), UQRA);

DEFINE_MFTCODEC_CLSID(FCC('UMRA'), UMRA);
DEFINE_MFTCODEC_CLSID(FCC('UMRG'), UMRG);
DEFINE_MFTCODEC_CLSID(FCC('UMY2'), UMY2);
DEFINE_MFTCODEC_CLSID(FCC('UMY4'), UMY4);
DEFINE_MFTCODEC_CLSID(FCC('UMH2'), UMH2);
DEFINE_MFTCODEC_CLSID(FCC('UMH4'), UMH4);
