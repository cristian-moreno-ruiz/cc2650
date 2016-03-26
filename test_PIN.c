///// #########    1    #########

#include <ti/drivers/PINCC26XX.h>


///// #########    2    #########



static PIN_Config SensortagAppPinTable[] =
{
    Board_LED1       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* LED initially off             */
    Board_KEY_RIGHT  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS,        /* Button is active low          */

    PIN_TERMINATE
};

static PIN_State sbpPins;
static PIN_Handle hSbpPins;






///// #########    3    #########

static void buttonHwiFxn(PIN_Handle hPin, PIN_Id pinId){
    
    events |= SBP_BTN_EVT;
    Semaphore_post(sem);
    
}



///// #########    4    #########




#define SBP_BTN_EVT 0x0010
    
    if(events & SBP_BTN_EVT){
        events &= ~SBP_BTN_EVT;
        
        if(LED_value){
            PIN_setOutputValue(hSbpPins, Board_LED1, LED_value--);
        } else{
            PIN_setOutputValue(hSbpPins, Board_LED1, LED_value++);
        }
    }
      

///// #########    5    #########


hSbpPins = PIN_open(&sbpPins, SBP_configTable);

PIN_registerIntCb(hSbpPins, buttonHwiFxn);

PIN_setConfig(hSbpPins, PIN_BM_IRQ, Board_KEY_UP | PIN_IRQ_NEGEDGE);

PIN_setConfig(hSbpPins, PINCC26XX_BM_WAKEUP, Board_KEY_UP | PINCC26XX_WAKEUP_NEGEDGE);






