/**
 * The primary application code.
 */

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

class App : public QMainWindow
{
  Q_OBJECT

public:
  App(QWidget* parent=0);
  virtual ~App();
  std::string objFile;
  std::string texFile;
  
signals:

public slots:

private:
  void buildGui();
};
