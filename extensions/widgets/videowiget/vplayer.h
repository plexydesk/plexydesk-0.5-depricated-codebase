
#ifndef V_PLAYER_H
#define V_PLAYER_H

#include <QtCore>
#include <QtGui>
#include <plexy.h>


namespace PlexyDesk
{
class VISIBLE_SYM VPlayer : public QObject
{
Q_OBJECT
    public :
        VPlayer(QObject *parent= 0);
        ~VPlayer();

        void setFileName(const QString& Path);
	
//        void fileName();
	
public slots:
	//void pull() {}
	void decode();
signals:
	void videoDone();
	void frameReady(QImage & img);
	
    protected:
        void init();
	      

    private:
        class Private;
            Private * const d;
};

}
#endif
