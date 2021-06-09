#include "WindowLibWPE.h"
#include <wpe/wpe.h>
#include <iostream>
#include "RNSKeyCodeMapping.h"
using namespace std;
rns_key_t  RnsShell::WindowLibWPE::keyIdentifierForWPEKeyCode(int keyCode)
{
    std::cout<<"RnsShell::WindowLibWPE::keyIdentifierForWPEKeyCode  "<< keyCode<<endl;
    switch (keyCode) {
    
    case WPE_KEY_Left:        return RNS_KEY_Left;
    case WPE_KEY_Right:       return RNS_KEY_Right;
    case WPE_KEY_Up:          return RNS_KEY_Up;
    case WPE_KEY_Down:        return RNS_KEY_Down;
    case WPE_KEY_Select:
    case WPE_KEY_ISO_Enter:
    case WPE_KEY_Return:      return RNS_KEY_Select;
    case WPE_KEY_PowerOff:    return RNS_KEY_PowerOff;
    case WPE_KEY_AudioMedia:  return RNS_KEY_AudioMedia;
    case WPE_KEY_Menu:        return RNS_KEY_Menu;
    case WPE_KEY_AudioLowerVolume: return RNS_KEY_AudioLowerVolume;
    case WPE_KEY_AudioRaiseVolume: return RNS_KEY_AudioRaiseVolume;
    case 61510:               return RNS_KEY_CHANNEL_Up;
    case 61511:               return RNS_KEY_CHANNEL_Down;
    case 61460:               return RNS_KEY_HELP;
    case WPE_KEY_Refresh:     return RNS_KEY_Refresh;
    case WPE_KEY_AudioStop:   return RNS_KEY_AudioStop;
    case WPE_KEY_AudioPlay:   return RNS_KEY_AudioPlay;
    case WPE_KEY_AudioPause:  return RNS_KEY_AudioPause;
    case WPE_KEY_Back:        return RNS_KEY_Back;
    case WPE_KEY_AudioNext:   return RNS_KEY_AudioNext;
    case WPE_KEY_AudioRewind: return RNS_KEY_AudioRewind;
    case WPE_KEY_AudioForward:return RNS_KEY_AudioForward;
    case WPE_KEY_AudioPrev:   return RNS_KEY_AudioPrev;
    case WPE_KEY_Green:       return RNS_KEY_Green;
    case WPE_KEY_Red:         return RNS_KEY_Red;
    case WPE_KEY_Blue:        return RNS_KEY_Blue;
    case WPE_KEY_Yellow:      return RNS_KEY_Yellow;
    case WPE_KEY_1:           return RNS_KEY_1;
    case WPE_KEY_2:           return RNS_KEY_2;
    case WPE_KEY_3:           return RNS_KEY_3;
    case WPE_KEY_4:           return RNS_KEY_4;
    case WPE_KEY_5:           return RNS_KEY_5;
    case WPE_KEY_6:           return RNS_KEY_6;
    case WPE_KEY_7:           return RNS_KEY_7;
    case WPE_KEY_8:           return RNS_KEY_8;
    case WPE_KEY_9:           return RNS_KEY_9;
    case WPE_KEY_0:           return RNS_KEY_0;
    case 61487:               return RNS_KEY_TV_RAD;
    case WPE_KEY_AudioRecord: return RNS_KEY_AUDIOREACORD;
    case 61495:               return RNS_KEY_AUD;
    case WPE_KEY_Cancel:      return RNS_KEY_Cancel;
    case WPE_KEY_AudioMute:   return RNS_KEY_Mute;
    case 61467:               return RNS_KEY_GUIA;
    default:                  return RNS_KEY_UNKNOWN;
    }
}
