/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is Netscape Communications
 * Corporation.  Portions created by Netscape are Copyright (C) 1998
 * Netscape Communications Corporation.  All Rights Reserved.
 */

#include "nsUnicodeToEUCKR.h"
#include "nsUCvKODll.h"

//----------------------------------------------------------------------
// Global functions and data [declaration]


static PRUint16 gAsciiShiftTable[] =  {
  0, u1ByteCharset,  
  ShiftCell(0,   0, 0, 0, 0, 0, 0, 0),
};

static PRUint16 gKSC5601ShiftTable[] =  {
  0, u2BytesGRCharset,  
  ShiftCell(0,   0, 0, 0, 0, 0, 0, 0),
};

static PRUint16 *g_EUCKRMappingTable[2] = {
  g_AsciiMapping,
  g_ufKSC5601Mapping
};

static PRUint16 *g_EUCKRShiftTable[2] =  {
  gAsciiShiftTable,
  gKSC5601ShiftTable
};

//----------------------------------------------------------------------
// Class nsUnicodeToEUCKR [implementation]

nsUnicodeToEUCKR::nsUnicodeToEUCKR() 
: nsMultiTableEncoderSupport(2,
                        (uShiftTable**) g_EUCKRShiftTable, 
                        (uMappingTable**) g_EUCKRMappingTable)
{
}

nsresult nsUnicodeToEUCKR::CreateInstance(nsISupports ** aResult) 
{
  nsIUnicodeEncoder *p = new nsUnicodeToEUCKR();
  if(p) {
   *aResult = p;
   return NS_OK;
  }
  return NS_ERROR_OUT_OF_MEMORY;
}

//----------------------------------------------------------------------
// Subclassing of nsTableEncoderSupport class [implementation]

NS_IMETHODIMP nsUnicodeToEUCKR::GetMaxLength(const PRUnichar * aSrc, 
                                              PRInt32 aSrcLength,
                                              PRInt32 * aDestLength)
{
  *aDestLength = aSrcLength * 2;
  return NS_OK;
}
