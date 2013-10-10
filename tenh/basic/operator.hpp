// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/operator.hpp by Victor Dods, created 2013/10/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_OPERATOR_HPP_
#define TENH_BASIC_OPERATOR_HPP_

#include "tenh/core.hpp"

#include "tenh/basic/vector.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/expressiontemplate_reindex.hpp"
#include "tenh/implementation/tensor.hpp"

namespace Tenh {

// Domain_ and Codomain_ must be based vector spaces.
template <typename Domain_, typename Codomain_, typename Scalar_ = float, typename UseArrayType_ = UseMemberArray>
struct Operator
    :
    public ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                                                 TypeList_t<typename DualOf_f<Domain_>::T> > >,
                              Scalar_,
                              UseArrayType_>
{
private:
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                                                  TypeList_t<typename DualOf_f<Domain_>::T> > >,
                               Scalar_,
                               UseArrayType_> Parent_Implementation;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                               TypeList_t<typename DualOf_f<Domain_>::T> > > TensorProduct;
public:
    typedef typename Parent_Implementation::ComponentIndex ComponentIndex;
    typedef typename Parent_Implementation::MultiIndex MultiIndex;
    typedef typename DualOf_f<Operator>::T Dual; // relies on the template specialization below

    explicit Operator (WithoutInitialization const &w) : Parent_Implementation(w) { }
    Operator (Operator const &op) : Parent_Implementation(op) { }

    // only use these if UseMemberArray is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit Operator (Scalar_ const &fill_with)
        :
        Parent_Implementation(fill_with)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Operator (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Implementation(x.as_member_array())
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }

    // only use these if UsePreallocatedArray is specified

    explicit Operator (Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    Operator (Scalar_ const &fill_with,
              Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Operator (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
              Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(x, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }

    // only use this if UseImmutableArray_t<...> is specified or if the vector space is 0-dimensional
    Operator ()
        :
        Parent_Implementation(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseImmutableArray_f<UseArrayType_>::V || DimensionOf_f<TensorProduct>::V == 0,
                      MUST_BE_USE_IMMUTABLE_ARRAY_OR_BE_ZERO_DIMENSIONAL);
    }

    template <typename Derived_,
              typename FreeFactorTypeList_,
              typename FreeDimIndexTypeList_,
              typename UsedDimIndexTypeList_>
    void operator = (ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
    {
        STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 2, LENGTH_MUST_BE_EXACTLY_2);
        typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex1;
        typedef typename Head_f<typename Body_f<FreeDimIndexTypeList_>::T>::T RhsDimIndex2;
        typedef AbstractIndex_c<RhsDimIndex1::SYMBOL> IndexToRename1;
        typedef AbstractIndex_c<RhsDimIndex2::SYMBOL> IndexToRename2;
        typedef AbstractIndex_c<'i'> I;
        typedef AbstractIndex_c<'j'> J;
        typedef TypeList_t<IndexToRename1,
                TypeList_t<IndexToRename2> > DomainAbstractIndexTypeList;
        typedef TypeList_t<I,
                TypeList_t<J> > CodomainAbstractIndexTypeList;
        I i;
        J j;
        (*this)(i|j) = reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(rhs.as_derived());
    }
    // TODO: a "no alias" version of operator= -- this probably requires adding a no_alias() method to this class.

    static std::string type_as_string ()
    {
        return "Operator<" + type_string_of<Domain_>() + ','
                           + type_string_of<Codomain_>() + ','
                           + type_string_of<Scalar_>() + ','
                           + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
struct DualOf_f<Operator<Domain_,Codomain_,Scalar_,UseArrayType_> >
{
    // the "dual" of a linear operator is its natural adjoint; if
    // A : V --> W, then A* : W* --> V*, where * denotes the dual functor.
    typedef Operator<typename DualOf_f<Codomain_>::T,typename DualOf_f<Domain_>::T,Scalar_,typename DualOf_f<UseArrayType_>::T> T;
};

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename Lhs_UseArrayType_,
          typename Rhs_UseArrayType_>
ExpressionTemplate_Addition_t<
    typename Operator<Domain_,Codomain_,Scalar_,Lhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    typename Operator<Domain_,Codomain_,Scalar_,Rhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    '+'>
    operator + (Operator<Domain_,Codomain_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Operator<Domain_,Codomain_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs(i|j) + rhs(i|j);
}

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
ExpressionTemplate_Addition_t<
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Derived_,
    '+'>
    operator + (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
                ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 2, LENGTH_MUST_BE_EXACTLY_2);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex1;
    typedef typename Head_f<typename Body_f<FreeDimIndexTypeList_>::T>::T RhsDimIndex2;
    typedef AbstractIndex_c<RhsDimIndex1::SYMBOL> IndexToRename1;
    typedef AbstractIndex_c<RhsDimIndex2::SYMBOL> IndexToRename2;
    typedef AbstractIndex_c<'i'> I;
    typedef AbstractIndex_c<'j'> J;
    typedef TypeList_t<IndexToRename1,
            TypeList_t<IndexToRename2> > DomainAbstractIndexTypeList;
    typedef TypeList_t<I,
            TypeList_t<J> > CodomainAbstractIndexTypeList;
    I i;
    J j;
    return lhs(i|j) + reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(rhs.as_derived());
}

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
ExpressionTemplate_Addition_t<
    Derived_,
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    '+'>
    operator + (ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &lhs,
                Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 2, LENGTH_MUST_BE_EXACTLY_2);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex1;
    typedef typename Head_f<typename Body_f<FreeDimIndexTypeList_>::T>::T RhsDimIndex2;
    typedef AbstractIndex_c<RhsDimIndex1::SYMBOL> IndexToRename1;
    typedef AbstractIndex_c<RhsDimIndex2::SYMBOL> IndexToRename2;
    typedef AbstractIndex_c<'i'> I;
    typedef AbstractIndex_c<'j'> J;
    typedef TypeList_t<IndexToRename1,
            TypeList_t<IndexToRename2> > DomainAbstractIndexTypeList;
    typedef TypeList_t<I,
            TypeList_t<J> > CodomainAbstractIndexTypeList;
    I i;
    J j;
    return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(lhs.as_derived()) + rhs(i|j);
}

// the "ExpressionTemplate_i + ExpressionTemplate_i" situation is already handled
// by the existing expression template code.

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename Lhs_UseArrayType_,
          typename Rhs_UseArrayType_>
ExpressionTemplate_Addition_t<
    typename Operator<Domain_,Codomain_,Scalar_,Lhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    typename Operator<Domain_,Codomain_,Scalar_,Rhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    '-'>
    operator - (Operator<Domain_,Codomain_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Operator<Domain_,Codomain_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs(i|j) - rhs(i|j);
}

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
ExpressionTemplate_Addition_t<
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Derived_,
    '-'>
    operator - (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
                ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 2, LENGTH_MUST_BE_EXACTLY_2);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex1;
    typedef typename Head_f<typename Body_f<FreeDimIndexTypeList_>::T>::T RhsDimIndex2;
    typedef AbstractIndex_c<RhsDimIndex1::SYMBOL> IndexToRename1;
    typedef AbstractIndex_c<RhsDimIndex2::SYMBOL> IndexToRename2;
    typedef AbstractIndex_c<'i'> I;
    typedef AbstractIndex_c<'j'> J;
    typedef TypeList_t<IndexToRename1,
            TypeList_t<IndexToRename2> > DomainAbstractIndexTypeList;
    typedef TypeList_t<I,
            TypeList_t<J> > CodomainAbstractIndexTypeList;
    I i;
    J j;
    return lhs(i|j) - reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(rhs.as_derived());
}

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
ExpressionTemplate_Addition_t<
    Derived_,
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    '-'>
    operator - (ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &lhs,
                Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 2, LENGTH_MUST_BE_EXACTLY_2);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex1;
    typedef typename Head_f<typename Body_f<FreeDimIndexTypeList_>::T>::T RhsDimIndex2;
    typedef AbstractIndex_c<RhsDimIndex1::SYMBOL> IndexToRename1;
    typedef AbstractIndex_c<RhsDimIndex2::SYMBOL> IndexToRename2;
    typedef AbstractIndex_c<'i'> I;
    typedef AbstractIndex_c<'j'> J;
    typedef TypeList_t<IndexToRename1,
            TypeList_t<IndexToRename2> > DomainAbstractIndexTypeList;
    typedef TypeList_t<I,
            TypeList_t<J> > CodomainAbstractIndexTypeList;
    I i;
    J j;
    return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(lhs.as_derived()) - rhs(i|j);
}

// the "ExpressionTemplate_i - ExpressionTemplate_i" situation is already handled
// by the existing expression template code.

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Scalar_,
    '*'>
    operator * (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
                Scalar_ const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs(i|j) * rhs;
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Scalar_,
    '*'>
    operator * (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
                int rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs(i|j) * rhs;
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Scalar_,
    '*'>
    operator * (Scalar_ const &lhs,
                Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs * rhs(i|j);
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Scalar_,
    '*'>
    operator * (int lhs,
                Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs * rhs(i|j);
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_, typename ScalarOperand_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Scalar_,
    '/'>
    operator / (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
                ScalarOperand_ const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs(i|j) / rhs;
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Operator<Domain_,Codomain_,Scalar_,UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Scalar_,
    '*'>
    operator - (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &v)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return -v(i|j);
}

// this only allows natural pairings of Operators and Vectors and so forth
template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename Lhs_UseArrayType_,
          typename Rhs_UseArrayType_>
ExpressionTemplate_Multiplication_t<
    typename Operator<Domain_,Codomain_,Scalar_,Lhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'k'> > > >::T,
    typename Operator<Domain_,Codomain_,Scalar_,Rhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'k'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T>
    operator * (Operator<Domain_,Codomain_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Operator<Domain_,Codomain_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    return lhs(i|k) * rhs(k|j);
}

// this only allows natural compositions (in the vector space/dual space sense)
template <typename Lhs_BasedVectorSpace_,
          typename Lhs_UseArrayType_,
          typename Scalar_,
          typename Rhs_Domain_,
          typename Rhs_Codomain_,
          typename Rhs_UseArrayType_>
ExpressionTemplate_Multiplication_t<
    typename Vector<Lhs_BasedVectorSpace_,Scalar_,Lhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<'i'>::T,
    typename Operator<Rhs_Domain_,Rhs_Codomain_,Scalar_,Rhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T>
    operator * (Vector<Lhs_BasedVectorSpace_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Operator<Rhs_Domain_,Rhs_Codomain_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs(i) * rhs(i|j);
}

// this only allows natural compositions (in the vector space/dual space sense)
template <typename Lhs_Domain_,
          typename Lhs_Codomain_,
          typename Lhs_UseArrayType_,
          typename Scalar_,
          typename Rhs_BasedVectorSpace_,
          typename Rhs_UseArrayType_>
ExpressionTemplate_Multiplication_t<
    typename Operator<Lhs_Domain_,Lhs_Codomain_,Scalar_,Lhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T,
    typename Vector<Rhs_BasedVectorSpace_,Scalar_,Rhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<'j'>::T>
    operator * (Operator<Lhs_Domain_,Lhs_Codomain_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Vector<Rhs_BasedVectorSpace_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    return lhs(i|j) * rhs(j);
}

// this only allows natural compositions (in the vector space/dual space sense)
template <typename Lhs_Domain_,
          typename Lhs_Codomain_,
          typename Lhs_UseArrayType_,
          typename Scalar_,
          typename Rhs_Domain_,
          typename Rhs_Codomain_,
          typename Rhs_UseArrayType_>
ExpressionTemplate_Multiplication_t<
    typename Operator<Lhs_Domain_,Lhs_Codomain_,Scalar_,Lhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'i'>,
                TypeList_t<AbstractIndex_c<'k'> > > >::T,
    typename Operator<Rhs_Domain_,Rhs_Codomain_,Scalar_,Rhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                TypeList_t<AbstractIndex_c<'k'>,
                TypeList_t<AbstractIndex_c<'j'> > > >::T>
    operator * (Operator<Lhs_Domain_,Lhs_Codomain_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Operator<Rhs_Domain_,Rhs_Codomain_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    return lhs(i|k) * rhs(k|j);
}
/*
// this only allows natural compositions (in the vector space/dual space sense)
template <typename Lhs_Domain_,
          typename Lhs_Codomain_,
          typename Lhs_UseArrayType_,
          typename Scalar_,
          typename Rhs_Derived_,
          typename Rhs_UsedDimIndexTypeList_>
typename ExpressionTemplate_ScalarMultiplication_t<
    typename Operator<Lhs_Domain_,Lhs_Codomain_,Scalar_,Lhs_UseArrayType_>
             ::template IndexedExpressionConstType_f<
                 TypeList_t<AbstractIndex_c<'i'>,
                 TypeList_t<AbstractIndex_c<'j'> > > >::T,
    Scalar_,
    '*'>
    operator * (Operator<Lhs_Domain_,Lhs_Codomain_,Scalar_,Lhs_UseArrayType_> const &lhs,
                ExpressionTemplate_i<Rhs_Derived_,Scalar_,EmptyTypeList,EmptyTypeList,Rhs_UsedDimIndexTypeList_> const &rhs)
{
    typedef AbstractIndex_c<'i'> i;
    typedef AbstractIndex_c<'j'> j;
    // this uses operator Scalar_ () in the expression template
    return lhs(i|j) * Scalar_(rhs);
}


// metafunctions to help with the type system atrocity that is the next few functions.
template <typename Lhs_Domain_,
          typename Lhs_Codomain_,
          typename Lhs_UseArrayType_,
          typename Scalar_,
          typename Rhs_Derived_,
          typename Rhs_FreeFactorTypeList_,
          typename Rhs_FreeDimIndexTypeList_,
          typename Rhs_UsedDimIndexTypeList_>
struct OperatorTimesExpressionTemplate_Helper_t
{
    // NOTE: it's critical that 'k' > 'i' here, so that none of the expression template's
    // indices are mapped to 'i' and would therefore contract with it.
    typedef TypeList_t<AbstractIndex_c<'i'>,TypeList_t<AbstractIndex_c<'k'> > Lhs_AbstractIndexTypeList;
    enum
    {
        STATIC_ASSERT_IN_ENUM((Rhs_FreeDimIndexTypeList_::LENGTH >= 1), LENGTH_MUST_BE_AT_LEAST_1)
    };
    typedef TypeList_t<AbstractIndex_c<Rhs_FreeDimIndexTypeList_::HeadType::SYMBOL> > DomainAbstractIndexTypeList;
    typedef TypeList_t<AbstractIndex_c<'k'> > CodomainAbstractIndexTypeList;
    typedef Reindex_e<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList> Reindex;

    // Dear citizens of Tensor Heaven,
    //
    // I am *so* sorry for this type.  One day, when the holy scripture of
    // C++11 is observed, this will all be made better.
    //
    // Sincerely,
    // Tensor God.
    typedef typename ExpressionTemplate_Multiplication_t<
                         typename Operator<Lhs_Domain_,Lhs_Codomain_,Scalar_,Lhs_UseArrayType_>
                                  ::template IndexedExpressionConstType_f<Lhs_AbstractIndexTypeList>::T,
                         typename Reindex
                                  ::template Eval_f<
                                      ExpressionTemplate_i<Rhs_Derived_,
                                                           Scalar_,
                                                           Rhs_FreeFactorTypeList_,
                                                           Rhs_FreeDimIndexTypeList_,
                                                           Rhs_UsedDimIndexTypeList_> >::T ReturnType;

};

// this only allows natural compositions (in the vector space/dual space sense)
template <typename Lhs_Domain_,
          typename Lhs_Codomain_,
          typename Lhs_UseArrayType_,
          typename Scalar_,
          typename Rhs_Derived_,
          typename Rhs_FreeFactorTypeList_,
          typename Rhs_FreeDimIndexTypeList_,
          typename Rhs_UsedDimIndexTypeList_>
typename OperatorTimesExpressionTemplate_Helper_t<Lhs_Domain_,
                                                  Lhs_Codomain_,
                                                  Lhs_UseArrayType_,
                                                  Scalar_,
                                                  Rhs_Derived_,
                                                  Rhs_FreeFactorTypeList_,
                                                  Rhs_FreeDimIndexTypeList_,
                                                  Rhs_UsedDimIndexTypeList_>::ReturnType
    operator * (Operator<Lhs_Domain_,Lhs_Codomain_,Scalar_,Lhs_UseArrayType_> const &lhs,
                ExpressionTemplate_i<Rhs_Derived_,Scalar_,Rhs_FreeFactorTypeList_,Rhs_FreeDimIndexTypeList_,Rhs_UsedDimIndexTypeList_> const &rhs)
{
    STATIC_ASSERT(Rhs_FreeDimIndexTypeList_::LENGTH == 1 || Rhs_FreeDimIndexTypeList_::LENGTH == 2,
                  LENGTH_MUST_BE_EXACTLY_1_OR_2);

    typedef OperatorTimesExpressionTemplate_Helper_t<Lhs_Domain_,
                                                     Lhs_Codomain_,
                                                     Lhs_UseArrayType_,
                                                     Scalar_,
                                                     Rhs_Derived_,
                                                     Rhs_FreeFactorTypeList_,
                                                     Rhs_FreeDimIndexTypeList_,
                                                     Rhs_UsedDimIndexTypeList_> OperatorTimesExpressionTemplate_Helper;
    typedef typename OperatorTimesExpressionTemplate_Helper::DomainAbstractIndexTypeList DomainAbstractIndexTypeList;
    typedef typename OperatorTimesExpressionTemplate_Helper::CodomainAbstractIndexTypeList CodomainAbstractIndexTypeList;


    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(lhs.as_derived()) - rhs(i);

    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    return lhs(i|k) * rhs(k|j);
}

*/
/*
// this only allows natural pairings of vectors with covectors (via the existing expression template code)
template <typename Lhs_Domain_,
          typename Lhs_Codomain_,
          typename Rhs_Domain_,
          typename Rhs_Codomain_,
          typename Scalar_,
          typename Lhs_UseArrayType_,
          typename Rhs_UseArrayType_>
Scalar_ operator * (Operator<Lhs_BasedVectorSpace_,Scalar_,Lhs_UseArrayType_> const &lhs,
                    Operator<Rhs_BasedVectorSpace_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) * rhs(i);
}

// this only allows natural pairings of vectors with covectors (via the existing expression template code)
template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
Scalar_ operator * (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
                    ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return lhs(i) * reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(rhs.as_derived());
}

// this only allows natural pairings of vectors with covectors (via the existing expression template code)
template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
Scalar_ operator * (ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &lhs,
                    Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(lhs.as_derived()) * rhs(i);
}

// the "ExpressionTemplate_i * ExpressionTemplate_i" situation is already handled
// by the existing expression template code.
*/
/*

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename Lhs_BasedVectorSpace_,
          typename Rhs_BasedVectorSpace_,
          typename Scalar_,
          typename Lhs_UseArrayType_,
          typename Rhs_UseArrayType_>
XYZ operator % (Operator<Lhs_BasedVectorSpace_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Operator<Rhs_BasedVectorSpace_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    // TODO: write an expression template for outer product -- essentially taking a list of Vector_i types
    // and bundling each of their indices into a single index.
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
XYZ operator % (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
                ExpressionTemplate_OuterProduct_t<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
{
    // TODO: return a new outer product expression template
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
XYZ operator % (ExpressionTemplate_OuterProduct_t<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &lhs,
                Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &rhs)
{
    // TODO: return a new outer product expression template
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename Scalar_,
          typename Lhs_Derived_,
          typename Lhs_FreeFactorTypeList_,
          typename Lhs_FreeDimIndexTypeList_,
          typename Lhs_UsedDimIndexTypeList_,
          typename Rhs_Derived_,
          typename Rhs_FreeFactorTypeList_,
          typename Rhs_FreeDimIndexTypeList_,
          typename Rhs_UsedDimIndexTypeList_>
XYZ operator % (ExpressionTemplate_OuterProduct_t<Lhs_Derived_,Scalar_,Lhs_FreeFactorTypeList_,Lhs_FreeDimIndexTypeList_,Lhs_UsedDimIndexTypeList_> const &lhs,
                ExpressionTemplate_OuterProduct_t<Rhs_Derived_,Scalar_,Rhs_FreeFactorTypeList_,Rhs_FreeDimIndexTypeList_,Rhs_UsedDimIndexTypeList_> const &rhs)
{
    // TODO: return a new outer product expression template
}
*/
} // end of namespace Tenh

#endif // TENH_BASIC_OPERATOR_HPP_