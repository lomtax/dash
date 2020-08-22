// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_BIGNUM_H
#define BITCOIN_BIGNUM_H

#include "serialize.h"
#include "uint256.h"
#include "version.h"

#include <stdexcept>
#include <stdint.h>
#include <vector>

#include <openssl/bn.h>
#include <openssl/opensslv.h>  // For using openssl 1.0 and 1.1 branches.

/** Errors thrown by the bignum class */
class bignum_error : public std::runtime_error
{
public:
    explicit bignum_error(const std::string& str) : std::runtime_error(str) {}
};


/** RAII encapsulated BN_CTX (OpenSSL bignum context) */
class CAutoBN_CTX
{
protected:
    BN_CTX* pctx;
    BN_CTX* operator=(BN_CTX* pnew) { return pctx = pnew; }

public:
    CAutoBN_CTX()
    {
        pctx = BN_CTX_new();
        if (pctx == NULL)
            throw bignum_error("CAutoBN_CTX : BN_CTX_new() returned NULL");
    }

    ~CAutoBN_CTX()
    {
        if (pctx != NULL)
            BN_CTX_free(pctx);
    }

    operator BN_CTX*() { return pctx; }
    BN_CTX& operator*() { return *pctx; }
    BN_CTX** operator&() { return &pctx; }
    bool operator!() { return (pctx == NULL); }
};


#if OPENSSL_VERSION_NUMBER < 0x10100000L
/** C++ wrapper for BIGNUM (OpenSSL 1.0 bignum) */
class CBigNum : public BIGNUM
{
#else
  /** C++ wrapper for BIGNUM (OpenSSL 1.1 bignum) */
class CBigNum
{
protected:
    BIGNUM *bn;

    void CBigNum_init()
    {
        bn = BN_secure_new();
    }

#endif
public:
    CBigNum()
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_init(this);
#else
        CBigNum_init();
#endif
    }

    CBigNum(const CBigNum& b)
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_init(this);
        if (!BN_copy(this, &b))
        {
            BN_clear_free(this);
            throw bignum_error("CBigNum::CBigNum(const CBigNum&) : BN_copy failed");
        }
#else
        CBigNum_init();
        if (!BN_copy(bn, &b))
        {
            BN_clear_free(bn);
            throw bignum_error("CBigNum::CBigNum(const CBigNum&) : BN_copy failed");
        }
#endif
    }

    CBigNum& operator=(const CBigNum& b)
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (!BN_copy(this, &b))
#else
        if (!BN_copy(bn, &b))
#endif
            throw bignum_error("CBigNum::operator= : BN_copy failed");
        return (*this);
    }

    ~CBigNum()
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_clear_free(this);
#else
        BN_clear_free(bn);
#endif
    }

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    BIGNUM *operator &() const
    {
        return bn;
    }
#endif

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    //CBigNum(char n) is not portable.  Use 'signed char' or 'unsigned char'.
    CBigNum(signed char n)        { BN_init(this); if (n >= 0) setulong(n); else setint64(n); }
    CBigNum(short n)              { BN_init(this); if (n >= 0) setulong(n); else setint64(n); }
    CBigNum(int n)                { BN_init(this); if (n >= 0) setulong(n); else setint64(n); }
    CBigNum(long n)               { BN_init(this); if (n >= 0) setulong(n); else setint64(n); }
    CBigNum(long long n)          { BN_init(this); setint64(n); }
    CBigNum(unsigned char n)      { BN_init(this); setulong(n); }
    CBigNum(unsigned short n)     { BN_init(this); setulong(n); }
    CBigNum(unsigned int n)       { BN_init(this); setulong(n); }
    CBigNum(unsigned long n)      { BN_init(this); setulong(n); }
    CBigNum(unsigned long long n) { BN_init(this); setuint64(n); }
    explicit CBigNum(uint256 n)   { BN_init(this); setuint256(n); }
#else
    //CBigNum(char n) is not portable.  Use 'signed char' or 'unsigned char'.
    CBigNum(signed char n)        { CBigNum_init(); if (n >= 0) setulong(n); else setint64(n); }
    CBigNum(short n)              { CBigNum_init(); if (n >= 0) setulong(n); else setint64(n); }
    CBigNum(int n)                { CBigNum_init(); if (n >= 0) setulong(n); else setint64(n); }
    CBigNum(long n)               { CBigNum_init(); if (n >= 0) setulong(n); else setint64(n); }
    CBigNum(long long n)          { CBigNum_init(); setint64(n); }
    CBigNum(unsigned char n)      { CBigNum_init(); setulong(n); }
    CBigNum(unsigned short n)     { CBigNum_init(); setulong(n); }
    CBigNum(unsigned int n)       { CBigNum_init(); setulong(n); }
    CBigNum(unsigned long n)      { CBigNum_init(); setulong(n); }
    CBigNum(unsigned long long n) { CBigNum_init(); setuint64(n); }
    explicit CBigNum(uint256 n)   { CBigNum_init(); setuint256(n); }
#endif

    explicit CBigNum(const std::vector<unsigned char>& vch)
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_init(this);
#else
        CBigNum_init();
#endif
        setvch(vch);
    }

    void setulong(unsigned long n)
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (!BN_set_word(this, n))
            throw bignum_error("CBigNum conversion from unsigned long : BN_set_word failed");
#else
        if (!BN_set_word(bn, n))
            throw bignum_error("CBigNum conversion from unsigned long : BN_set_word failed");
#endif
    }

    unsigned long getulong() const
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        return BN_get_word(this);
#else
        return BN_get_word(bn);
#endif
    }

    unsigned int getuint() const
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        return BN_get_word(this);
#else
        return BN_get_word(bn);
#endif
    }

    int getint() const
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        unsigned long n = BN_get_word(this);
        if (!BN_is_negative(this))
#else
        unsigned long n = BN_get_word(bn);
        if (!BN_is_negative(bn))
#endif
            return (n > (unsigned long)std::numeric_limits<int>::max() ? std::numeric_limits<int>::max() : n);
        else
            return (n > (unsigned long)std::numeric_limits<int>::max() ? std::numeric_limits<int>::min() : -(int)n);
    }

    void setint64(int64_t sn)
    {
        unsigned char pch[sizeof(sn) + 6];
        unsigned char* p = pch + 4;
        bool fNegative;
        uint64_t n;

        if (sn < (int64_t)0)
        {
            // Since the minimum signed integer cannot be represented as positive so long as its type is signed, 
            // and it's not well-defined what happens if you make it unsigned before negating it,
            // we instead increment the negative integer by 1, convert it, then increment the (now positive) unsigned integer by 1 to compensate
            n = -(sn + 1);
            ++n;
            fNegative = true;
        } else {
            n = sn;
            fNegative = false;
        }

        bool fLeadingZeroes = true;
        for (int i = 0; i < 8; i++)
        {
            unsigned char c = (n >> 56) & 0xff;
            n <<= 8;
            if (fLeadingZeroes)
            {
                if (c == 0)
                    continue;
                if (c & 0x80)
                    *p++ = (fNegative ? 0x80 : 0);
                else if (fNegative)
                    c |= 0x80;
                fLeadingZeroes = false;
            }
            *p++ = c;
        }
        unsigned int nSize = p - (pch + 4);
        pch[0] = (nSize >> 24) & 0xff;
        pch[1] = (nSize >> 16) & 0xff;
        pch[2] = (nSize >> 8) & 0xff;
        pch[3] = (nSize) & 0xff;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_mpi2bn(pch, p - pch, this);
#else
        BN_mpi2bn(pch, p - pch, bn);
#endif
    }

    void setuint64(uint64_t n)
    {
        unsigned char pch[sizeof(n) + 6];
        unsigned char* p = pch + 4;
        bool fLeadingZeroes = true;
        for (int i = 0; i < 8; i++)
        {
            unsigned char c = (n >> 56) & 0xff;
            n <<= 8;
            if (fLeadingZeroes)
            {
                if (c == 0)
                    continue;
                if (c & 0x80)
                    *p++ = 0;
                fLeadingZeroes = false;
            }
            *p++ = c;
        }
        unsigned int nSize = p - (pch + 4);
        pch[0] = (nSize >> 24) & 0xff;
        pch[1] = (nSize >> 16) & 0xff;
        pch[2] = (nSize >> 8) & 0xff;
        pch[3] = (nSize) & 0xff;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_mpi2bn(pch, p - pch, this);
#else
        BN_mpi2bn(pch, p - pch, bn);
#endif
    }

    void setuint256(uint256 n)
    {
        unsigned char pch[sizeof(n) + 6];
        unsigned char* p = pch + 4;
        bool fLeadingZeroes = true;
        unsigned char* pbegin = (unsigned char*)&n;
        unsigned char* psrc = pbegin + sizeof(n);
        while (psrc != pbegin)
        {
            unsigned char c = *(--psrc);
            if (fLeadingZeroes)
            {
                if (c == 0)
                    continue;
                if (c & 0x80)
                    *p++ = 0;
                fLeadingZeroes = false;
            }
            *p++ = c;
        }
        unsigned int nSize = p - (pch + 4);
        pch[0] = (nSize >> 24) & 0xff;
        pch[1] = (nSize >> 16) & 0xff;
        pch[2] = (nSize >> 8) & 0xff;
        pch[3] = (nSize >> 0) & 0xff;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_mpi2bn(pch, p - pch, this);
#else
        BN_mpi2bn(pch, p - pch, bn);
#endif
    }

    uint256 getuint256() const
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        unsigned int nSize = BN_bn2mpi(this, NULL);
#else
        unsigned int nSize = BN_bn2mpi(bn, NULL);
#endif

        uint256 n;
        n.SetNull();
        if (nSize < 4)
            return n;
        std::vector<unsigned char> vch(nSize);
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_bn2mpi(this, &vch[0]);
#else
        BN_bn2mpi(bn, &vch[0]);
#endif
        if (vch.size() > 4)
            vch[4] &= 0x7f;
       
        for (unsigned int i = 0, j = vch.size()-1; i < sizeof(n) && j >= 4; i++, j--)
            ((unsigned char*)&n)[i] = vch[j];
        return n;
    }

    void setvch(const std::vector<unsigned char>& vch)
    {
        std::vector<unsigned char> vch2(vch.size() + 4);
        unsigned int nSize = vch.size();
        // BIGNUM's byte stream format expects 4 bytes of
        // big endian size data info at the front
        vch2[0] = (nSize >> 24) & 0xff;
        vch2[1] = (nSize >> 16) & 0xff;
        vch2[2] = (nSize >> 8) & 0xff;
        vch2[3] = (nSize >> 0) & 0xff;
        // swap data to big endian
        reverse_copy(vch.begin(), vch.end(), vch2.begin() + 4);
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_mpi2bn(&vch2[0], vch2.size(), this);
#else
        BN_mpi2bn(&vch2[0], vch2.size(), bn);
#endif
    }

    std::vector<unsigned char> getvch() const
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        unsigned int nSize = BN_bn2mpi(this, NULL);
#else
        unsigned int nSize = BN_bn2mpi(bn, NULL);
#endif
        if (nSize <= 4)
            return std::vector<unsigned char>();
        std::vector<unsigned char> vch(nSize);
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        BN_bn2mpi(this, &vch[0]);
#else
        BN_bn2mpi(bn, &vch[0]);
#endif
        vch.erase(vch.begin(), vch.begin() + 4);
        reverse(vch.begin(), vch.end());
        return vch;
    }

    // The "compact" format is a representation of a whole
    // number N using an unsigned 32bit number similar to a
    // floating point format.
    // The most significant 8 bits are the unsigned exponent of base 256.
    // This exponent can be thought of as "number of bytes of N".
    // The lower 23 bits are the mantissa.
    // Bit number 24 (0x800000) represents the sign of N.
    // N = (-1^sign) * mantissa * 256^(exponent-3)
    //
    // Satoshi's original implementation used BN_bn2mpi() and BN_mpi2bn().
    // MPI uses the most significant bit of the first byte as sign.
    // Thus 0x1234560000 is compact (0x05123456)
    // and  0xc0de000000 is compact (0x0600c0de)
    // (0x05c0de00) would be -0x40de000000
    //
    // Bitcoin only uses this "compact" format for encoding difficulty
    // targets, which are unsigned 256bit quantities.  Thus, all the
    // complexities of the sign bit and using base 256 are probably an
    // implementation accident.
    //
    // This implementation directly uses shifts instead of going
    // through an intermediate MPI representation.
    CBigNum& SetCompact(unsigned int nCompact)
    {
        unsigned int nSize = nCompact >> 24;
        bool fNegative     =(nCompact & 0x00800000) != 0;
        unsigned int nWord = nCompact & 0x007fffff;
        if (nSize <= 3)
        {
            nWord >>= 8*(3-nSize);
#if OPENSSL_VERSION_NUMBER < 0x10100000L
            BN_set_word(this, nWord);
        }
        else
        {
            BN_set_word(this, nWord);
            BN_lshift(this, this, 8*(nSize-3));
        }
        BN_set_negative(this, fNegative);
#else
	    BN_set_word(bn, nWord);
        }
        else
        {
            BN_set_word(bn, nWord);
            BN_lshift(bn, bn, 8*(nSize-3));
        }
        BN_set_negative(bn, fNegative);
#endif
        return *this;
    }

    unsigned int GetCompact() const
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        unsigned int nSize = BN_num_bytes(this);
#else
	unsigned int nSize = BN_num_bytes(bn);
#endif
        unsigned int nCompact = 0;
        if (nSize <= 3)
#if OPENSSL_VERSION_NUMBER < 0x10100000L
            nCompact = BN_get_word(this) << 8*(3-nSize);
        else
        {
            CBigNum bn;
            BN_rshift(&bn, this, 8*(nSize-3));
            nCompact = BN_get_word(&bn);
#else
	    nCompact = BN_get_word(bn) << 8*(3-nSize);
        else
        {
            CBigNum cbn;
            BN_rshift(cbn.bn, bn, 8*(nSize-3));
            nCompact = BN_get_word(bn);
#endif
        }
        // The 0x00800000 bit denotes the sign.
        // Thus, if it is already set, divide the mantissa by 256 and increase the exponent.
        if (nCompact & 0x00800000)
        {
            nCompact >>= 8;
            nSize++;
        }
        nCompact |= nSize << 24;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        nCompact |= (BN_is_negative(this) ? 0x00800000 : 0);
#else
	nCompact |= (BN_is_negative(bn) ? 0x00800000 : 0);
#endif
        return nCompact;
    }

    void SetHex(const std::string& str)
    {
        // skip 0x
        const char* psz = str.c_str();
        while (isspace(*psz))
            psz++;
        bool fNegative = false;
        if (*psz == '-')
        {
            fNegative = true;
            psz++;
        }
        if (psz[0] == '0' && tolower(psz[1]) == 'x')
            psz += 2;
        while (isspace(*psz))
            psz++;

        // hex string to bignum
        *this = 0;
        int n;
        while ((n = HexDigit(*psz)) != -1)
        {
            *this <<= 4;
            *this += n;
            ++psz;
        }
        if (fNegative)
            *this = 0 - *this;
    }

    std::string ToString(int nBase=10) const
    {
        CAutoBN_CTX pctx;
        CBigNum bnBase = nBase;
        CBigNum bn0 = 0;
        std::string str;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        CBigNum bn = *this;
        BN_set_negative(&bn, false);
#else
        CBigNum bn1 = *this;
        BN_set_negative(&bn1, false);
#endif
        CBigNum dv;
        CBigNum rem;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (BN_cmp(&bn, &bn0) == 0)
#else
        if (BN_cmp(&bn1, &bn0) == 0)
#endif
            return "0";
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        while (BN_cmp(&bn, &bn0) > 0)
#else
        while (BN_cmp(&bn1, &bn0) > 0)
#endif
        {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
            if (!BN_div(&dv, &rem, &bn, &bnBase, pctx))
#else
            if (!BN_div(&dv, &rem, &bn1, &bnBase, pctx))
#endif
                throw bignum_error("CBigNum::ToString() : BN_div failed");
#if OPENSSL_VERSION_NUMBER < 0x10100000L
            bn = dv;
#else
            bn1 = dv;
#endif
            unsigned int c = rem.getulong();
            str += "0123456789abcdef"[c];
        }
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (BN_is_negative(this))
#else
        if (BN_is_negative(bn))
#endif
            str += "-";
        reverse(str.begin(), str.end());
        return str;
    }

    std::string GetHex() const
    {
        return ToString(16);
    }

    unsigned int GetSerializeSize(int nType=0, int nVersion=PROTOCOL_VERSION) const
    {
        return ::GetSerializeSize(getvch(), nType, nVersion);
    }

    template<typename Stream>
    void Serialize(Stream& s, int nType=0, int nVersion=PROTOCOL_VERSION) const
    {
        ::Serialize(s, getvch(), nType, nVersion);
    }

    template<typename Stream>
    void Unserialize(Stream& s, int nType=0, int nVersion=PROTOCOL_VERSION)
    {
        std::vector<unsigned char> vch;
        ::Unserialize(s, vch, nType, nVersion);
        setvch(vch);
    }


    bool operator!() const
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        return BN_is_zero(this);
#else
        return BN_is_zero(bn);
#endif
    }

    CBigNum& operator+=(const CBigNum& b)
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (!BN_add(this, this, &b))
#else
        if (!BN_add(bn, bn, &b))
#endif
            throw bignum_error("CBigNum::operator+= : BN_add failed");
        return *this;
    }

    CBigNum& operator-=(const CBigNum& b)
    {
        *this = *this - b;
        return *this;
    }

    CBigNum& operator*=(const CBigNum& b)
    {
        CAutoBN_CTX pctx;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (!BN_mul(this, this, &b, pctx))
#else
        if (!BN_mul(bn, bn, &b, pctx))
#endif
            throw bignum_error("CBigNum::operator*= : BN_mul failed");
        return *this;
    }

    CBigNum& operator/=(const CBigNum& b)
    {
        *this = *this / b;
        return *this;
    }

    CBigNum& operator%=(const CBigNum& b)
    {
        *this = *this % b;
        return *this;
    }

    CBigNum& operator<<=(unsigned int shift)
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (!BN_lshift(this, this, shift))
#else
        if (!BN_lshift(bn, bn, shift))
#endif
            throw bignum_error("CBigNum:operator<<= : BN_lshift failed");
        return *this;
    }

    CBigNum& operator>>=(unsigned int shift)
    {
        // Note: BN_rshift segfaults on 64-bit if 2^shift is greater than the number
        //   if built on ubuntu 9.04 or 9.10, probably depends on version of OpenSSL
        CBigNum a = 1;
        a <<= shift;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (BN_cmp(&a, this) > 0)
#else
        if (BN_cmp(&a, bn) > 0)
#endif
        {
            *this = 0;
            return *this;
        }

#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (!BN_rshift(this, this, shift))
#else
        if (!BN_rshift(bn, bn, shift))
#endif
            throw bignum_error("CBigNum:operator>>= : BN_rshift failed");
        return *this;
    }


    CBigNum& operator++()
    {
        // prefix operator
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (!BN_add(this, this, BN_value_one()))
#else
        if (!BN_add(bn, bn, BN_value_one()))
#endif
            throw bignum_error("CBigNum::operator++ : BN_add failed");
        return *this;
    }

    const CBigNum operator++(int)
    {
        // postfix operator
        const CBigNum ret = *this;
        ++(*this);
        return ret;
    }

    CBigNum& operator--()
    {
        // prefix operator
        CBigNum r;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (!BN_sub(&r, this, BN_value_one()))
#else
        if (!BN_sub(&r, bn, BN_value_one()))
#endif
            throw bignum_error("CBigNum::operator-- : BN_sub failed");
        *this = r;
        return *this;
    }

    const CBigNum operator--(int)
    {
        // postfix operator
        const CBigNum ret = *this;
        --(*this);
        return ret;
    }


    friend inline const CBigNum operator-(const CBigNum& a, const CBigNum& b);
    friend inline const CBigNum operator/(const CBigNum& a, const CBigNum& b);
    friend inline const CBigNum operator%(const CBigNum& a, const CBigNum& b);
};



inline const CBigNum operator+(const CBigNum& a, const CBigNum& b)
{
    CBigNum r;
    if (!BN_add(&r, &a, &b))
        throw bignum_error("CBigNum::operator+ : BN_add failed");
    return r;
}

inline const CBigNum operator-(const CBigNum& a, const CBigNum& b)
{
    CBigNum r;
    if (!BN_sub(&r, &a, &b))
        throw bignum_error("CBigNum::operator- : BN_sub failed");
    return r;
}

inline const CBigNum operator-(const CBigNum& a)
{
    CBigNum r(a);
    BN_set_negative(&r, !BN_is_negative(&r));
    return r;
}

inline const CBigNum operator*(const CBigNum& a, const CBigNum& b)
{
    CAutoBN_CTX pctx;
    CBigNum r;
    if (!BN_mul(&r, &a, &b, pctx))
        throw bignum_error("CBigNum::operator* : BN_mul failed");
    return r;
}

inline const CBigNum operator/(const CBigNum& a, const CBigNum& b)
{
    CAutoBN_CTX pctx;
    CBigNum r;
    if (!BN_div(&r, NULL, &a, &b, pctx))
        throw bignum_error("CBigNum::operator/ : BN_div failed");
    return r;
}

inline const CBigNum operator%(const CBigNum& a, const CBigNum& b)
{
    CAutoBN_CTX pctx;
    CBigNum r;
    if (!BN_mod(&r, &a, &b, pctx))
        throw bignum_error("CBigNum::operator% : BN_div failed");
    return r;
}

inline const CBigNum operator<<(const CBigNum& a, unsigned int shift)
{
    CBigNum r;
    if (!BN_lshift(&r, &a, shift))
        throw bignum_error("CBigNum:operator<< : BN_lshift failed");
    return r;
}

inline const CBigNum operator>>(const CBigNum& a, unsigned int shift)
{
    CBigNum r = a;
    r >>= shift;
    return r;
}

inline bool operator==(const CBigNum& a, const CBigNum& b) { return (BN_cmp(&a, &b) == 0); }
inline bool operator!=(const CBigNum& a, const CBigNum& b) { return (BN_cmp(&a, &b) != 0); }
inline bool operator<=(const CBigNum& a, const CBigNum& b) { return (BN_cmp(&a, &b) <= 0); }
inline bool operator>=(const CBigNum& a, const CBigNum& b) { return (BN_cmp(&a, &b) >= 0); }
inline bool operator<(const CBigNum& a, const CBigNum& b)  { return (BN_cmp(&a, &b) < 0); }
inline bool operator>(const CBigNum& a, const CBigNum& b)  { return (BN_cmp(&a, &b) > 0); }

#endif
