#include <stan/math/prim.hpp>
#include <test/unit/math/test_ad.hpp>

TEST(primScalFun, log10) {
  stan::test::expect_common_prim([](auto x) { return std::log10(x); },
                                 [](auto x) { return stan::math::log10(x); });
}

TEST(MathFunctions, log10_works_with_other_functions) {
  Eigen::VectorXd a(5);
  a << 1.1, 1.2, 1.3, 1.4, 1.5;
  Eigen::RowVectorXd b(5);
  b << 1.1, 1.2, 1.3, 1.4, 1.5;
  stan::math::multiply(a, stan::math::log10(b));
}
