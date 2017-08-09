#ifndef __MEPARAMS_H__
#define __MEPARAMS_H__

extern "C"
{
#include "MEParams/cMEParams.h"
};

class MEParams : private cMEParams
{
public:
    MEParams();

    MEParams(MEParams& op);

    MEParams& operator= (MEParams& op);

    void SetMotionArea( int iMaxMotHor, int iMaxMotVert );
    void SetMEAlg( MEAlgorithm meAlg );
    void SetPrecision( EstimationPrecision precMX, EstimationPrecision precMY );
    void SetInterlaced( BOOL bIntrlcd );
    void SetBlockSize( cMacroBlockPartitions MaxBlSz, cMacroBlockPartitions MinBlSz );
    void SetBordersAllowed( BOOL bAllow );
    void SetInterpAlg( InterpolationAlgorithm interpAlg );
    void SetRDOInfo( void* pRDOInfo );
    void SetSADFunctions( BlockDiffFunction pGetError_16x16, BlockDiffFunction pGetError_16x8,
                            BlockDiffFunction pGetError_8x8, BlockDiffFunction pGetError_4x4 );
    void SetSADFunctionsChroma( BlockDiffFunction pGetErrorChr_16x16, BlockDiffFunction pGetErrorChr_8x8, BlockDiffFunction pGetErrorChr_4x4 );
    void SetChromaSettings( BOOL ConsiderChroma, SuitableChromaModifiers ChrXMod, SuitableChromaModifiers ChrYMod );
    void SetHierReduction( SuitableHierReduction HierReduction );
    void SetStatisticFileName( BOOL bSaveStat, char* sName );
    void SetCoefficients( float fMeanErrorPP, float fMeanErrRenew, float fDispersionPP, float fDispersionRenew );

    int GetMaxMotHor();
    int GetMaxMotVert();
    MEAlgorithm GetMEAlg();
    EstimationPrecision GetPrecisionMX();
    EstimationPrecision GetPrecisionMY();
    void GetInterlaced( BOOL& bIntrlcd );
    cMacroBlockPartitions GetMaxBlockSize();
    cMacroBlockPartitions GetMinBlockSize();
    BOOL GetBordersAllowed();
    InterpolationAlgorithm GetInterpAlg();
    void GetSADFunctions( BlockDiffFunction& pGetError_16x16, BlockDiffFunction& pGetError_16x8,
                            BlockDiffFunction& pGetError_8x8, BlockDiffFunction& pGetError_4x4 );
    void GetSADFunctionsChroma( BlockDiffFunction& pGetErrorChr_16x16, BlockDiffFunction& pGetErrorChr_8x8, BlockDiffFunction& pGetErrorChr_4x4 );
    void GetChromaSettings( BOOL& ConsiderChroma, SuitableChromaModifiers& ChrXMod, SuitableChromaModifiers& ChrYMod );
    SuitableHierReduction GetHierReduction();
    void GetCoefficients( float fMeanErrorPP, float fMeanErrRenew, float fDispersionPP, float fDispersionRenew );
    BOOL GetSaveStaticstic();
    char* GetStatisticFileName();

    virtual ~MEParams();
};

#endif // __MEPARAMS_H__