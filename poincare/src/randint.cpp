#include <poincare/randint.h>
#include <poincare/complex.h>
#include <poincare/infinity.h>
#include <poincare/integer.h>
#include <poincare/layout_helper.h>
#include <poincare/random.h>
#include <poincare/rational.h>
#include <poincare/undefined.h>
#include <poincare/serialization_helper.h>
#include <ion.h>

extern "C" {
#include <assert.h>
}
#include <cmath>

namespace Poincare {

constexpr Expression::FunctionHelper Randint::s_functionHelper;

int RandintNode::numberOfChildren() const { return Randint::s_functionHelper.numberOfChildren(); }

Layout RandintNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return LayoutHelper::Prefix(Randint(this), floatDisplayMode, numberOfSignificantDigits, Randint::s_functionHelper.name());
}

int RandintNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, Randint::s_functionHelper.name());
}

template <typename T> Evaluation<T> RandintNode::templateApproximate(Context * context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit, bool * inputIsUndefined) const {
  Evaluation<T> aInput = childAtIndex(0)->approximate(T(), context, complexFormat, angleUnit);
  Evaluation<T> bInput = childAtIndex(1)->approximate(T(), context, complexFormat, angleUnit);
  if (inputIsUndefined) {
    *inputIsUndefined = aInput.isUndefined() || bInput.isUndefined();
  }
  T a = aInput.toScalar();
  T b = bInput.toScalar();
  if (std::isnan(a) || std::isnan(b) || a != std::round(a) || b != std::round(b) || a > b || std::isinf(a) || std::isinf(b)) {
    return Complex<T>::Undefined();
  }
  T result = std::floor(Random::random<T>()*(b+1.0-a)+a);
  return Complex<T>::Builder(result);
}

Expression RandintNode::shallowReduce(ReductionContext reductionContext) {
  return Randint(this).shallowReduce(reductionContext);
}

Expression Randint::shallowReduce(ExpressionNode::ReductionContext reductionContext) {
  Expression e = Expression::defaultShallowReduce();
  if (e.isUndefined()) {
    return e;
  }
  bool inputIsUndefined = false;
  double eval = static_cast<RandintNode *>(node())->templateApproximate<double>(reductionContext.context(), reductionContext.complexFormat(), reductionContext.angleUnit(), &inputIsUndefined).toScalar();
  if (inputIsUndefined) {
    /* The input might be NAN because we are reducing a function's expression
     * which depends on x. We thus do not want to replace too early with
     * undefined. */
    return *this;
  }
  Expression result = Number::DecimalNumber(eval);
  replaceWithInPlace(result);
  return result.shallowReduce(reductionContext);
}

}
