#ifndef ORBITAL_H
#define ORBITAL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "Personaje.h"

class Orbital : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Orbital(Personaje *personaje, QObject *parent = nullptr);
    //le pasamos como constructor donde pertenece
public slots:
    void mover(); //determina el movimiento, la misma clase lo hace

private:
    Personaje *personaje;
    float angulo;
    float radio;
};


#endif // ORBITAL_H
