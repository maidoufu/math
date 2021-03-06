#ifndef STAN_MATH_REV_FUN_EIGEN_NUMTRAITS_HPP
#define STAN_MATH_REV_FUN_EIGEN_NUMTRAITS_HPP

#include <stan/math/rev/meta.hpp>
#include <stan/math/rev/core.hpp>
#include <stan/math/rev/core/std_numeric_limits.hpp>
#include <stan/math/prim/fun/Eigen.hpp>
#include <limits>

namespace Eigen {

/**
 * Numerical traits template override for Eigen for automatic
 * gradient variables.
 *
 * Documentation here:
 *   http://eigen.tuxfamily.org/dox/structEigen_1_1NumTraits.html
 */
template <>
struct NumTraits<stan::math::var> : GenericNumTraits<stan::math::var> {
  using Real = stan::math::var;
  using NonInteger = stan::math::var;
  using Nested = stan::math::var;

  /**
   * Return the precision for <code>stan::math::var</code> delegates
   * to precision for <code>douboe</code>.
   *
   * @return precision
   */
  static inline stan::math::var dummy_precision() {
    return NumTraits<double>::dummy_precision();
  }

  enum {
    /**
     * stan::math::var is not complex.
     */
    IsComplex = 0,

    /**
     * stan::math::var is not an integer.
     */
    IsInteger = 0,

    /**
     * stan::math::var is signed.
     */
    IsSigned = 1,

    /**
     * stan::math::var does not require initialization.
     */
    RequireInitialization = 0,

    /**
     * Twice the cost of copying a double.
     */
    ReadCost = 2 * NumTraits<double>::ReadCost,

    /**
     * This is just forward cost, but it's the cost of a single
     * addition (plus memory overhead) in the forward direction.
     */
    AddCost = NumTraits<double>::AddCost,

    /**
     * Multiply cost is single multiply going forward, but there's
     * also memory allocation cost.
     */
    MulCost = NumTraits<double>::MulCost
  };

  /**
   * Return the number of decimal digits that can be represented
   * without change.  Delegates to
   * <code>std::numeric_limits<double>::digits10()</code>.
   */
  static int digits10() { return std::numeric_limits<double>::digits10; }
};

/**
 * Scalar product traits specialization for Eigen for reverse-mode
 * autodiff variables.
 */
template <typename BinaryOp>
struct ScalarBinaryOpTraits<stan::math::var, double, BinaryOp> {
  using ReturnType = stan::math::var;
};

/**
 * Scalar product traits specialization for Eigen for reverse-mode
 * autodiff variables.
 */
template <typename BinaryOp>
struct ScalarBinaryOpTraits<double, stan::math::var, BinaryOp> {
  using ReturnType = stan::math::var;
};

namespace internal {
/**
 * Partial specialization of Eigen's remove_all struct to stop
 * Eigen removing pointer from vari* variables
 */
template <>
struct remove_all<stan::math::vari*> {
  using type = stan::math::vari*;
};

/**
 * Specialization of matrix-vector products for reverse-mode
 * autodiff variables.
 *
 * @tparam Index index type
 * @tparam LhsMapper left-hand side data and stride
 * @tparam CongjuageLhs left-hand side conjugacy flag
 * @tparam CongjuageRhs right-hand side conjugacy flag
 * @tparam RhsMapper right-hand side data and stride
 * @tparam Version integer version number
 */
template <typename Index, typename LhsMapper, bool ConjugateLhs,
          bool ConjugateRhs, typename RhsMapper, int Version>
struct general_matrix_vector_product<Index, stan::math::var, LhsMapper,
                                     ColMajor, ConjugateLhs, stan::math::var,
                                     RhsMapper, ConjugateRhs, Version> {
  using LhsScalar = stan::math::var;
  using RhsScalar = stan::math::var;
  using ResScalar = stan::math::var;
  enum { LhsStorageOrder = ColMajor };

  EIGEN_DONT_INLINE static void run(Index rows, Index cols,
                                    const LhsMapper& lhsMapper,
                                    const RhsMapper& rhsMapper, ResScalar* res,
                                    Index resIncr, const ResScalar& alpha) {
    const LhsScalar* lhs = lhsMapper.data();
    const Index lhsStride = lhsMapper.stride();
    const RhsScalar* rhs = rhsMapper.data();
    const Index rhsIncr = rhsMapper.stride();
    run(rows, cols, lhs, lhsStride, rhs, rhsIncr, res, resIncr, alpha);
  }

  EIGEN_DONT_INLINE static void run(Index rows, Index cols,
                                    const LhsScalar* lhs, Index lhsStride,
                                    const RhsScalar* rhs, Index rhsIncr,
                                    ResScalar* res, Index resIncr,
                                    const ResScalar& alpha) {
    using stan::math::gevv_vvv_vari;
    using stan::math::var;
    for (Index i = 0; i < rows; ++i) {
      res[i * resIncr] += var(
          new gevv_vvv_vari(&alpha, &lhs[i], lhsStride, rhs, rhsIncr, cols));
    }
  }
};

template <typename Index, typename LhsMapper, bool ConjugateLhs,
          bool ConjugateRhs, typename RhsMapper, int Version>
struct general_matrix_vector_product<Index, stan::math::var, LhsMapper,
                                     RowMajor, ConjugateLhs, stan::math::var,
                                     RhsMapper, ConjugateRhs, Version> {
  using LhsScalar = stan::math::var;
  using RhsScalar = stan::math::var;
  using ResScalar = stan::math::var;
  enum { LhsStorageOrder = RowMajor };

  EIGEN_DONT_INLINE static void run(Index rows, Index cols,
                                    const LhsMapper& lhsMapper,
                                    const RhsMapper& rhsMapper, ResScalar* res,
                                    Index resIncr, const RhsScalar& alpha) {
    const LhsScalar* lhs = lhsMapper.data();
    const Index lhsStride = lhsMapper.stride();
    const RhsScalar* rhs = rhsMapper.data();
    const Index rhsIncr = rhsMapper.stride();
    run(rows, cols, lhs, lhsStride, rhs, rhsIncr, res, resIncr, alpha);
  }

  EIGEN_DONT_INLINE static void run(Index rows, Index cols,
                                    const LhsScalar* lhs, Index lhsStride,
                                    const RhsScalar* rhs, Index rhsIncr,
                                    ResScalar* res, Index resIncr,
                                    const RhsScalar& alpha) {
    for (Index i = 0; i < rows; i++) {
      res[i * resIncr] += stan::math::var(new stan::math::gevv_vvv_vari(
          &alpha,
          (static_cast<int>(LhsStorageOrder) == static_cast<int>(ColMajor))
              ? (&lhs[i])
              : (&lhs[i * lhsStride]),
          (static_cast<int>(LhsStorageOrder) == static_cast<int>(ColMajor))
              ? (lhsStride)
              : (1),
          rhs, rhsIncr, cols));
    }
  }
};

template <typename Index, int LhsStorageOrder, bool ConjugateLhs,
          int RhsStorageOrder, bool ConjugateRhs>
struct general_matrix_matrix_product<Index, stan::math::var, LhsStorageOrder,
                                     ConjugateLhs, stan::math::var,
                                     RhsStorageOrder, ConjugateRhs, ColMajor> {
  using LhsScalar = stan::math::var;
  using RhsScalar = stan::math::var;
  using ResScalar = stan::math::var;

  using Traits = gebp_traits<RhsScalar, LhsScalar>;

  using LhsMapper
      = const_blas_data_mapper<stan::math::var, Index, LhsStorageOrder>;
  using RhsMapper
      = const_blas_data_mapper<stan::math::var, Index, RhsStorageOrder>;

  EIGEN_DONT_INLINE
  static void run(Index rows, Index cols, Index depth, const LhsScalar* lhs,
                  Index lhsStride, const RhsScalar* rhs, Index rhsStride,
                  ResScalar* res, Index resStride, const ResScalar& alpha,
                  level3_blocking<LhsScalar, RhsScalar>& /* blocking */,
                  GemmParallelInfo<Index>* /* info = 0 */) {
    for (Index i = 0; i < cols; i++) {
      general_matrix_vector_product<
          Index, LhsScalar, LhsMapper, LhsStorageOrder, ConjugateLhs, RhsScalar,
          RhsMapper,
          ConjugateRhs>::run(rows, depth, lhs, lhsStride,
                             &rhs[static_cast<int>(RhsStorageOrder)
                                          == static_cast<int>(ColMajor)
                                      ? i * rhsStride
                                      : i],
                             static_cast<int>(RhsStorageOrder)
                                     == static_cast<int>(ColMajor)
                                 ? 1
                                 : rhsStride,
                             &res[i * resStride], 1, alpha);
    }
  }

  EIGEN_DONT_INLINE
  static void run(Index rows, Index cols, Index depth,
                  const LhsMapper& lhsMapper, const RhsMapper& rhsMapper,
                  ResScalar* res, Index resStride, const ResScalar& alpha,
                  level3_blocking<LhsScalar, RhsScalar>& blocking,
                  GemmParallelInfo<Index>* info = 0) {
    const LhsScalar* lhs = lhsMapper.data();
    const Index lhsStride = lhsMapper.stride();
    const RhsScalar* rhs = rhsMapper.data();
    const Index rhsStride = rhsMapper.stride();

    run(rows, cols, depth, lhs, lhsStride, rhs, rhsStride, res, resStride,
        alpha, blocking, info);
  }
};
}  // namespace internal
}  // namespace Eigen
#endif
