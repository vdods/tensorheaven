// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/innerproduct.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
#define TENH_IMPLEMENTATION_INNERPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/vee.hpp"

namespace Tenh {

template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_> struct InnerProduct_f;

// corresponds to the identity matrix
struct StandardInnerProduct { static std::string type_as_string () { return "StandardInnerProduct"; } };

// template specialization for inner product on Euclidean space
template <typename VectorSpace_, typename Scalar_>
struct InnerProduct_f<BasedVectorSpace_c<VectorSpace_,StandardBasis>,StandardInnerProduct,Scalar_>
{
private:
    typedef BasedVectorSpace_c<VectorSpace_,StandardBasis> BasedVectorSpace;
    typedef typename DualOf_f<BasedVectorSpace>::T DualOfBasedVectorSpace;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfBasedVectorSpace> SymmetricPower;
    // for using bundle_index_map -- the use of UsePreallocatedArray is somewhat arbitrary,
    // since only the static method bundle_index_map will be used.
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UsePreallocatedArray> BootstrappingImplementation;
    typedef ComponentIndex_t<AS_VECTOR_SPACE(SymmetricPower)::DIMENSION> ComponentIndex;
    static Scalar_ evaluator (ComponentIndex const &i)
    {
        typedef typename BootstrappingImplementation::MultiIndex::IndexTypeList IndexTypeList;
        typedef MultiIndex_t<IndexTypeList> MultiIndex;
        MultiIndex m(BootstrappingImplementation::template bundle_index_map<IndexTypeList,ComponentIndex>(i));
        return m.template el<0>() == m.template el<1>() ? Scalar_(1) : Scalar_(0);
    }
    typedef ComponentGenerator_t<Scalar_,AS_VECTOR_SPACE(SymmetricPower)::DIMENSION,evaluator,StandardInnerProduct> ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

template <Uint32 ORDER_, typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_>
struct InnerProduct_f<TensorPowerOfBasedVectorSpace_c<ORDER_,BasedVectorSpace_>,TensorPower_c<ORDER_,InnerProductId_>,Scalar_>
{
private:
    enum { STATIC_ASSERT_IN_ENUM(ORDER_ > 0, ORDER_MUST_BE_POSITIVE) }; // TODO: allow ORDER_ == 0
    typedef TensorPowerOfBasedVectorSpace_c<ORDER_,BasedVectorSpace_> TensorPowerOfBasedVectorSpace;
    typedef typename DualOf_f<TensorPowerOfBasedVectorSpace>::T DualOfTensorPowerOfBasedVectorSpace;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfTensorPowerOfBasedVectorSpace> SymmetricPower;
    typedef ComponentIndex_t<AS_VECTOR_SPACE(SymmetricPower)::DIMENSION> ComponentIndex;
    static Scalar_ evaluator (ComponentIndex const &i)
    {
        typedef typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T HeadInnerProduct;
        HeadInnerProduct head_inner_product;
        typedef typename InnerProduct_f<TensorPowerOfBasedVectorSpace_c<ORDER_-1,BasedVectorSpace_>,TensorPower_c<ORDER_-1,InnerProductId_>,Scalar_>::T BodyInnerProduct;
        BodyInnerProduct body_inner_product;
        AbstractIndex_c<'a'> a;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'B'> B;
        AbstractIndex_c<'P'> P;
        AbstractIndex_c<'Q'> Q;
        AbstractIndex_c<'r'> r;
        AbstractIndex_c<'s'> s;
        AbstractIndex_c<'T'> T;
        return (head_inner_product(a).split(a,j|k) * body_inner_product(B).split(B,P|Q))
               .bundle(j|P,TensorPowerOfBasedVectorSpace(),r)
               .bundle(k|Q,TensorPowerOfBasedVectorSpace(),s)
               .bundle(r|s,SymmetricPower(),T)[i];
    }
    typedef ComponentGenerator_t<Scalar_,AS_VECTOR_SPACE(SymmetricPower)::DIMENSION,evaluator,TensorPower_c<ORDER_,InnerProductId_> > ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

// template specialization for ORDER_ == 1 (which is really just the original inner product)
// TODO: do specialization for ORDER_ == 0 (but this would depend on the basis being Standard
// (i.e. the 0th tensor power is the field itself and 1 is the basis vector)).
template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_>
struct InnerProduct_f<TensorPowerOfBasedVectorSpace_c<1,BasedVectorSpace_>,TensorPower_c<1,InnerProductId_>,Scalar_>
{
private:
    typedef TensorPowerOfBasedVectorSpace_c<1,BasedVectorSpace_> TensorPowerOfBasedVectorSpace;
    typedef typename DualOf_f<TensorPowerOfBasedVectorSpace>::T DualOfTensorPowerOfBasedVectorSpace;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(HasBasedVectorSpaceStructure_f<DualOfTensorPowerOfBasedVectorSpace>::V, MUST_BE_BASED_VECTOR_SPACE, HAS_STRUCTURE),
        //STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(DualOfTensorPowerOfBasedVectorSpace), MUST_BE_BASED_VECTOR_SPACE)
    };
    typedef typename AS_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(DualOfTensorPowerOfBasedVectorSpace) Dual;
    enum { STATIC_ASSERT_IN_ENUM__UNIQUE(IS_BASED_VECTOR_SPACE_UNIQUELY(Dual), MUST_BE_BASED_VECTOR_SPACE, THINGY) };
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfTensorPowerOfBasedVectorSpace> SymmetricPower;
    enum { STATIC_ASSERT_IN_ENUM(IS_VECTOR_SPACE_UNIQUELY(SymmetricPower), MUST_BE_VECTOR_SPACE) };
    typedef ComponentIndex_t<AS_VECTOR_SPACE(SymmetricPower)::DIMENSION> ComponentIndex;
    static Scalar_ evaluator (ComponentIndex const &i)
    {
        typedef typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T HeadInnerProduct;
        HeadInnerProduct head_inner_product;
        return head_inner_product[i];
    }
    typedef ComponentGenerator_t<Scalar_,AS_VECTOR_SPACE(SymmetricPower)::DIMENSION,evaluator,TensorPower_c<1,InnerProductId_> > ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_INNERPRODUCT_HPP_