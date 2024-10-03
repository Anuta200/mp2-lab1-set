// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len<=0) throw -1;
        BitLen=len;
        MemLen= (BitLen + (sizeof(TELEM) * 8 - 1)) / (sizeof(TELEM) * 8);
        pMem=new TELEM[MemLen];
        for (int i = 0; i < MemLen;i++) {
            pMem[i] = 0;
        }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
     delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n >= BitLen) || (n < 0)) throw -2;
    int res = n / (sizeof(TELEM)*8);
    return res;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n >= BitLen) || (n < 0)) throw - 4;
    return 1 << n % (sizeof(TELEM)*8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    int i = GetMemIndex(n);
    TELEM t = GetMemMask(n);
    pMem[i] = pMem[i] | t;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    int i = GetMemIndex(n);
    TELEM t = ~GetMemMask(n);
    pMem[i] = pMem[i] & t;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int i = GetMemIndex(n);
    TELEM t = GetMemMask(n);
    return (pMem[i] & t)!=0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0;i < MemLen;i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; ++i) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    //int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    int len = std::max(BitLen, bf.BitLen);
    TBitField result(len);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = pMem[i] | bf.pMem[i];

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = std::max(BitLen, bf.BitLen);// BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField result(len);
    for (int i = 0; i < result.MemLen; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0;i < MemLen;i++) {
        result.pMem[i] = ~pMem[i];
    }
    int lastBitMask = (1 << BitLen % (sizeof(TELEM) * 8)) - 1;
    if (BitLen % (sizeof(TELEM) * 8) != 0)
        result.pMem[MemLen - 1] &= lastBitMask;
    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++) {
        int b;
        istr >> b;
        if (b) {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
