#include "MEParams.h"

MEParams::MEParams()
{
    cInitMEParams((cMEParams*)this);
}

MEParams::MEParams(MEParams& op)
{
    cCopyMEParams(this, (cMEParams*) &op);
}

MEParams& MEParams::operator= (MEParams& op)
{
    cDeInitMEParams((cMEParams*)this);
    cCopyMEParams(this, &op);
    return *this;
}

void MEParams::SetMotionArea( int iMaxMotHor, int iMaxMotVert )
{
    cSetMotionAreaME( this, iMaxMotHor, iMaxMotVert );
}

void MEParams::SetMEAlg( MEAlgorithm meAlg )
{
    cSetMEAlgME( this, meAlg );
}

void MEParams::SetPrecision( EstimationPrecision precMX, EstimationPrecision precMY )
{
    cSetPrecisionME( this, precMX, precMY );
}

void MEParams::SetInterlaced( BOOL bIntrlcd )
{
    cSetInterlacedME( this, bIntrlcd );
}

void MEParams::SetBlockSize( cMacroBlockPartitions MaxBlSz, cMacroBlockPartitions MinBlSz )
{
    cSetBlockSizeME( this, MaxBlSz, MinBlSz );
}

void MEParams::SetBordersAllowed( BOOL bAllow )
{
    cSetAllowBordersME( this, bAllow );
}

void MEParams::SetInterpAlg( InterpolationAlgorithm interpAlg )
{
    cSetInterlacedME( this, interpAlg );
}

void MEParams::SetRDOInfo( void* pRDOInfo )
{
    cSetRDOInfoME( this, pRDOInfo );
}

void MEParams::SetSADFunctions( BlockDiffFunction pGetError_16x16, BlockDiffFunction pGetError_16x8,
                                  BlockDiffFunction pGetError_8x8, BlockDiffFunction pGetError_4x4 )
{
    cSetSADFunctionsME( this, pGetError_16x16, pGetError_16x8, pGetError_8x8, pGetError_4x4 );
}

void MEParams::SetSADFunctionsChroma( BlockDiffFunction pGetErrorChr_16x16,
                                        BlockDiffFunction pGetErrorChr_8x8,
                                        BlockDiffFunction pGetErrorChr_4x4 )
{
    cSetSADFunctionsChromaME( this, pGetErrorChr_16x16, pGetErrorChr_8x8, pGetErrorChr_4x4 );
}

void MEParams::SetChromaSettings( BOOL ConsiderChroma, SuitableChromaModifiers ChrXMod, SuitableChromaModifiers ChrYMod )
{
    cSetChromaSettingsME( this, ConsiderChroma, ChrXMod, ChrYMod );
}

void MEParams::SetHierReduction( SuitableHierReduction HierReduction )
{
    cSetHierReductionME( this, HierReduction );
}

void MEParams::SetStatisticFileName( BOOL bSaveStat, char* sName )
{
    cSetStatisticFileNameME( this, bSaveStat, sName );
}

void MEParams::SetCoefficients( float fMeanErrorPP, float fMeanErrRenew, float fDispersionPP, float fDispersionRenew )
{
    cSetCoefficientsME( this, fMeanErrorPP, fMeanErrRenew, fDispersionPP, fDispersionRenew );
}

int MEParams::GetMaxMotHor()
{
    return m_iMaxMotHor;
}

int MEParams::GetMaxMotVert()
{
    return m_iMaxMotVert;
}

MEAlgorithm MEParams::GetMEAlg(  )
{
    return m_MEAlgorithm;
}

EstimationPrecision MEParams::GetPrecisionMX()
{
    return m_PrecMX;
}

EstimationPrecision MEParams::GetPrecisionMY()
{
    return m_PrecMY;
}


void MEParams::GetInterlaced( BOOL& bIntrlcd )
{
    bIntrlcd = m_bIsInterlaced;
}

cMacroBlockPartitions MEParams::GetMaxBlockSize()
{
    return m_MaxBlSize;
}

cMacroBlockPartitions MEParams::GetMinBlockSize()
{
    return m_MinBlSize;
}

BOOL MEParams::GetBordersAllowed()
{
    return m_bBordersAllowed;
}

InterpolationAlgorithm MEParams::GetInterpAlg()
{
    return m_interAlg;
}

void MEParams::GetSADFunctions( BlockDiffFunction& pGetError_16x16, BlockDiffFunction& pGetError_16x8,
                                BlockDiffFunction& pGetError_8x8, BlockDiffFunction& pGetError_4x4 )
{
    pGetError_16x16 = m_advMEParams.pGetError_16x16;
    pGetError_16x8  = m_advMEParams.pGetError_16x8;
    pGetError_8x8   = m_advMEParams.pGetError_8x8;
    pGetError_4x4   = m_advMEParams.pGetError_4x4;
}

void MEParams::GetSADFunctionsChroma( BlockDiffFunction& pGetErrorChr_16x16,
                                      BlockDiffFunction& pGetErrorChr_8x8,
                                      BlockDiffFunction& pGetErrorChr_4x4 )
{
    pGetErrorChr_16x16 = m_advMEParams.pGetErrorChr_16x16;
    pGetErrorChr_8x8   = m_advMEParams.pGetErrorChr_8x8;
    pGetErrorChr_4x4   = m_advMEParams.pGetErrorChr_4x4;
}

void MEParams::GetChromaSettings( BOOL& bConsiderChroma, SuitableChromaModifiers& ChrXMod, SuitableChromaModifiers& ChrYMod )
{
    bConsiderChroma = m_advMEParams.m_bConsiderChroma;
    ChrXMod         = m_advMEParams.m_ChrXMod;
    ChrYMod         = m_advMEParams.m_ChrYMod;
}

SuitableHierReduction MEParams::GetHierReduction()
{
    return m_advMEParams.m_HierarchicReduction;
}

void MEParams::GetCoefficients( float fMeanErrorPP, float fMeanErrRenew, float fDispersionPP, float fDispersionRenew )
{
    fMeanErrorPP     = m_devMEParams.m_fMeanErrorPP;
    fMeanErrRenew    = m_devMEParams.m_fMeanErrRenew;
    fDispersionPP    = m_devMEParams.m_fDispersionPP;
    fDispersionRenew = m_devMEParams.m_fDispersionRenew;
}

BOOL MEParams::GetSaveStaticstic()
{
    return m_devMEParams.m_bSaveStatistic;
}

char* MEParams::GetStatisticFileName()
{
    return m_devMEParams.m_sFileName;
}

MEParams::~MEParams()
{
    cDeInitMEParams((cMEParams*)this);
}
