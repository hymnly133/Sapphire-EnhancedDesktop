#include "screenfunc.h"
#include "SysFunctions.h"
#include "qapplication.h"
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

QPoint mapToLS(QWidget *aim, QPoint dis)
{

    return pls->mapFromGlobal(aim->mapToGlobal(dis));

}

void updateScreen()
{
    auto screens = QGuiApplication::screens();
    if(screenNum != screens.count())
    {
        Shift_Global = -pscs[0]->virtualGeometry().topLeft();
        pls->setFixedSize(pdt->size()*2);
        pls->move(0,0);
        if(screens.count() > screenNum)
        {
            qDebug() << u8"检测到扩展屏加入";
            for(int i=0;i<screenNum;i++){
                pmws[i]->setFixedSize(pscs[i]->availableSize());
                pmws[i]->move(pscs[i]->geometry().topLeft()+Shift_Global);
            }
            for(int i=screenNum;i<screens.count();i++){
                pscs[i] = screens[i];
                pmws[i] = new MainWindow(nullptr,i);
                if(UnusedJsons.contains(i)){
                    qDebug()<<"加载了"<<i<<"布局文件";
                    pmws[i]->load_json(UnusedJsons[i]);
                    UnusedJsons.remove(i);
                }
                else{
                    qDebug()<<"新建了"<<i<<"布局文件";
                    pmws[i]->Init();
                }
            }
        }else
        {
            qDebug() << u8"检测到扩展屏移除";
            for(int i=0;i<screens.count();i++){
                pmws[i]->setFixedSize(pscs[i]->availableSize());
                pmws[i]->move(pscs[i]->geometry().topLeft()+Shift_Global);
            }
            for(int i=screens.count();i<screenNum;i++){
                qDebug()<<"保存了"<<i<<"布局文件";
                UnusedJsons[i] = pmws[i]->to_json();
                pmws[i]->deleteLater();

                pscs.remove(i);
                pmws.remove(i);
            }
        }

        screenNum = screens.count();



    }

}
