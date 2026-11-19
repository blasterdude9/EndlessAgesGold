//---------------------------------------------------------------------------
// Copyright (C) 1998, Interscope Ltd. All rights reserved.
// Reproduction or distribution of this program, or any portion of it, 
// is permitted only if this header is kept as it is.
// For more information, contact:
//
// Interscope Ltd., 5 Culturii St., 5th floor, 4800 Baia Mare, Romania
//    Phone/Fax: +40-62-215023
//    E-mail: office@interscope.ro
//
//   $Author: Levente Farkas $
//     $Date: 5/13/98 12:03a $
//  $Modtime: 4/27/98 6:50a $
// $Revision: 4 $
//  $Archive: /Interscope/Thebe/InstallMaster/Reminder.Hpp $
// $Workfile: Reminder.Hpp $
//---------------------------------------------------------------------------

#ifndef __Source_Code_Reminders_Hpp__
#define __Source_Code_Reminders_Hpp__

// Reminder macros, use them where you want to put reminders in your code
//
// Usage:
// #pragma message(Reminder("reminder text here"))
//
// Note:
// i)  Do not use the words "error" or "warning" in your reminder message because 
//     the C++ IDE counts them as actual errors and warning, and stops compilation
// ii) You should define a keyboard macro for inserting such reminders in your code

#define __Stringize(L)      #L
#define __MakeString(M,L)   M(L)

#define $__Line             __MakeString(__Stringize,__LINE__)
#define Reminder(msg)       __FILE__ "(" $__Line ") : Reminder : " msg


#endif



