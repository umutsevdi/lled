#pragma once
/******************************************************************************

 * File: /home/umutsevdi/src/dev/lled/src/ui/Dialogs.h
 *
 * Author: Umut Sevdi
 * Created: 08/04/24
 * Description: 

*****************************************************************************/

#include "common/Status.h"
namespace lled
{

/**
* Creates a pop-up with given message
* @r - Message to display
* @return Whether the button is pressed or not
*/
void popup_tooltip(Result& r, bool& show);
}// namespace lled
