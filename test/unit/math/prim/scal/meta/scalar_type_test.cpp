#include <stan/math/prim/arr.hpp>
#include <stan/math/prim/scal.hpp>
#include <gtest/gtest.h>
#include <test/unit/util.hpp>
#include <vector>

TEST(MetaTraitsPrimScal, ScalarTypeScal) {
  test::expect_same_type<double, stan::scalar_type<double>::type>();
  test::expect_same_type<int, stan::scalar_type<int>::type>();
}

TEST(MetaTraitsPrimArr, ScalarTypeArray) {
  using stan::scalar_type;
  using std::vector;

  test::expect_same_type<double, scalar_type<vector<double>>::type>();
  test::expect_same_type<int, scalar_type<vector<int>>::type>();
  test::expect_same_type<double, scalar_type<vector<vector<double>>>::type>();
}

TEST(MetaTraitsPrimArr, ScalarTypeArrayConst) {
  using stan::scalar_type;
  using std::vector;

  test::expect_same_type<double, scalar_type<const vector<double>>::type>();
  test::expect_same_type<int, scalar_type<const vector<int>>::type>();
  test::expect_same_type<double,
                         scalar_type<const vector<vector<double>>>::type>();
}

TEST(MetaTraitsPrimArr, ScalarTypeArrayConstConst) {
  using stan::scalar_type;
  using std::vector;

  test::expect_same_type<double const*,
                         scalar_type<const vector<double const*>>::type>();
  test::expect_same_type<int const*,
                         scalar_type<const vector<int const*>>::type>();
  test::expect_same_type<
      double const*, scalar_type<const vector<vector<double const*>>>::type>();
}
