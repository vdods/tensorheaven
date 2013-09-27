// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen_svd.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_SVD_HPP_
#define TENH_INTEROP_EIGEN_SVD_HPP_

#include "tenh/core.hpp"
#include "tenh/interop/eigen.hpp"
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/tensor.hpp"

#include "Eigen/Core"
#include "Eigen/SVD"

namespace Tenh {

template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_>
void SVD_of_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<Factor2_> > >,
                                        Scalar_,
                                        UseArrayType_> const &t,
                     ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<TODO THINK ABOUT THIS> > >,
                                        Scalar_,
                                        UseArrayType_> const &u,
                     ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<TODO THINK ABOUT THIS, THINK ABOUT>,
                                        Scalar_,
                                        UseArrayType_> const &s,
                     ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<TODO THINK ABOUT THIS,
                                                                           TypeList_t<Factor2_> > >,
                                        Scalar_,
                                        UseArrayType_> const &v)
{
}

// SVD with u and v matrices; the singular values are in s; t = u * s * v^T
template <typename Factor1, typename Factor2, typename Basis, typename Derived>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t,
                     Tensor2_t<Factor1,Factor1> &u,
                     Tensor2Diagonal_t<Factor1,Factor2> &s,
                     Tensor2_t<Factor2,Factor2> &v)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(euclideanly_embedded_EigenMatrix_from(t).jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV));
    memcpy(u.pointer_to_allocation(), &svd.matrixU()(0,0), u.allocation_size_in_bytes());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
    memcpy(v.pointer_to_allocation(), &svd.matrixV()(0,0), v.allocation_size_in_bytes());
}

// SVD with u matrix but no v matrix; the singular values are in s.
template <typename Factor1, typename Factor2, typename Basis, typename Derived>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t,
                     Tensor2_t<Factor1,Factor1> &u,
                     Tensor2Diagonal_t<Factor1,Factor2> &s)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(euclideanly_embedded_EigenMatrix_from(t).jacobiSvd(Eigen::ComputeFullU));
    memcpy(u.pointer_to_allocation(), &svd.matrixU()(0,0), u.allocation_size_in_bytes());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
}

// SVD with v matrix but no u matrix; the singular values are in s.
template <typename Factor1, typename Factor2, typename Basis, typename Derived>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t,
                     Tensor2Diagonal_t<Factor1,Factor2> &s,
                     Tensor2_t<Factor2,Factor2> &v)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(euclideanly_embedded_EigenMatrix_from(t).jacobiSvd(Eigen::ComputeFullU));
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
    memcpy(v.pointer_to_allocation(), &svd.matrixV()(0,0), v.allocation_size_in_bytes());
}

// SVD with no u or v matrix; the singular values are in s.
template <typename Factor1, typename Factor2, typename Basis, typename Derived>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t,
                     Tensor2Diagonal_t<Factor1,Factor2> &s)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(euclideanly_embedded_EigenMatrix_from(t).jacobiSvd());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_SVD_HPP_
