#ifndef MYBTN_H
#define MYBTN_H

#include <QPushButton>

class mybtn : public QPushButton
{
    Q_OBJECT
    QString normalImgPath;
    QString pressImgpath;
public:
      mybtn(qreal portion1,qreal portion2,QString normalImag,QString pressImg="");
      void jump1();
      void jump2();
signals:

};

#endif // MYBTN_H
