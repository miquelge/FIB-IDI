#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"
#include <math.h>


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event); 

    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform_p1 ();
    void modelTransform_p2 ();
    void modelTransform_Terra ();
    void projectTransform ();
    void viewTransform ();
    void ini_camera ();
    void info_model (const Model &m);
    void pinta_terra ();
    void pinta_patricio ();
    void pinta_lego ();



    // locations
    GLuint vertexLoc, colorLoc, transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VBO_Pos, VBO_Col, VAO, VBO_Pos_Terra, VBO_Col_Terra, VAO_Terra;
    GLuint VBO_Pos2, VBO_Col2, VAO2;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    
    Model m, m2;

    glm::vec3 pos, OBS, VRP, UP, punt_Min, punt_Max, punt_Centre, punt_Centre_Base;
    
    float dist = 10, angle = M_PI, sx = 0, sy = 0, zoom = 0;
    float FOV, ra, znear, zfar, radi, l, r, b, t;
    
    int rx,ry, click_x, click_y;

    bool DoingInteractive, patricio_mola = false;

    public slots:
        void actualitzar(bool b);


};

