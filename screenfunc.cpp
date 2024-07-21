#include "screenfunc.h"
#include "SysFunctions.h"
#include "qscreen.h"
QPoint Shift_Global;

int screenInd(QWidget *aim){
    return pdt->screenNumber (aim);
}

int cursorScreenInd(){
    for(int i=0;i<screenNum;i++){
        if(pscs[i]->geometry().contains(QCursor::pos ())){
            return i;
        }
    }
    return 0;
}
