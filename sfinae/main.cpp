#include <vector>
#include <type_traits>

#include <Eigen/Eigen>
#include <xtensor/xtensor.hpp>
#include <xtensor/xarray.hpp>

#include <iostream>

namespace detail
{
    template <class T>
    struct is_xtensor : std::false_type
    {
    };

    template <class T>
    struct is_xtensor<xt::xarray<T>> : std::true_type
    {
    };

    template <class T, size_t N>
    struct is_xtensor<xt::xtensor<T, N>> : std::true_type
    {
    };

    template <class T, typename = void>
    struct io_impl
    {
        static T load()
        {
            std::cout << "load generic" << std::endl;
            return T();
        }

        static void dump(const T& data){
            std::cout << "dump generic" << std::endl;
        }
    };

    template <class T>
    struct io_impl<T, typename std::enable_if<std::is_base_of<Eigen::DenseBase<T>, T>::value>::type>
    {
        static T load()
        {
            std::cout << "load Eigen::DenseBase" << std::endl;
            return T();
        }

        static void dump(const T& data)
        {
            std::cout << "dump Eigen::DenseBase" << std::endl;
        }
    };

    template <class T>
    struct io_impl<T, typename std::enable_if<is_xtensor<T>::value>::type>
    {
        static T load()
        {
            std::cout << "load xt::xarray<T> or xt::xtensor<T, N>" << std::endl;
            return T();
        }

        static void dump(const T& data){
            std::cout << "dump xt::xarray<T> or xt::xtensor<T, N>" << std::endl;
        }
    };

    template <class T>
    struct io_impl<T, typename std::enable_if<std::is_same<std::vector<typename T::value_type>, T>::value>::type>
    {
        static T load()
        {
            std::cout << "load std::vector" << std::endl;
            return T();
        }

        static void dump(const T& data){
            std::cout << "dump std::vector" << std::endl;
        }
    };
};

template <class T>
inline void dump(const T& data)
{
    return detail::io_impl<std::decay_t<T>>::dump(data);
}

template <class T>
inline T load()
{
    return detail::io_impl<std::decay_t<T>>::load();
}

int main()
{
    Eigen::ArrayXf A = Eigen::ArrayXf::Random(50);
    Eigen::MatrixXd B = Eigen::MatrixXd::Random(20, 5);
    xt::xarray<double> C = {{1, 2, 3}, {4, 5, 6}};
    xt::xtensor<double,2> D = {{1, 2, 3}, {4, 5, 6}};
    std::vector<double> E = {1, 2, 3};
    int F = 0;

    dump(A);
    dump(B);
    dump(C);
    dump(D);
    dump(E);
    dump(F);

    load<decltype(A)>();
    load<decltype(B)>();
    load<decltype(C)>();
    load<decltype(D)>();
    load<decltype(E)>();
    load<decltype(F)>();

    return 0;
}
