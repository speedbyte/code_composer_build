/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_SD_ListCtrl_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_SD 
*-----------------------------------------------------------------------------
* Description:    
*-----------------------------------------------------------------------------
* $Date: 2011-06-20 10:53:23 +0200 (Mo, 20 Jun 2011) $
* $Rev: 20286 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/src/APP_SD_ListCtrl_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"


#include "APP_SD_FrameBuilder.h"
#include "APP_SD.h"
#include "KPB_BAP.h"
#include "APP_SD_ListCtrl.h"

#include "TRACE.h"
#include "APP_SD_ListCtrl_C1.id"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/** 
\brief	variable to save the current call management function currently in progress
*/
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/
static SuccessFailType _APP_SD_LC_sfGetVisibleCells(KPB_enFrameIdType frameId, 
                                                    Uint16Type *pVisibleElems);
static SuccessFailType _APP_SD_LC_sfGetStartCellIdx(KPB_enFrameIdType frameId, 
                                                    Uint16Type *pStartPos);
static void _APP_SD_FB_sfBuildAndSendScrollBar(const AppSdFrameDataListCommon *pListCtrl);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/


/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

AppSdListBufType gListBuffer[APP_SD_MAX_LIST_REQUEST_ELEMENTS];

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/* ------------ Module Internal Interfaces ----------------------*/

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_u16GetStartIndex                                    */
/** 
  \brief        Returns the start index of the requested list  

  \param        pListCtrl
                Current list control structure

  \return       Uint16Type
                Start index

 */
/*----------------------------------------------------------------------------*/
Uint16Type APP_SD_LC_u16GetStartIndex(const AppSdFrameDataListCommon *pListCtrl)
{
    Uint16Type startElem = pListCtrl->listElemOnTop + pListCtrl->requestedElems;

    if( startElem  > pListCtrl->numListElems )
    {
        startElem = startElem - pListCtrl->numListElems;
    }

    //return pListCtrl->listElemOnTop + pListCtrl->requestedElems;
    return startElem;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_u16GetElemsLeft                                     */
/** 
  \brief        Returns the number of elements that needs to be requested  

  \param        pListCtrl
                Current list control structure

  \return       Uint16Type
                Elements left

 */
/*----------------------------------------------------------------------------*/
Uint16Type APP_SD_LC_u16GetElemsLeft(const AppSdFrameDataListCommon *pListCtrl)
{
    Uint16Type neededElems = 0;

    if(_APP_SD_LC_sfGetVisibleCells(pListCtrl->frameId, &neededElems) != SUCCESS)
    {
        DEBUG_VALUE1( APP_SD_SWI_ERR, MOD_APP_SD, "APP_SD_LC_u16GetElemsLeft: invalid frame id %d", 
                      pListCtrl->frameId );

        return 0;
    }

    if(neededElems > pListCtrl->numListElems)
    {
        neededElems = pListCtrl->numListElems;
    }

    neededElems = neededElems - pListCtrl->requestedElems;

    return neededElems;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_u16GetNextListIndex                                 */
/** 
  \brief        Returns the absolute index of next list element  

  \param        pListCtrl
                Current list control structure

  \return       Uint16Type
                Index

 */
/*----------------------------------------------------------------------------*/
Uint16Type APP_SD_LC_u16GetNextListIndex(AppSdFrameDataListCommon *pListCtrl)
{
    Uint16Type startIndex = APP_SD_LC_u16GetStartIndex(pListCtrl);

    if(APP_SD_LC_u16GetElemsLeft(pListCtrl) > 0)
    {
        return startIndex;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_vRequestData                                        */
/** 
  \brief        Interface to trigger the nex list request  

  \param        pListCtrl
                Current list control structure

  \return       void

 */
/*----------------------------------------------------------------------------*/
#ifdef APP_SD_LIST_CTRL_EXTENDED
void APP_SD_LC_vRequestData(const AppSdFrameDataListCommon *pListCtrl)
{
    Uint16Type neededElems = 0;
    Uint16Type startElem = 0;
    
    if(_APP_SD_LC_sfGetVisibleCells(pListCtrl->frameId, &neededElems) != SUCCESS)
    {
        DEBUG_VALUE1( APP_SD_SWI_ERR, MOD_APP_SD, "APP_SD_LC_vRequestData: invalid frame id %d",
                      pListCtrl->frameId );
    }

    if(neededElems > pListCtrl->numListElems)
    {
        neededElems = pListCtrl->numListElems;
    }

    neededElems = neededElems - pListCtrl->requestedElems;

    startElem = pListCtrl->listElemOnTop + pListCtrl->requestedElems;

    if( startElem > pListCtrl->numListElems )
    {
        startElem = 1;
    }
    else if((startElem + neededElems - 1) > pListCtrl->numListElems)
    {
        neededElems = pListCtrl->numListElems - startElem + 1;
    }


    APP_SD_vListDataRequest(startElem, neededElems);
}


#else
void APP_SD_LC_vRequestData(const AppSdFrameDataListCommon *pListCtrl)
{
    Uint16Type neededElems = 0;
    Uint16Type startElem = 0;
    
    if(_APP_SD_LC_sfGetVisibleCells(pListCtrl->frameId, &neededElems) != SUCCESS)
    {
        DEBUG_VALUE1( APP_SD_SWI_ERR, MOD_APP_SD, "APP_SD_LC_vRequestData: invalid frame id %d",
                      pListCtrl->frameId );
    }

    if(neededElems > pListCtrl->numListElems)
    {
        neededElems = pListCtrl->numListElems;
    }

    startElem = pListCtrl->listElemOnTop + pListCtrl->requestedElems;
    neededElems = neededElems - pListCtrl->requestedElems;

    if(startElem > pListCtrl->numListElems)
    {
        startElem  = pListCtrl->numListElems;
    }


    APP_SD_vListDataRequest(startElem, neededElems);
}
#endif


/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_bDataNeed                                           */
/** 
  \brief        Returns whether there are more elements needed or not  

  \param        pListCtrl
                Current list control structure

  \return       BooleanType
                BT_TRUE or BT_FALSE

 */
/*----------------------------------------------------------------------------*/
BooleanType APP_SD_LC_bDataNeed(AppSdFrameDataListCommon *pListCtrl)
{
    if(APP_SD_LC_u16GetElemsLeft(pListCtrl) > 0)
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_stGetListBuffer                                        */
/** 
  \brief        Returns a list buffer entry to put a list item in   

  \param        pListCtrl
                Current list control structure

  \return       AppSdListBufType
                Pointer to a list buffer entry

 */
/*----------------------------------------------------------------------------*/
AppSdListBufType *APP_SD_stGetListBuffer(AppSdFrameDataListCommon *pListCtrl)
{
    Uint16Type index = pListCtrl->requestedElems;
    
    /* check if we received enough elements */
    if(APP_SD_LC_u16GetElemsLeft(pListCtrl) == 0)
    {
        return NULL;
    }

    if(index < APP_SD_MAX_LIST_REQUEST_ELEMENTS)
    {
        pListCtrl->requestedElems++;
        (void)memset(&gListBuffer[index], 0, sizeof(gListBuffer[index]));
        return &gListBuffer[index];
    }

    return NULL;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_vScrollUpDown                                       */
/** 
  \brief        This functions is used to scroll multiple times  

  \param        pListCtrl
                Current list control structure

  \param        upOrDown
                Indicates the scroll direction (up or down)

  \param        number
                Specifies the number of scroll steps

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_SD_LC_vScrollUpDown(AppSdFrameDataListCommon *pListCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown, Uint16Type number)
{
    Uint16Type i=0;

    for(i=0; i<number; i++)
    {
        APP_SD_LC_vButtonUpDown(pListCtrl, upOrDown);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_vButtonUpDown                                       */
/** 
  \brief        This functions is used to scroll one time  

  \param        pListCtrl
                Current list control structure

  \param        upOrDown
                Indicates the scroll direction (up or down)

  \return       void

 */
/*----------------------------------------------------------------------------*/
#ifdef APP_SD_LIST_CTRL_EXTENDED
void APP_SD_LC_vButtonUpDown(AppSdFrameDataListCommon *pListCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown)
{
    Uint16Type visibleElems = 0;
    Uint16Type preListElemOnTop = pListCtrl->listElemOnTop;

    if( _APP_SD_LC_sfGetVisibleCells(pListCtrl->frameId, &visibleElems) != SUCCESS )
    {
        return;
    }
    

    if(upOrDown == enLogiButManMflUp)
    {
        if(pListCtrl->focusedListElemOffset > 0)
        {
            pListCtrl->focusedListElemOffset--;
        }
        else if(visibleElems < pListCtrl->numListElems)
        {
            if( pListCtrl->scrollType == APP_SD_LIST_SCROLL_TYPE_ENDLESS)
            {
                if( pListCtrl->listElemOnTop == 1 )
                {
                    pListCtrl->listElemOnTop = pListCtrl->numListElems;
                }
                else
                {
                    pListCtrl->listElemOnTop--;
                }
            }
            else
            {
                if( pListCtrl->listElemOnTop > 1 )
                {
                    pListCtrl->listElemOnTop--;
                }
            }
        }
    }
    else
    {
        visibleElems = visibleElems < pListCtrl->numListElems ? 
                                      visibleElems : pListCtrl->numListElems;

        if(pListCtrl->focusedListElemOffset < visibleElems - 1 )
        {
            pListCtrl->focusedListElemOffset++;
        }
        else if(visibleElems < pListCtrl->numListElems)
        {
            if( pListCtrl->scrollType == APP_SD_LIST_SCROLL_TYPE_ENDLESS)
            {
                if(pListCtrl->listElemOnTop < pListCtrl->numListElems)
                {
                    pListCtrl->listElemOnTop++;
                } 
                else
                {
                    pListCtrl->listElemOnTop = 1;
                }
            }
            else
            {
                if(pListCtrl->listElemOnTop + visibleElems <= pListCtrl->numListElems)
                {
                    pListCtrl->listElemOnTop++;
                }
            }
        }
    }



    /* if the list elements did not have changed, we only need to update the attributes */
    if(preListElemOnTop == pListCtrl->listElemOnTop)
    {
        pListCtrl->updateAttrOnly = BT_TRUE;
    }
    else
    {
        pListCtrl->updateAttrOnly = BT_FALSE;
    }
        
    pListCtrl->requestedElems = 0;
}


#else
void APP_SD_LC_vButtonUpDown(AppSdFrameDataListCommon *pListCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown, BooleanType rotatedList)
{
    Uint16Type visibleElems = 0;
    Uint16Type preListElemOnTop = pListCtrl->listElemOnTop;

    if( _APP_SD_LC_sfGetVisibleCells(pListCtrl->frameId, &visibleElems) != SUCCESS )
    {
        return;
    }


    if(visibleElems > pListCtrl->numListElems)
    {
        visibleElems = pListCtrl->numListElems;
    }

    /* reduce the visibleElems by 1 for our calculation */
    visibleElems = visibleElems - 1;
    
    if(upOrDown == enLogiButManMflUp)
    {
        if(pListCtrl->focusedListElem == 1)
        {
            /* we have an underrun, go to the last element */
            if(BT_TRUE == rotatedList)
            {
            	pListCtrl->listElemOnTop   = pListCtrl->numListElems - visibleElems;
            	pListCtrl->focusedListElem = pListCtrl->numListElems;
            }
            else
            {
            	// still do nothing, this is not an roll over list
            }
        }
        else
        {
            if(pListCtrl->listElemOnTop == pListCtrl->focusedListElem)
            {
                pListCtrl->listElemOnTop   = pListCtrl->listElemOnTop - 1;
            }
            
            pListCtrl->focusedListElem = pListCtrl->focusedListElem - 1;
        }
    }
    else
    {
        if(pListCtrl->focusedListElem == pListCtrl->numListElems)
        {
            /* we have an overrun, go to the first element */
            if(BT_TRUE == rotatedList)
            {
	            pListCtrl->listElemOnTop   = 1;
	            pListCtrl->focusedListElem = 1;
            }
            else
            {
            	// still do nothing, this is not an roll over list
            }
        }
        else
        {
            pListCtrl->focusedListElem = pListCtrl->focusedListElem + 1;

            if((pListCtrl->listElemOnTop + visibleElems) < pListCtrl->focusedListElem)
            {
                pListCtrl->listElemOnTop   = pListCtrl->listElemOnTop + 1;
            }
        }
    }

    /* if the list elements did not have changed, we only need to update the attributes */
    if(preListElemOnTop == pListCtrl->listElemOnTop)
    {
        pListCtrl->updateAttrOnly = BT_TRUE;
    }
    else
    {
        pListCtrl->updateAttrOnly = BT_FALSE;
    }
        
    pListCtrl->requestedElems = 0;
}
#endif


/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_sfSendListFrame                                     */
/** 
  \brief        Finish and send a list data frame 
 
  \brief        Description:\n
                This function is used to convert the internal list presentation
                to the KPB understandable format. It adds missing attributes and 
                parameters.

  \param        pFrameBuffer
                Pointer to KPB frame buffer

  \param        pListCtrl
                Current list control structure

  \param        pFrameData
                List data elements

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_SD_LC_sfSendListFrame( KpbFrameDataReqType *pFrameBuffer, 
                                           AppSdFrameDataListCommon *pListCtrl,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType activate )
{
    SuccessFailType result = SUCCESS;
    Uint16Type startPos = 0;
    Uint16Type neededElems = 0;
    Uint16Type visibleElems = 0;
    Uint16Type i = 0;

    if( _APP_SD_LC_sfGetStartCellIdx(pListCtrl->frameId, &startPos) != SUCCESS )
    {
        return FAIL;
    }

    if( _APP_SD_LC_sfGetVisibleCells(pListCtrl->frameId, &visibleElems) != SUCCESS )
    {
        return FAIL;
    }

    if(visibleElems > pListCtrl->numListElems)
    {
        neededElems = pListCtrl->numListElems;
    }
    else
    {
        neededElems = visibleElems;
    }

    pFrameBuffer->arrayHeader.elements = neededElems;

    
    if( pListCtrl->updateAttrOnly == BT_TRUE )
    {
        pFrameBuffer->arrayHeader.mode.common.recordAddress = KPB_FRAME_DATA_REC_ADDR_ATTR;
    }
    else
    {
        pFrameBuffer->arrayHeader.mode.common.recordAddress = KPB_FRAME_DATA_REC_ADDR_NAME |
                                                              KPB_FRAME_DATA_REC_ADDR_TYPE |
                                                              KPB_FRAME_DATA_REC_ADDR_ATTR;
    }


    for( i=0; i<neededElems; i++)                         
    { 
        Uint16Type cellIdx = i + startPos;
        pFrameBuffer->cell[cellIdx].isSet              = BT_TRUE;
        pFrameBuffer->cell[cellIdx].pos                = cellIdx;
        pFrameBuffer->cell[cellIdx].attribute.rawByte |= KPB_CELL_ATTRIBUTE_VISIBLE |
                                                         KPB_CELL_ATTRIBUTE_SELECTABLE;

        switch(gListBuffer[i].cell.type)
        {
            case KPB_CELL_TYPE_CHECK_BOX:
            case KPB_CELL_TYPE_RADIO_BUTTON:
                pFrameBuffer->cell[cellIdx].type = gListBuffer[i].cell.type;
                if(gListBuffer[i].cell.activated)
                {
                    pFrameBuffer->cell[cellIdx].attribute.rawByte |= KPB_CELL_ATTRIBUTE_ACTIVATED;
                }
                break;

            case KPB_CELL_TYPE_PHONE_NUMBER:
                pFrameBuffer->cell[cellIdx].type = KPB_CELL_TYPE_PHONE_NUMBER;
                break;
    
            default:
                pFrameBuffer->cell[cellIdx].type = KPB_CELL_TYPE_MENU_ITEM;
                break;
        }


        if( i == pListCtrl->focusedListElemOffset)   
        {
            pFrameBuffer->cell[cellIdx].attribute.rawByte |= KPB_CELL_ATTRIBUTE_FOCUSED;
        }

        (void)memcpy( &(pFrameBuffer->cell[cellIdx].name), 
                &gListBuffer[i].data, 
                sizeof(pFrameBuffer->cell[cellIdx].name));
    }

    if(neededElems < visibleElems)
    {
        for(i=neededElems; i<visibleElems; i++)
        {
            Uint16Type cellIdx = i + startPos;
            pFrameBuffer->cell[cellIdx].isSet = BT_TRUE;
            pFrameBuffer->cell[cellIdx].pos   = cellIdx;
            pFrameBuffer->cell[cellIdx].attribute.rawByte = 0;
        }
    }

    pListCtrl->requestedElems = 0;

    result = _APP_SD_FB_sfSendAndActivateFrame( pFrameBuffer, pFrameData, activate);

    if( pListCtrl->updateAttrOnly == BT_TRUE )
    {
        pListCtrl->updateAttrOnly = BT_FALSE;
    }

    //RT:5664 always update the slider position
    _APP_SD_FB_sfBuildAndSendScrollBar(pListCtrl);
    return result;
}



/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_u16GetSelectedIndex                                 */
/** 
  \brief        Returns the current item in focus  

  \param        pListCtrl
                Current list control structure

  \return       Uint16Type
                List index

 */
/*----------------------------------------------------------------------------*/
Uint16Type APP_SD_LC_u16GetSelectedIndex(const AppSdFrameDataListCommon *pListCtrl)
{
    Uint16Type selectedIndex = pListCtrl->listElemOnTop + pListCtrl->focusedListElemOffset;
    
    if(selectedIndex > pListCtrl->numListElems)
    {
        selectedIndex = selectedIndex - pListCtrl->numListElems;
    }

    return selectedIndex;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_LC_u16SetSelectedIndex                                 */
/** 
  \brief        Returns the current item in focus  

  \param        pListCtrl
                Current list control structure

  \return       SuccessFailType
                Success or Fail

 */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_SD_LC_sfSetSelectedIndex(AppSdFrameDataListCommon *pListCtrl, Uint16Type selIndex)
{
    Uint16Type visibleElems = 0;

    if(selIndex > pListCtrl->numListElems)
    {
        return FAIL;
    }

    if(_APP_SD_LC_sfGetVisibleCells(pListCtrl->frameId, &visibleElems) != SUCCESS)
    {
        return FAIL;
    }

    if( selIndex > pListCtrl->numListElems-visibleElems)
    {
        pListCtrl->listElemOnTop = pListCtrl->numListElems - visibleElems + 1;
    }
    else
    {
        pListCtrl->listElemOnTop = selIndex;
    }


    pListCtrl->focusedListElemOffset = 0;


    return SUCCESS;
} 


/* ---------------- Internal Member -------------------------*/

static SuccessFailType _APP_SD_LC_sfGetVisibleCells(KPB_enFrameIdType frameId, Uint16Type *pVisibleElems)
{
    switch(frameId)
    {
        case KPB_FRAME_ID_2:
            *pVisibleElems = KPB_u16GetVisListElements();
            if(*pVisibleElems > APP_SD_MAX_LIST_REQUEST_ELEMENTS)
            {
                DEBUG_VALUE2( APP_SD_SWI_ERR, MOD_APP_SD, "vGetVisibleCells: invalid num_cells %d (max: %d) for FrameId 2",
                              *pVisibleElems,
                              APP_SD_MAX_LIST_REQUEST_ELEMENTS);
                /* set to default APP_SD_MAX_LIST_REQUEST_ELEMENTS */ 
                *pVisibleElems = APP_SD_MAX_LIST_REQUEST_ELEMENTS;
            }
            break;
        case KPB_FRAME_ID_3:
            *pVisibleElems = 2;
            break;
        case KPB_FRAME_ID_4:
            *pVisibleElems = 1;
            break;
        case KPB_FRAME_ID_5:
            *pVisibleElems = 1;
            break;
        case KPB_FRAME_ID_6:
            *pVisibleElems = 1;
            break;
        default:
            return FAIL;
    }

    return SUCCESS;
}


static SuccessFailType _APP_SD_LC_sfGetStartCellIdx(KPB_enFrameIdType frameId, Uint16Type *pStartPos)
{
    switch(frameId)
    {
        case KPB_FRAME_ID_2:
            *pStartPos = 1;
            break;
        case KPB_FRAME_ID_3:
            *pStartPos = 2;
            break;
        case KPB_FRAME_ID_4:
            *pStartPos = 3;
            break;
        case KPB_FRAME_ID_5:
            *pStartPos = 7;
            break;
        case KPB_FRAME_ID_6:
            *pStartPos = 4;
            break;
        default:
            return FAIL;
    }

    return SUCCESS;
}

static void _APP_SD_FB_sfBuildAndSendScrollBar(const AppSdFrameDataListCommon *pListCtrl)
{
    KpbScrollBarReqType sb;
    Uint16Type visibleListElems = 0;
    Uint16Type lastListElem     = pListCtrl->numListElems;
    Uint16Type focusedListElem  = APP_SD_LC_u16GetSelectedIndex(pListCtrl);
    Uint16Type listElemOnTop    = pListCtrl->listElemOnTop;
    Uint16Type UsedlistElem;
    BooleanType validIndices    = BT_TRUE;

    if(_APP_SD_LC_sfGetVisibleCells(pListCtrl->frameId, &visibleListElems) != SUCCESS)
    {
        DEBUG_VALUE1( APP_SD_SWI_ERR, MOD_APP_SD, "sfBuildAndSendScrollBar: invalid frame id %d",
                      pListCtrl->frameId);
        return;
    }

    (void)memset(&sb, 0, sizeof(sb));

    sb.frameId = pListCtrl->frameId;

    /* make it 0 based indices and check for underrun */
    validIndices = --visibleListElems == (Uint16Type)-1 ? BT_FALSE : validIndices;
    validIndices = --lastListElem     == (Uint16Type)-1 ? BT_FALSE : validIndices;
    validIndices = --focusedListElem  == (Uint16Type)-1 ? BT_FALSE : validIndices;
    validIndices = --listElemOnTop    == (Uint16Type)-1 ? BT_FALSE : validIndices;

    if(((listElemOnTop == lastListElem)&&((focusedListElem == 0)||(focusedListElem == 1))) ||
      ((listElemOnTop == lastListElem - 1) && (focusedListElem == 0))) //RT:5664 use the focusedListElem for the scroll bar
        UsedlistElem = 0;
    else
        UsedlistElem = listElemOnTop;

    if(validIndices)
    {
        if( lastListElem != 0 && lastListElem > visibleListElems )
        {
            sb.sliderLength         = (Uint8Type)(100 * visibleListElems/lastListElem);
            sb.attribute.bit.arrows = 1;
        }
        else
        {
            sb.sliderLength         = 0;
            sb.sliderPosition       = 0;
            sb.attribute.bit.arrows = 0;
        }

        /* check if vertical ScrollBar must be shown */
        if(UsedlistElem == 0)
        {
            sb.sliderPosition   = 0;
        }
        else if((UsedlistElem + visibleListElems) == lastListElem)
        {
            sb.sliderPosition   = 100;
        }
        else
        {
            sb.sliderPosition   = (Uint8Type)(100 * (Uint32Type)(UsedlistElem+(visibleListElems/2))/lastListElem);
        }
    }
    else
    {
        /* send empty scrollbar in the case of invalid indices */
        sb.sliderLength         = 0;
        sb.sliderPosition       = 0;
        sb.attribute.bit.arrows = 0;
    }

    if((pListCtrl->scrollType == APP_SD_LIST_SCROLL_TYPE_ENDLESS) && (KPB_enGetKpbVersionNumber() == KPB_VERSION_1_0)) //RT:5729
    {
        sb.sliderPosition = 50;
    }

    _KPB_vBapSendScrollBar(&sb);
}


/* End Of File APP_SD_ListCtrl_C1.c */


