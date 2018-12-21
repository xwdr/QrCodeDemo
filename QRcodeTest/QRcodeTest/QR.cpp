#include "QR.h"

QR::QR()
{
    m_pSourceSring = QR_CODE;
}

QR::~QR()
{
    free(m_pRGBData);
    QRcode_free(m_pQRcode);
}

QR* QR::GetInteance()
{
    static QR mqr;
    return &mqr;

}

void QR::Init()
{
    m_pQRcode = QRcode_encodeString(m_pSourceSring, 0, QR_ECLEVEL_H, QR_MODE_8, 1);
    if (m_pQRcode)
    {
        //矩阵的维数
        m_unWidth = m_pQRcode->width;
        SetBmpFileSzie();
        PrepareBmpheaders();
        QrCodeBitstoBmppixels();
        bool bRet = OutPutBmpImage();
        if (bRet)
        {
            cout << "success!" << endl;
        }
        else
        {
            cout << "failed" << endl;
        }
    }
    else
    {
        cout << "qr init failed!" << endl;
    }
}

//设置bmp文件大小
void QR::SetBmpFileSzie()
{
    //每一个维度占的像素的个数（8），每个像素3个字节
    m_unWidthAdjusted = m_unWidth * OUT_FILE_PIXEL_PRESCALER * 3;
    if (m_unWidthAdjusted % 4)
    {
        //Windows规定一个扫描行所占的字节数必须是
        //4的倍数（即以long为单位），不足的以0填充，
        m_unWidthAdjusted = (m_unWidthAdjusted / 4 + 1) * 4;
    }
        
    m_unDataBytes = m_unWidthAdjusted * m_unWidth * OUT_FILE_PIXEL_PRESCALER;
    // 分配像素缓冲区
    if (!(m_pRGBData = (unsigned char*)malloc(m_unDataBytes)))
    {
        cout << "out of memory" << endl;
        return;
    }
    // 初始化白色
    memset(m_pRGBData, 0xff, m_unDataBytes); 
}

//设置bmp头
void QR::PrepareBmpheaders()
{
    m_kFileHeader.bfType = 0x4d42;  // "BM"
    m_kFileHeader.bfSize = sizeof(BITMAPFILEHEADER)+
        sizeof(BITMAPINFOHEADER)+
        m_unDataBytes;
    m_kFileHeader.bfReserved1 = 0;
    m_kFileHeader.bfReserved2 = 0;
    m_kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+
        sizeof(BITMAPINFOHEADER);
   
    m_kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_kInfoHeader.biWidth = m_unWidth * OUT_FILE_PIXEL_PRESCALER;
    m_kInfoHeader.biHeight = -((int)m_unWidth * OUT_FILE_PIXEL_PRESCALER);
    m_kInfoHeader.biPlanes = 1;
    m_kInfoHeader.biBitCount = 24;
    m_kInfoHeader.biCompression = BI_RGB;
    m_kInfoHeader.biSizeImage = 0;
    m_kInfoHeader.biXPelsPerMeter = 0;
    m_kInfoHeader.biYPelsPerMeter = 0;
    m_kInfoHeader.biClrUsed = 0;
    m_kInfoHeader.biClrImportant = 0;
}

//将二维码转化成bmp像素
void QR::QrCodeBitstoBmppixels()
{
    m_pSourceData = m_pQRcode->data;
    for (int y = 0; y < m_unWidth; y++)
    {
        m_pDestData = m_pRGBData + m_unWidthAdjusted * y * 8;
        for (int x = 0; x < m_unWidth; x++)
        {
            if (*m_pSourceData & 1)
            {
                for (int l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
                {
                    for (int n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
                    {
                        *(m_pDestData + n * 3 + m_unWidthAdjusted * l) = PIXEL_COLOR_B;
                        *(m_pDestData + 1 + n * 3 + m_unWidthAdjusted * l) = PIXEL_COLOR_G;
                        *(m_pDestData + 2 + n * 3 + m_unWidthAdjusted * l) = PIXEL_COLOR_R;
                    }
                }
            }
            m_pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
            m_pSourceData++;
        }
    }
}

//输出bmp图片
bool QR::OutPutBmpImage()
{
    int ret = fopen_s(&m_pFile, FILE_NAME, "wb");
    if (ret)
    {
        return false;
    }
    fwrite(&m_kFileHeader, sizeof(BITMAPFILEHEADER), 1, m_pFile);
    fwrite(&m_kInfoHeader, sizeof(BITMAPINFOHEADER), 1, m_pFile);
    fwrite(m_pRGBData, sizeof(unsigned char), m_unDataBytes, m_pFile);
    return true;
}