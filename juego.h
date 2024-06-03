#ifndef JUEGO_H
#define JUEGO_H

#include "personaje.h"
#include "seleccionArma.h"
#include <QGraphicsView>
#include <QWidget>
#include <QVector2D>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QList>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QMessageBox>
#include <QDebug>
#include <QObject>

namespace Ui {
class Juego;
}

class Juego : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Juego(QWidget *parent = nullptr);
    ~Juego();

    //setters de dificultad
    int getNivelActual();
    void verificarPuntuacion();
    void establecerNivel(int nivel);

    //background
    void setBack(int nivel);
private slots:
    void crearEnemigo();
    void actualizarVida(int nuevaVida);
    void actualizarPuntuacion(int nuevaPts);
    void mostrarImagenMuerte();

    //subida de nivel (cambio de escena
    void mostrarSeleccionArma();
    void iniciarNivel();

private:
    Ui::Juego *ui;
    QGraphicsScene * escena;
    Personaje * personaje;
    SeleccionArma *seleccionarma;
    QTimer *tiempo;
    QTimer *enemigoTiempo;
    QGraphicsTextItem *vidaTexto;
    QGraphicsTextItem *nivelJuego;
    QGraphicsTextItem *ptsJugador;
    //imagen muerte
    QGraphicsPixmapItem *muerte;
    QPixmap *imagen;

    //datos para definir dificultad
    //int nivel;
    int puntuacionObjetivo;
    QGraphicsTextItem *mensajeNivel;
    int nivelActual;

    //metodos encapsulados
    void actualizarNivel(int tiempo);
    void limpiarNivel();
};

#endif // JUEGO_H
