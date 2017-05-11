#include "logo_controller.h"
#include "logo_icon.h"
#include "../apps_container.h"

namespace OnBoarding {

LogoController::LogoController() :
  ViewController(nullptr),
  Timer(10),
  m_logoView()
{
}

View * LogoController::view() {
  return &m_logoView;
}

void LogoController::fire() {
  app()->dismissModalViewController();
  AppsContainer * appsContainer = (AppsContainer *)app()->container();
  appsContainer->windowRedraw();
}

}
