//---------------------------------------------------------------------------
// Copyright (C) 1998, Interscope Ltd. All rights reserved.
// Reproduction or distribution of this program, or any portion of it, 
// is permitted only if this header is kept as it is.
// For more information, contact:
//
// Interscope Ltd., 5 Culturii St., 5th Floor, 4800 Baia Mare, RO
//    Phone/Fax: +40-62-215023
//    E-mail: office@interscope.ro
//
//   $Author: Levente Farkas $
//     $Date: 5/12/98 11:49p $
//  $Modtime: 4/27/98 6:50a $
// $Revision: 14 $
//  $Archive: /Interscope/Thebe/InstallMaster/DrvType.cpp $
// $Workfile: DrvType.cpp $
//-----------------------------------------------------------------------


#include "StdAfx.H"


#include <String.H>

#include "Portable.H"
#include "DrvType.H"


//--- Debugee --------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#ifdef __MFC__
#define new DEBUG_NEW
#endif // __MFC__
#endif // _DEBUG


//--- Determines the type of a drive ------------------------------------

// See the "MS-DOS Programmer's Reference" for further information
// about this structure.

struct DEVICEPARAMS
{
    BYTE  bSpecFunc;        // Special functions
    BYTE  bDevType;         // Device type
    WORD  wDevAttr;         // Device attributes
    WORD  wCylinders;       // Number of cylinders
    BYTE  bMediaType;       // Media type
    // Beginning of BIOS parameter block (BPB)
    WORD  wBytesPerSec;     // Bytes per sector
    BYTE  bSecPerClust;     // Sectors per cluster
    WORD  wResSectors;      // Number of reserved sectors
    BYTE  bFATs;            // Number of FATs
    WORD  wRootDirEnts;     // Number of root-directory entries
    WORD  wSectors;         // Total number of sectors
    BYTE  bMedia;           // Media descriptor
    WORD  wFATsecs;         // Number of sectors per FAT
    WORD  wSecPerTrack;     // Number of sectors per track
    WORD  wHeads;           // Number of heads
    DWORD dwHiddenSecs;     // Number of hidden sectors
    DWORD dwHugeSectors;    // Number of sectors if wSectors == 0
    // End of BIOS parameter block (BPB)
};

typedef DEVICEPARAMS FAR * LPDEVICEPARAMS;


//--- Forwards -----------------------------------------------------------

BOOL GetDeviceParameters(int nDrive, LPDEVICEPARAMS dp);
BOOL IsCDRomDrive(int nDrive);
UINT GetDriveTypeEx(int nDrive);


//-------------------------------------------------------------
// Pre     : nDrive   Drive number  0 = A, 1 = B, 2 = C, and so on
//           dp       Pointer to a structure that will contain the 
//                    drive's parameters
// Post    : Returns TRUE if it succeeded, FALSE if it failed
// Globals :
// I/O     :
// Task    : Fills a DEVICEPARAMS struct with info about the given drive
//           Calls DOS IOCTL Get Device Parameters (440Dh, 60h) function
//-------------------------------------------------------------
BOOL GetDeviceParameters(WORD nDrive, LPDEVICEPARAMS dp)
{
    BOOL bResult=TRUE; // Assume success
    __asm 
    {
        push ds
        mov  bx, nDrive
        inc  bx           // Convert 0-based #'s to 1-based #s
        mov  ch, 08h      // Device category--must be 08h
        mov  cl, 60h      // MS-DOS IOCTL Get Device Parameters
        lds  dx, dp
        mov  ax, 440Dh
        int  21h

        jnc  gdp_done     // CF SET if error
        mov  bResult, FALSE
    }

    gdp_done:

    __asm 
    {
        pop  ds
    }
    return bResult;
}

//-------------------------------------------------------------
// Pre     : nDrive   Drive number  0 = A, 1 = B, 2 = C, and so on
// Post    : Returns TRUE if nDrive is a CD-ROM drive, FALSE if it isn't
// Globals :
// I/O     :
// Task    : Determines if a drive is a CD-ROM. Calls MSCDEX and checks
//           that MSCDEX is loaded, and that MSCDEX reports the drive is a
//           CD-ROM
//-------------------------------------------------------------
BOOL IsCDRomDrive(WORD nDrive)
{
    BOOL bResult=FALSE; // Assume not a CD-ROM drive
    __asm
    {
        mov  ax, 1500h       // Check if MSCDEX exists
        xor  bx, bx
        int  2Fh

        or   bx, bx          // BX unchanged if MSCDEX is not around
        jz   not_cd_drive    // No

        mov  ax, 150Bh       // MSCDEX CD-ROM Drive Check
        xor  bx, bx
        mov  cx, nDrive
        int  2Fh

        cmp  bx, 0ADADh      // Check MSCDEX signature
        jne  not_cd_drive
        or   ax, ax          // Check the drive type
        jz   not_cd_drive    // 0 (zero) means not CD-ROM
        mov  bResult,TRUE
    }

    not_cd_drive:

    return bResult;
}

//-------------------------------------------------------------
// Pre     : nDrive   Drive number  0 = A, 1 = B, 2 = C, and so on
// Post    : Returns the type of drive
// Globals :
// I/O     :
// Task    : Determines the type of a drive. Calls Windows's GetDriveType
//           to determine if a drive is valid, fixed, remote, or removeable,
//           then breaks down these categories further to specific device
//           types
//-------------------------------------------------------------
UINT GetDriveTypeEx(WORD nDrive)
{
    DEVICEPARAMS dp;
    UINT uType;
    char root[4];

    ZeroMemory(&dp,sizeof(dp)); // Init device params struct

#if defined(__WIN32__) || defined(_WIN32)
    STRCPY(root,_T("C:\\"));
    root[0]=('A'+nDrive);
    uType=GetDriveType(root);
#else
    uType=GetDriveType(nDrive);
#endif

    switch(uType)
    {
        case DRIVE_REMOTE:
            // GetDriveType() reports CD-ROMs as Remote drives. Need
            // to see if the drive is a CD-ROM or a network drive
            if(IsCDRomDrive(nDrive))
                return EX_DRIVE_CDROM;
            else
                return EX_DRIVE_REMOTE;

        case DRIVE_REMOVABLE:
            // Check for a floppy disk drive. If it isn't, then we
            // don't know what kind of removable media it is
            // For example, could be a Bernoulli box or something new...
            if(GetDeviceParameters(nDrive,&dp)) 
                switch(dp.bDevType)
                {
                    // Floppy disk drive types
                    case 0x0:
                    case 0x1:
                    case 0x2:
                    case 0x3:
                    case 0x4:
                    case 0x7:
                    case 0x8: 
                        return EX_DRIVE_FLOPPY;
                }
            return EX_DRIVE_REMOVABLE; // Unknown removable media type

        case DRIVE_FIXED:
            // GetDeviceParameters returns a device type of 0x05 for
            // hard disks. Because hard disks and RAM disks are the two
            // types of fixed-media drives, we assume that any fixed-
            // media drive that isn't a hard disk is a RAM disk
            if(GetDeviceParameters(nDrive,&dp) && (dp.bDevType==0x05))
                return EX_DRIVE_FIXED;
            else
                return EX_DRIVE_RAMDISK;
    }

    return EX_DRIVE_INVALID; // Drive is invalid if we get here
}

