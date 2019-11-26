#include "arrow_view.h"

namespace HardwareTest {

const uint8_t arrowUpMask[10][9] = {
  {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},
  {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
  {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

const uint8_t arrowDownMask[10][9] = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
  {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
  {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},
};

ArrowView::ArrowView() :
  m_directionIsUp(true),
  m_color(Palette::Text)
{
}

void ArrowView::setDirection(bool isUp) {
  if (m_directionIsUp!= isUp) {
    m_directionIsUp = isUp;
    markRectAsDirty(bounds());
  }
}

void ArrowView::setColor(KDColor color) {
  if (m_color != color) {
    m_color = color;
    markRectAsDirty(bounds());
  }
}

KDColor s_arrowWorkingBuffer[10*9];

void ArrowView::drawRect(KDContext * ctx, KDRect rect) const {
  ctx->fillRect(bounds(), Palette::BackgroundHard);
  KDCoordinate startLine = m_directionIsUp ? k_arrowHeight : 0;
  KDCoordinate startArrow = m_directionIsUp ? 0 : bounds().height()-k_arrowHeight;
  ctx->fillRect(KDRect((Ion::Display::Width-k_arrowThickness)/2, startLine, k_arrowThickness, bounds().height()-k_arrowHeight), m_color);
  KDRect frame((Ion::Display::Width-k_arrowWidth)/2, startArrow, k_arrowWidth, k_arrowHeight);
  const uint8_t * mask = m_directionIsUp ? (const uint8_t *)arrowUpMask : (const uint8_t *)arrowDownMask;
  ctx->blendRectWithMask(frame, m_color, mask, s_arrowWorkingBuffer);
}

}

