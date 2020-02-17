#include "Application.h"

#include "BasicWidget.h"
#include "Lab5Widget.h"

Application::Application(QWidget* parent) : QMainWindow(parent)
{
  buildGui();
}

Application::~Application()
{}

void Application::buildGui()
{
  // A simple menubar.
  QMenuBar* menu = menuBar();
  QMenu* file = menu->addMenu("File");
  QAction* exit = file->addAction("Quit", [this]() {close();});

  // Our basic widget.
  // BasicWidget* widget = new BasicWidget(this);
  Lab5Widget* widget = new Lab5Widget(this);
  setCentralWidget(widget);
}
