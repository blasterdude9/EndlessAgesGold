//---------------------------------------------------------------------------
// Copyright (C) 1998, Interscope Ltd. All rights reserved.
// Reproduction or distribution of this program, or any portion of it, 
// is permitted only if this header is kept as it is.
// For more information, contact:
//
// Interscope Ltd., 5 Culturii St., 5th Floor, 4800 Baia Mare, RO
//    Phone/Fax: +40-62-415023
//    E-mail: office@interscope.ro
//
//   $Author: Levente Farkas $
//     $Date: 3/11/98 4:10p $
//  $Modtime: 3/11/98 3:03p $
// $Revision: 17 $
//  $Archive: /Interscope/Cabinet/Main.cpp $
// $Workfile: Main.cpp $
//-----------------------------------------------------------------------

#include "Trace.H"
#include "Cabinet.Hpp"


char *return_fci_error_string(int err)
{
	switch (err)
	{
		case FCIERR_NONE:
			return "No error";

		case FCIERR_OPEN_SRC:
			return "Failure opening file to be stored in cabinet";
		
		case FCIERR_READ_SRC:
			return "Failure reading file to be stored in cabinet";
		
		case FCIERR_ALLOC_FAIL:
			return "Insufficient memory in FCI";

		case FCIERR_TEMP_FILE:
			return "Could not create a temporary file";

		case FCIERR_BAD_COMPR_TYPE:
			return "Unknown compression type";

		case FCIERR_CAB_FILE:
			return "Could not create cabinet file";

		case FCIERR_USER_ABORT:
			return "Client requested abort";

		case FCIERR_MCI_FAIL:
			return "Failure compressing data";

		default:
			return "Unknown error";
	}
}

char *return_fdi_error_string(int err)
{
	switch (err)
	{
		case FDIERROR_NONE:
			return "No error";

		case FDIERROR_CABINET_NOT_FOUND:
			return "Cabinet not found";
			
		case FDIERROR_NOT_A_CABINET:
			return "Not a cabinet";
			
		case FDIERROR_UNKNOWN_CABINET_VERSION:
			return "Unknown cabinet version";
			
		case FDIERROR_CORRUPT_CABINET:
			return "Corrupt cabinet";
			
		case FDIERROR_ALLOC_FAIL:
			return "Memory allocation failed";
			
		case FDIERROR_BAD_COMPR_TYPE:
			return "Unknown compression type";
			
		case FDIERROR_MDI_FAIL:
			return "Failure decompressing data";
			
		case FDIERROR_TARGET_FILE:
			return "Failure writing to target file";
			
		case FDIERROR_RESERVE_MISMATCH:
			return "Cabinets in set have different RESERVE sizes";
			
		case FDIERROR_WRONG_CABINET:
			return "Cabinet returned on fdintNEXT_CABINET is incorrect";
			
		case FDIERROR_USER_ABORT:
			return "User aborted";
			
		default:
			return "Unknown error";
	}
}

void main()
{   
    ///*
    CCabinetBuilder cb(12345,1440000,1440000,250000);
    if(cb.InitCabinet("D:\\Temp\\Test","Test","Disk"))
    {
        if(!cb.AddFile("D:\\Temp\\Reboot.Log") ||
           !cb.AddFile("D:\\Temp\\sysreport.bmp") ||
           !cb.AddFile("D:\\Temp\\levyaway.txt") ||
           !cb.AddFile("D:\\Temp\\Blackbug.avi") ||
           !cb.AddFile("K:\\Microsoft\\Outlook 98 Beta 2\\mpi95_2s.cab"))
            TRACE("Could not add file (%s [%d][%d][%d])\n",return_fci_error_string(cb.GetErrorCode()),cb.GetErrorCode(),cb.GetErrorCodeEx(),errno);
        //cb.FlushCabinet();
    }
    //*/
    /*
    CCabinetExtractor ce;
    ce.SetDefaultExtractPath("D:\\Temp\\Extract");

    TRACE("Error code checking (%s [%d][%d])\n",return_fdi_error_string(ce.GetErrorCode()),ce.GetErrorCode(),ce.GetErrorCodeEx());
    if(!ce.ExtractFiles("D:\\Temp\\Test\\Setup1.cab"))
        TRACE("Error extracting file(s) (%s [%d][%d])\n",return_fdi_error_string(ce.GetErrorCode()),ce.GetErrorCode(),ce.GetErrorCodeEx());
    */
}

