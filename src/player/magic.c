#include "src\misc\definer.h"

/*displays most recent MIDAS error and exits to DOS*/
void MIDASerror() {
    set_mode(TEXT_MODE); 
    printf("MIDAS error: %s\n", MIDASgetErrorMessage(MIDASgetLastError()));
#ifndef NOMUSIC
    MIDASclose(); /*!!!!!*/
#endif
    exit(EXIT_FAILURE);
}
void MIDAS_CALL SyncCallback(unsigned syncNum, unsigned position, unsigned row) {
    /* Prevent warnings: */
    position = position;
    row = row;

    /* Check if the infobyte is interesting - do something only when command
       "W42" is encountered: */
    if ( syncNum == 0x42 ) {
        /* Yeah, yeah, flash the border! */
        border = 15;
        /* The timer will set the border color */
    }
}
/*called immediately before Vertical Retrace starts*/
void MIDAS_CALL prevr(void) {
    frameCount++;

    if ( border >= 0 )
    {
        vgaSetBorder(11);
        border -= 15;
    }
}

void midasMagicStart() {
#ifndef NOMUSIC
    MIDASstartup();
    //try detecting sound card, if no card detected run config

#ifdef ATTEMPT_DETECT
    if ( (argc > 1) || (!MIDASdetectSoundCard()) )
#endif
    {
        if ( !MIDASconfig() ) {
            if ( MIDASgetLastError() ){
                MIDASerror();
            }
            else
            {
                printf("EXITED!\n");
                return;
            }
        }
    }
#endif /* #ifndef NOMUSIC */    

#ifndef NOMUSIC
    refreshRate = MIDASgetDisplayRefreshRate();

    if ( refreshRate == 0 ) {
        printf("RUN IN DOS\n");
        if ( getch() == 27 )
            return;
        refreshRate = 70000; /* default 70Hz */
    }

    if ( !MIDASinit() )
        MIDASerror();

#endif


#ifndef NOMUSIC
    if ( !MIDASsetTimerCallbacks(refreshRate, TRUE, &prevr, NULL, NULL) )
        MIDASerror();

    if ( !MIDASsetMusicSyncCallback(playHandle, &SyncCallback) )
        MIDASerror();
#endif
}

void midasMagicDies() {
#ifndef NOMUSIC
    if ( !MIDASsetMusicSyncCallback(playHandle, NULL) )
        MIDASerror();
    if ( !MIDASstopModule(playHandle) )
        MIDASerror();
    if ( !MIDASfreeModule(module) )
        MIDASerror();
    if ( !MIDASremoveTimerCallbacks() )
        MIDASerror();
    if ( !MIDASclose() )
        MIDASerror();
#endif
}