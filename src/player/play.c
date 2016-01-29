#include "src\misc\definer.h"
/* this function is called immediately ***before*** Vertical Retrace starts */
void MIDAS_CALL PreVR(void){
	int i;
	frameCount++;
}
/* this function is called immediately ***after*** Vertical Retrace starts */
void MIDAS_CALL immVR(void){
}
/* this function is called some time later during Vertical Retrace */
void MIDAS_CALL inVR(void){

}
void initPlayer() {
    MIDASstartup();
    MIDASinit();
    MIDASsetTimerCallbacks(70000, FALSE, &PreVR, &immVR, NULL/*&inVR*/);
    module = MIDASloadModule("epic.mod");
    playHandle = MIDASplayModule(module, TRUE);
    MIDASstartBackgroundPlay(0);
}
void endPlayer() {
    MIDASstopModule(playHandle);
    MIDASfreeModule(module);
    MIDASstopBackgroundPlay();
    MIDASclose();
}