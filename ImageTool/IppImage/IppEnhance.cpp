#include "pch.h"
#include "IppEnhance.h"
#include "math.h"

void IppInverse(IppByteImage& img)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
		p[i] = 255 - p[i];
}

void IppBrightness(IppByteImage& img, int n)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
		p[i] = limit(p[i] + n);
}

void IppContrast(IppByteImage& img, int n)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
		p[i] = limit(p[i] + (p[i] - 128) * n / 100);
}

void IppGammaCorrection(IppByteImage& img, float gamma)
{
	float inv_gamma = 1.f / gamma;
	
	float gamma_table[256];
	for (int i = 0; i < 256; i++)
		gamma_table[i] = pow(i / 255.f, inv_gamma);

	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = static_cast<BYTE>(limit(gamma_table[p[i]] * 255 + 0.5f));
	}
}

void IppHistogram(IppByteImage& img, float histo[256])
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	int cnt[256];
	memset(cnt, 0, sizeof(int) * 256);
	for (int i = 0; i < size; i++)
		cnt[p[i]]++;

	for (int i = 0; i < 256; i++)
		histo[i] = static_cast<float>(cnt[i]) / size;
}