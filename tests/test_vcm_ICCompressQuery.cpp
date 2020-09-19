/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "test_win_fmt.h"
#include "expand.h"

void vcm_ICCompressQuery_anyout_ok(DWORD fccCodec, DWORD fccRaw, LONG width, LONG height)
{
	HIC hic;
	LRESULT lr;

	BITMAPINFOHEADER bihIn;

	hic = ICOpen(ICTYPE_VIDEO, fccCodec, ICMODE_COMPRESS);
	BOOST_REQUIRE(hic != NULL);

	memset(&bihIn, 0, sizeof(BITMAPINFOHEADER));
	bihIn.biSize = sizeof(BITMAPINFOHEADER);
	bihIn.biWidth = width;
	bihIn.biHeight = height;
	bihIn.biPlanes = 1;
	bihIn.biBitCount = FCC2BitCount(fccRaw);
	bihIn.biCompression = FCC2Compression(fccRaw);
	bihIn.biSizeImage = 10000000;

	lr = ICCompressQuery(hic, &bihIn, NULL);
	BOOST_CHECK(lr == ICERR_OK);

	lr = ICClose(hic);
	BOOST_CHECK(lr == ICERR_OK);
}

void vcm_ICCompressQuery_anyout_ng(DWORD fccCodec, DWORD fccRaw, LONG width, LONG height)
{
	HIC hic;
	LRESULT lr;

	BITMAPINFOHEADER bihIn;

	hic = ICOpen(ICTYPE_VIDEO, fccCodec, ICMODE_COMPRESS);
	BOOST_REQUIRE(hic != NULL);

	memset(&bihIn, 0, sizeof(BITMAPINFOHEADER));
	bihIn.biSize = sizeof(BITMAPINFOHEADER);
	bihIn.biWidth = width;
	bihIn.biHeight = height;
	bihIn.biPlanes = 1;
	bihIn.biBitCount = FCC2BitCount(fccRaw);
	bihIn.biCompression = FCC2Compression(fccRaw);
	bihIn.biSizeImage = 10000000;

	lr = ICCompressQuery(hic, &bihIn, NULL);
	BOOST_CHECK(lr != ICERR_OK);

	lr = ICClose(hic);
	BOOST_CHECK(lr == ICERR_OK);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressQuery_anyout_biCompression_ok, expand(data::make(vecCodecFcc) ^ data::make(vecSupportedInputFccs)), fccCodec, fccRaw)
{
	vcm_ICCompressQuery_anyout_ok(fccCodec, fccRaw, TEST_WIDTH, TEST_HEIGHT);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressQuery_anyout_biCompression_ng, expand(data::make(vecCodecFcc) ^ data::make(vecUnsupportedInputFccs)), fccCodec, fccRaw)
{
	vcm_ICCompressQuery_anyout_ng(fccCodec, fccRaw, TEST_WIDTH, TEST_HEIGHT);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressQuery_anyout_biWidth_ok, expand(data::make(vecCodecFcc) ^ data::make(vecTopPriorityRawFcc) ^ data::make(vecSupportedWidth)), fccCodec, fccRaw, width)
{
	vcm_ICCompressQuery_anyout_ok(fccCodec, fccRaw, width, TEST_HEIGHT);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressQuery_anyout_biWidth_ng, expand(data::make(vecCodecFcc) ^ data::make(vecTopPriorityRawFcc) ^ data::make(vecUnsupportedWidth)), fccCodec, fccRaw, width)
{
	vcm_ICCompressQuery_anyout_ng(fccCodec, fccRaw, width, TEST_HEIGHT);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressQuery_anyout_biHeight_ok, expand(data::make(vecCodecFcc) ^ data::make(vecTopPriorityRawFcc) ^ data::make(vecSupportedHeight)), fccCodec, fccRaw, height)
{
	vcm_ICCompressQuery_anyout_ok(fccCodec, fccRaw, TEST_WIDTH, height);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressQuery_anyout_biHeight_ng, expand(data::make(vecCodecFcc) ^ data::make(vecTopPriorityRawFcc) ^ data::make(vecUnsupportedHeight)), fccCodec, fccRaw, height)
{
	vcm_ICCompressQuery_anyout_ng(fccCodec, fccRaw, TEST_WIDTH, height);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressQuery_anyout_combination_ng, data::make(vecUnsupportedCombination), fccCodec, fccPrimary /* notused */, fccRaw, width, height)
{
	vcm_ICCompressQuery_anyout_ng(fccCodec, fccRaw, width, height);
}
