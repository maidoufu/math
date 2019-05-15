#ifndef STAN_MATH_PRIM_SCAL_FUN_FMIN_HPP
#define STAN_MATH_PRIM_SCAL_FUN_FMIN_HPP

#include <boost/math/tools/promotion.hpp>

namespace stan {
namespace math {

/**
 * Return the lesser of the two specified arguments.  If one is
 * greater than the other, return not-a-number.
 *
 * @param x First argument.
 * @param y Second argument.
 * @return Minimum of x or y and if one is NaN return the other
 */
template <typename T1, typename T2>
inline typename boost::math::tools::promote_args<T1, T2>::type fmin(
    const T1& x, const T2& y) {
  using std::fmin;
  return fmin(x, y);
}

}  // namespace math
}  // namespace stan
#endif
