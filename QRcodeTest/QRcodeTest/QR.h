#ifndef QR_H
#define  QR_H
#include <iostream>
#include "qrencode.h"   

using namespace std;

typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef signed long     LONG;
#define BI_RGB          0L
#define OUT_FILE_PIXEL_PRESCALER 8               //预分频器（每个维度上每个QRCode像素的bmp文件中的像素数）
#define FILE_NAME       "qr.bmp"
#define QR_CODE         "https://www.baidu.com"
#define PIXEL_COLOR_R                0                  // bmp像素的颜色
#define PIXEL_COLOR_G                0
#define PIXEL_COLOR_B                0xff

#pragma pack(push, 2)
typedef struct
{
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BITMAPFILEHEADER;
typedef struct
{
    DWORD      biSize;
    LONG       biWidth;
    LONG       biHeight;
    WORD       biPlanes;
    WORD       biBitCount;
    DWORD      biCompression;
    DWORD      biSizeImage;
    LONG       biXPelsPerMeter;
    LONG       biYPelsPerMeter;
    DWORD      biClrUsed;
    DWORD      biClrImportant;
} BITMAPINFOHEADER;


#pragma pack(pop)

class QR
{
public:
    QR();
    ~QR();
    static QR* GetInteance();

    void Init();
    void SetBmpFileSzie();
    void PrepareBmpheaders();
    void QrCodeBitstoBmppixels();
    bool OutPutBmpImage();

private:
    QRcode*          m_pQRcode;
    FILE*            m_pFile;
    BITMAPFILEHEADER m_kFileHeader;
    BITMAPINFOHEADER m_kInfoHeader;
    char*            m_pSourceSring;
    unsigned int     m_unWidth;
    unsigned int     m_unWidthAdjusted;
    unsigned int     m_unDataBytes;
    unsigned char*   m_pRGBData;
    unsigned char*   m_pSourceData;
    unsigned char*   m_pDestData;
};


#endif // QR_PNG_H


