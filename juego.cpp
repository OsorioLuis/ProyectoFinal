#include "juego.h"
#include "casa.h"
#include "ui_juego.h"
#include "enemigo.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QAudioOutput>

Juego::Juego(QWidget *parent)
    : QGraphicsView(parent)
    , ui(new Ui::Juego)
    , puntuacionObjetivo(10)
{
    ui->setupUi(this);
    escena = new QGraphicsScene();
    setBack(1);

    //configuracion juego
    setFixedSize(1600, 900);
    setSceneRect(0,0, 1600, 900);
    //setScene(escena);

    ui->graphicsView->setScene(escena);
    ui->graphicsView->setFixedSize(1600, 900);
    ui->graphicsView->setSceneRect(0,0, 1600, 900);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    establecerNivel(1);
    //configuracion de musica de fondo
    backsound = new QMediaPlayer(this);
    audioOut = new QAudioOutput(this);

    backsound->setAudioOutput(audioOut);
    backsound->setSource(QUrl("qrc:/efectos/sonidos/background.mp3"));
    backsound->setLoops(QMediaPlayer::Infinite);
    audioOut->setVolume(0.5);
    backsound->play();

    //imagen muerte
    muerte = new QGraphicsPixmapItem;
    QPixmap imagen(":/Imagenes/videoJuego/muerte.png");
    muerte->setPixmap(imagen);
    muerte->setPos(350,400);
    muerte->setVisible(false);
    escena->addItem(muerte);

    //creacion de elementos en escena:
    personaje = new Personaje();
    //personaje->setPixmap(normal);
    personaje->setPos(width() / 2, height() / 2); //posicion en el centro

    //creacion textos
    vidaTexto = new QGraphicsTextItem();
    nivelJuego = new QGraphicsTextItem();
    objetivo = new QGraphicsTextItem();

    //puntuacion objetivo
    objetivo->setPlainText(QString("Objetivo: %1").arg(puntuacionObjetivo));


    objetivo->setDefaultTextColor(Qt::white);
    objetivo->setFont(QFont("times", 24));
    objetivo->setPos(700, 820);

    //nivel
    nivelJuego->setPlainText(QString("Nivel: %1").arg(nivelActual));
    nivelJuego->setDefaultTextColor(Qt::white);
    nivelJuego->setFont(QFont("times", 24));
    nivelJuego->setPos(742, 60);

    //vida
    vidaTexto->setPlainText(QString("Vida:%1 | ").arg(personaje->getVida()));
    vidaTexto->setDefaultTextColor(Qt::white);
    vidaTexto->setFont(QFont("times", 15, 1, true));
    vidaTexto->setPos(70, 55);

    //puntuacion
    ptsJugador = new QGraphicsTextItem();
    ptsJugador->setPlainText(QString("Eliminados: %1").arg(personaje->getPuntacion()));
    ptsJugador->setDefaultTextColor(Qt::white);
    ptsJugador->setFont(QFont("times", 15, 1, true));
    ptsJugador->setPos(160, 55);

    //mensaje puntuacion objetivo lograda

    escena->addItem(ptsJugador);
    escena->addItem(nivelJuego);
    escena->addItem(vidaTexto);
    escena->addItem(objetivo);

    //slots para cambio de vida
    //hacemos la conexion entre la señal vidaCambiada y actualizarVida
    connect(personaje, &Personaje::vidaCambiada, this, &Juego::actualizarVida);
    connect(personaje, &Personaje::cambioPuntuacion, this, &Juego::actualizarPuntuacion);


    //player focusable
    personaje->setFlag(QGraphicsItem::ItemIsFocusable);
    personaje->setFocus();
    //añadimos al jugador en la escena
    escena->addItem(personaje);

    //añadir enemigos
    /*for(int i = 0; i < 10; i++){
        Enemigo *enemigo = new Enemigo(personaje);
        escena->addItem(enemigo);
    }*/

    //layout ajustado a mainwindow
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *container = new QWidget();
    container->setLayout(layout);

    //establecer niveles
    //nivel inicial
    enemigoTiempo = new QTimer(this);
    connect(enemigoTiempo, &QTimer::timeout, this, &Juego::crearEnemigo);

    connect(personaje, &Personaje::muerte, this, &Juego::mostrarImagenMuerte);
    actualizarNivel(1000);

    //seleccion de arma
    //el evento que muestra los nuevos niveles es cuando se presiona el boton
    seleccionarma = new SeleccionArma();
    connect(seleccionarma, &SeleccionArma::iniciarNivel, this, &Juego::iniciarNivel);

}

Juego::~Juego()
{
    delete ui;
}


void Juego::verificarPuntuacion(){
    if (personaje->getPuntacion() >= puntuacionObjetivo) {
        nivelActual++;
        puntuacionObjetivo *= 5;
        objetivo->setPlainText(QString("Objetivo: %1").arg(puntuacionObjetivo));
        nivelJuego->setPlainText(QString("Nivel: %1").arg(nivelActual));

        // mensajeNivel->setPlainText(QString("Has pasado al nivel %1").arg(nivelActual));
        // mensajeNivel->setVisible(true);

        // //temporizador para ocultar mensaje luego de 2 segundos
        // QTimer::singleShot(3000, [this](){
        //     mensajeNivel->setVisible(false);
        // });

        //ventana para la seleccion de arma
        mostrarSeleccionArma();        

    }
}

void Juego::establecerNivel(int nivel){
    nivelActual = nivel;
}

void Juego::setBack(int nivel){
    if(nivel == 1){
        escena->setSceneRect(0,0, 1600, 900);
        escena->setBackgroundBrush(QBrush(QImage(":/Imagenes/videoJuego/back.png")));
    }else if(nivel == 2){
        escena->setSceneRect(0,0, 1600, 900);
        escena->setBackgroundBrush(QBrush(QImage(":/Imagenes/videoJuego/back2.png")));
    }else if(nivel == 3){
        escena->setSceneRect(0,0, 1600, 900);
        escena->setBackgroundBrush(QBrush(QImage(":/Imagenes/videoJuego/back3.png")));
    }


}

void Juego::agregarCasas(int cantidad){
    for(int i = 0; i < cantidad; i++){
        int posx = QRandomGenerator::global()->bounded(1600);
        int posy = QRandomGenerator::global()->bounded(900);

        Casa *casa = new Casa();
        if(posx >= 800 && posy >= 450){
            casa->setPos(posx / 2, posy / 2);
            escena->addItem(casa);
        }

        //añadimos la casa a la lista, de las casas que serán dañadas
        casas.append(casa);
    }
}

void Juego::actualizarPuntuacion(int nuevaPts){
    ptsJugador->setPlainText(QString("Eliminados: %1").arg(nuevaPts));
    verificarPuntuacion();
}

void Juego::actualizarVida(int nuevaVida){
    vidaTexto->setPlainText(QString("Vida:%1 | ").arg(nuevaVida));

}

void Juego::mostrarImagenMuerte(){
    muerte->setVisible(true);
    enemigoTiempo->stop();

    setFocusPolicy(Qt::NoFocus);
    //aqui irá el retorno al menu del juego
}

void Juego::mostrarSeleccionArma(){
    //con esto detenemos la generacion de enemigos
    enemigoTiempo->stop();
    limpiarNivel();

    //para actualizar la vida y mostrar la nueva
    personaje->setVida();
    personaje->setPos(width() / 2, height() / 2); //posicion en el centro
    vidaTexto->setPlainText(QString("Vida:%1 | ").arg(personaje->getVida()));

    //mostramos ventana
    seleccionarma->show();

}

void Juego::iniciarNivel(){
    if(nivelActual == 2){
        seleccionarma->hide();
        actualizarNivel(700);
        setBack(nivelActual);
        //ahora ponemos el orbital al jugador

        personaje->iniciarOrbital();

    }else if(nivelActual == 3){
        //añadimos casas y orbital
        personaje->iniciarOrbital();
        agregarCasas(10);

        seleccionarma->hide();
        actualizarNivel(600);
        setBack(nivelActual);
    }
}

void Juego::actualizarNivel(int time){
    // se define el tiempo del enemigo
    enemigoTiempo->stop();
    limpiarNivel();
    //cambio de images y otros metodos....
    enemigoTiempo->start(time); //cambiando este dato podemos definir dificultad
}

void Juego::limpiarNivel(){
    for (auto item : escena->items()) {
        Enemigo *enemigo = dynamic_cast<Enemigo*>(item);
        if (enemigo) {
            escena->removeItem(enemigo);
            delete enemigo;
        }
    }
}


void Juego::crearEnemigo(){
    //generacion aleatoria de enemigo
    int randomX = rand() % 1600;
    int randomY= rand() % 900;

    //añadir enemigo a escena
    Enemigo *enemigo = new Enemigo(nivelActual, personaje);
    enemigo->setPos(randomX, randomY);
    escena->addItem(enemigo);
}
