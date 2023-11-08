//#include <glm/glew.h>
#include "MyGLWidget.h"
#include <iostream>
#include <cmath>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent) {
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget () {
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL () {
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  glEnable (GL_DEPTH_TEST);  

  OBS = glm::vec3(0,0,5);
  VRP = glm::vec3(0,0,0);
  UP = glm::vec3(0,1,0);
  //FOV = M_PI/2;
  //FOV = 2*float(atan((1-0.5)/5));
  FOV = 1.2;
  ra = 1.0;

  l = -2.f;
  r = 2.f;
  b = -2.f;
  t = 2.f;
  znear = 0.1;
  zfar = 15.0;
  projectTransform();
  viewTransform();
}

void MyGLWidget::info_model(const Model &m) {
  double min_x, min_y, min_z, max_x, max_y, max_z; 
  min_x = m.vertices()[0];
  min_y = m.vertices()[1];
  min_z = m.vertices()[2];
  max_x = m.vertices()[0];
  max_y = m.vertices()[1];
  max_z = m.vertices()[2];
  for(unsigned int i = 3;i<m.vertices().size();i+=3) {
    double x = m.vertices()[i];
    double y = m.vertices()[i+1];
    double z = m.vertices()[i+2];
    if(x<min_x) min_x = x;
    else if (x>max_x) max_x = x;
    if(y<min_y) min_y = y;
    else if (y>max_y) max_y = y;
    if(z<min_z) min_z = z;
    else if (z>max_z) max_z = z;
  }
  punt_Min = glm::vec3(min_x, min_y, min_z);
  punt_Max = glm::vec3(max_x, max_y, max_z);
  punt_Centre = glm::vec3((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);
  punt_Centre_Base = glm::vec3((min_x+max_x)/2, min_y, (min_z+max_z)/2);
  radi = sqrt(pow(punt_Max.x-punt_Centre.x, 2) + pow(punt_Max.y-punt_Centre.y, 2) + pow(punt_Max.z-punt_Centre.z, 2));
  /*
  std::cout << "punt_Min: "<<punt_Min.x<<", "<<punt_Min.y<<", "<<punt_Min.z<<std::endl;
  std::cout << "punt_Max: "<<punt_Max.x<<", "<<punt_Max.y<<", "<<punt_Max.z<<std::endl;
  std::cout << "punt_Centre: "<<punt_Centre.x<<", "<<punt_Centre.y<<", "<<punt_Centre.z<<std::endl;
  std::cout << "punt_Centre_Base: "<<punt_Centre_Base.x<<", "<<punt_Centre_Base.y<<", "<<punt_Centre_Base.z<<std::endl;
  std::cout << "radi: "<<radi<<std::endl;
  */
}

void MyGLWidget::paintGL () {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  pinta_patricio();
  pinta_terra();
}

void MyGLWidget::pinta_patricio () {
  info_model(m);
  glBindVertexArray (VAO);

  modelTransform_p1();
  glDrawArrays(GL_TRIANGLES, 0, m.faces ().size () * 3); // pintem
  modelTransform_p2();
  glDrawArrays(GL_TRIANGLES, 0, m.faces ().size () * 3); // pintem
}

void MyGLWidget::pinta_terra () {
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void MyGLWidget::resizeGL (int w, int h) {
  if (w/h >= 1) {
  	l = -2 - (4*w/h -4)/2;
  	r = 2 + (4*w/h -4)/2;
  	b = -2;
  	t = 2;
  }
  else if (w/h < 1) {
  	l = -2;
  	r = 2;
  	b = -2 - (4*h/w -4)/2;
  	t = 2 + (4*h/w -4)/2;
  }
  projectTransform();
  update();
  glViewport(0, 0, w, h);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Z: {
      zoom -= 0.05;
  	  //std::cout << "FOV: "<<FOV<<", zoom: "<<zoom<<std::endl;
      projectTransform();
      update();
      break;
    }
    case Qt::Key_X: {
      zoom += 0.05;
      //std::cout << "FOV: "<<FOV<<", zoom"<<zoom<<std::endl;
      projectTransform();
      update();
      break;
    }
    default: {
      event->ignore();
      break;
  	}
  }
}

void MyGLWidget::mousePressEvent (QMouseEvent *e) {
  DoingInteractive = true;
  click_x = e->x();
  click_y = e->y();
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *) {
  DoingInteractive = false;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
  makeCurrent();
  if (DoingInteractive) {
    sy += (e->x() - click_x) * M_PI / 180.0;
    sx += (e->y() - click_y) * M_PI / 180.0;
    viewTransform ();
  }
  click_x = e->x();
  click_y = e->y();
  update();
}

void MyGLWidget::createBuffers () {
  m.load("/Users/Miquelge/Desktop/IDI/uni/models/Patricio.obj");
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO_Pos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces ().size () * 3 * 3, m.VBO_vertices (), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Col);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Col);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces ().size () * 3 * 3, m.VBO_matdiff (), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glm::vec3 posterra[4] = {
        glm::vec3(-2.0, 0, 2.0),
        glm::vec3(2.0, 0, 2.0),
        glm::vec3(-2.0, 0, -2.0),
        glm::vec3(2.0, 0, -2.0)
  };
  glm::vec3 colterra[4] = {
        glm::vec3(1.,1.,0.),
        glm::vec3(1.,1.,0.),
        glm::vec3(1.,1.,0.),
        glm::vec3(1.,1.,0.),
  };

  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  glGenBuffers(1, &VBO_Pos_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos_Terra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Col_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Col_Terra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders () {
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("./shaders/fragshad.frag");
  vs.compileSourceFile("./shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  vertexLoc = glGetAttribLocation(program->programId(), "vertex");
  colorLoc = glGetAttribLocation(program->programId(), "color");
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

void MyGLWidget::modelTransform_p2 () {
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(1, 0, 1));
  float mida = punt_Max.y-punt_Min.y;
  transform = glm::scale(transform, glm::vec3(1/mida));

  transform = glm::translate(transform, punt_Centre_Base*-1.0f);
  //transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform_p1 () {
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-1, 0, -1));
  float mida = punt_Max.y-punt_Min.y;
  transform = glm::scale(transform, glm::vec3(1/mida));
  transform = glm::rotate(transform, float(M_PI), glm::vec3(0,1,0));
  transform = glm::translate(transform, punt_Centre_Base*-1.0f);
  //transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform () {
  //Sabem que punt max escena = (2, 1, 2)
  //i punt min escena = (-2, 0, -2)
  //glm::mat4 Proj = glm::perspective (FOV+zoom, ra, znear, zfar);
  glm::mat4 Proj = glm::ortho (l, r, b, t, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
  //glm::mat4 view = glm::lookAt (OBS, VRP, UP);
  
  glm::mat4 view (1.0f);
  view = glm::translate(view, glm::vec3(0, 0,-10));
  view = glm::rotate(view, sx, glm::vec3(1,0,0));
  view = glm::rotate(view, sy, glm::vec3(0,1,0));
  
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &view[0][0]);
}
