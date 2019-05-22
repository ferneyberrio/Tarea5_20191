/***********************************************************************
************************************************************************
1. Creando simulaciones de Movimiento con Qt : Dibujando la particula
************************************************************************
************************************************************************/

//Primero se debe crear un objeto grafico que se va mover en la escena. Para esto se debe diseñar una clase que herede de la
//clase QGraphicsItem

/*****************************************************************************************************************************/
// Archivo de cabecera Particula.h
#ifndef PARTICULA_H
#define PARTICULA_H
 
#include <QGraphicsItem>
#include <QPainter>
 
class Particula : public QGraphicsItem
{
public:
    Particula(float x, float y, float velx, float vely, float r);
    ~Particula();
 
protected:
	float px;  //Variable para la posicion en x de la particula
	float py;  //Variable para la posicion en y de la particula
	float vx;  //Variable para la velocidad en x de la particula
	float vy;  //Variable para la velocidad en y de la particula
	float rad; //Variable para el radio de la particula
	
    //Estos metodos: boundingRect y paint se deben implementar, ya que son metodos virtuales puros en la clase QGraphicsItem
    QRectF boundingRect() const; //Este metodo debe implementarse para encerrar la figura en un rectangulo
     
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); //Se implementa para dar formato a la figura
};
 
#endif // Particula_H

/*****************************************************************************************************************************/
//Archivo fuente Particula.cpp
#include "Particula.h"
 
Particula::Particula(float x, float y, float velx, float vely, float r) : QGraphicsItem()  //Un constructor por defecto, que invoca al constructor de la clase base
{
	px = x;
	py = y;
	vx = velx;
	vy = vely;
	rad = r;	
}
 
Particula::~Particula()  //Un destructor por defecto
{
 
}
 
QRectF Particula::boundingRect() const  //Implementacion del metodo boundingRect
{
    return QRectF(-1*rad,-1*rad,2*rad,2*rad);   // Creando el area rectangular para delimitar la Particula (es una particula circular)
}
 
void Particula::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)    //Implementacion del metodo paint
{
        painter->setBrush(Qt::darkGreen);     // Color de relleno de la Particula
        painter->drawEllipse(boundingRect());  // Dibujando la Particula con el metodo drawEllipse y aprovechando las propiedades del rectangulo delimitador
}

/*****************************************************************************************************************************/
//Luego para adjuntar la figura (Particula) a la escena debe emplearse el metodo addItem

QGraphicsScene *escena = new QGraphicsScene();  //Reservando memoria para un puntero a un objeto escena de Qt
ui->graphicsView->setScene(escena);  //Montando la escena en el widget GraphicsView

Particula *particle = new Particula(10,30,5,5,15);  //Declarando un objeto grafico de la clase Particula

escena->addItem(particle); //Agregando el objeto grafico a la escena


/*****************************************************************************************************************************
******************************************************************************************************************************
2. Creando simulaciones de Movimiento con Qt : Agregando un metodo que actualiza la posicion de la particula
*******************************************************************************************************************************
*******************************************************************************************************************************/

//La posicion de la particula se actualiza ya sea a traves del calculo de una formula o de manera directa a traves de un incremento establecido.
//Para que el cambio de posicion se materialice es necesario invocar un metodo heredado de la clase QGraphicsItem, el metodo setPos,
//que recibe como parametros de entrada las nuevas coordenadas de la posicion en x, y

//Nota: el parametro "dt" es un "delta de tiempo" que controla cuantos pixeles avanza la particula en cada cuadro de escena 

void Particula::moverParticula(float dt, float vertlim)
{
	px += vx*dt;
	py += vy*dt;
	
	setPos(px, vertlim-py);
}

/*****************************************************************************************************************************
******************************************************************************************************************************
3. Creando simulaciones de Movimiento con Qt : Agregando un metodo que verifica que haya sucedido una colision
*******************************************************************************************************************************
*******************************************************************************************************************************/

//Se debe verificar si al modificar la posicion de la particula ocurre una colision con otro elemento grafico de la escena o con
//alguno de los limites horizontales y verticales de la escena (paredes, piso, techo.
//Por ejemplo: en el caso de una particula de forma circular se debe verificar si la distancia entre un limite de escena (o un elemento
//grafico) y las nuevas coordenadas de la particula es menor que el radio de la particula, entonces se evidencia que va a ocurrir una colision.
//Luego dependiendo de la orientacion de la colison se debe cambiar el signo del vector de velocidad para cambiar el sentido del movimiento de
//la particula (Ejemplo: si va hacia arriba y colisiona, se cambia el signo de la velocidad en "y" y la particula debe ir hacia abajo)

void Particula::colisionParticula(float horzlim, float vertlim)
{
	//Si la colision es con una barrera en sentido de avance horizontal (adelante o atras)
	if((px < rad) || (px > horzlim - rad))
		vx = -1*vx;
	
	//Si la colision es con una barrera en sentido de avance vertical (arriba o abajo)
	if((py < rad) || (py > vertlim - rad))
		vy = -1*vy;
}

/*****************************************************************************************************************************
******************************************************************************************************************************
4. Creando simulaciones de Movimiento con Qt : Agregando un metodo que actualiza la escena de la simulacion
*******************************************************************************************************************************
*******************************************************************************************************************************/

//Es un metodo que actualiza cada cuadro de escena del movimiento de la particula. Es decir, es un metodo que se ejecuta
//periodicamente y actualiza la posicion de la particula para el nuevo cuadro de escena y ademas verifica si hay una colision.
//Este metodo NO es de la clase Particula sino de la clase donde se declara el objeto de tipo particula y el objeto de tipo escena,
//por lo general el objeto MainWindow pero tambien puede ser un objeto de la clase QDialog o de la clase QWidget
//Este metodo debe ubicarse en la seccion de la clase que se denomina "private slots"
//La ejecucion de este metodo se puede "disparar" con ayuda de un timer (objeto de la clase QTimer) para que se ejecute automaticamente

void MainWindow::actualizarEscena(void)
{
	particula->moverParticula(dt, vertlim);
	particula->colisionParticula(horzlim, vertlim);
}


/*****************************************************************************************************************************
******************************************************************************************************************************
5. Creando simulaciones de Movimiento con Qt : Creando un temporizador para controlar los cuadros de animacion en la escena
*******************************************************************************************************************************
*******************************************************************************************************************************/

//Se declara un objeto de la clase QTimer, el constructor de esta clase recibe un puntero a si mismo (puntero this)

#include <QTimer>

QTimer *timer = new QTimer(this);

//Para que el timer comience a correr se debe inicializar con el metodo start que recibe como parametro de entrada la cantidad de
//milisegundos que se van a contar. Por lo general esta inicializacion se hace a traves de un boton que se presiona para dar inicio
//a la animacion o simulacion.

timer->start(50);  //Este timer cuenta 50 milisegundos, se reinicia automaticamente y cuenta otros 50 milisegundos y asi suecesivamente

//Previo al funcionamiento de la simulacion es preferible detener el timer con el metodo stop (es como un reset por si acaso)
//(en el constructor de la clase MainWindow)
timer->stop();

//Para actualizar el cuadro de escena de la simulacion se aprovecha una senial que emite el timer cada vez que alcanza la cuenta que se
//le configuro al metodo start.

//Es necesario crear de manera manual un "disparador de eventos" que ejecute automaticamente un metodo o funcion cada que el timer alcance la cuenta
//Para lograr ese cometido es necesario invocar una funcion de Qt llamada connect - Signal - Slot
//Esa funcion se invoca pasandole como parametros: 
// - el objeto que va emitir la senial, en este caso el timer
// - el nombre de la senial que se va a emitir, en este caso la senial se denomina timeout. Se debe hacer casting al tipo SIGNAL 
// - un puntero al objeto que contiene el metodo que va ser invocado luego de emitirse la senial timeout. Por lo general se pasa un puntero this
// - un metodo del objeto que se paso en el parametro anterior. Es el metodo que se va ejecutar cuando se emite la senial. Se debe hacer casting al tipo SLOT

//Este "disparador" se debe utilizar en el constructor de la clase MainWindow
connect(timer,SIGNAL(timeout()),this,SLOT(actualizarEscena()));









