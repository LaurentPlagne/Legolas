/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImageFilter.cxx
 * Author: lolo
 *
 * Created on 13 mai 2017, 13:17
 */

#include <cstdlib>
#include "bitmap_image.hpp"
#include "UTILITES.hxx"
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

using namespace std;

struct Mean3Blur {

    template <class A4D>
    void operator()(int begin, int end, const A4D source, A4D target) const {

        //        typedef typename A3D::Element Element;
        typedef typename A4D::RealType Scalar;

        for (int k = begin; k < end; k++) {
            auto source3D = source[k];
            auto target3D = target[k];

            const int nc = source3D.size();
            assert(nc == 3);
            const int ny = source3D[0].size();
            const int nx = source3D[0][0].size();

            const int r = 5;

            const Scalar coef(1.0 / ((2 * r + 1)*(2 * r + 1)));
            const Scalar zero(0.0);

            Scalar sumR(zero);
            Scalar sumG(zero);
            Scalar sumB(zero);


            for (int j = 0; j < ny; j++) {
                for (int i = 0; i < nx; i++) {

                    sumR = zero;
                    sumG = zero;
                    sumB = zero;

                    for (int sj = -r; sj <= r; sj++) {
                        int jp = j + sj;
                        if (jp < 0 || jp >= ny) jp = j - sj;

                        for (int si = -r; si <= r; si++) {

                            int ip = i + si;
                            if (ip < 0 || ip >= nx) ip = i - si;

                            sumR += coef * source3D[0][jp][ip];
                            sumG += coef * source3D[1][jp][ip];
                            sumB += coef * source3D[2][jp][ip];


                        }
                    }

                    target3D[0][j][i] = sumR;
                    target3D[1][j][i] = sumG;
                    target3D[2][j][i] = sumB;



                }
            }
        }
    }
};

template <int RADIUS>
struct MeanKernel {
    static const int radius = RADIUS;

    double operator()(int i, int j) const {
        const int ks = 2 * radius + 1;

        return 1.0 / (ks * ks);
    }
};

template <int RADIUS>
struct GaussianKernel {
    static const int radius = RADIUS;
    const double sigma_ = double(radius) / 2.57;
    double coef_;

    double gauss(int i, int j) const {
        const double PI = 3.141592653589793238463;
        const int is = (radius) - i;
        const int js = (radius) - j;

        return std::exp(-double(is * is + js * js) / (2.0 * PI * sigma_ * sigma_));
    }

    GaussianKernel(void) : sigma_(double(radius) / 2.57) {


        const int ks = 2 * radius + 1;
        double sum = 0.;
        for (int i = 0; i < ks; i++) {
            for (int j = 0; j < ks; j++) {
                sum += gauss(i, j);
            }
        }

        coef_ = 1.0 / sum;

        //        for (int i = 0; i < ks; i++) {
        //            for (int j = 0; j < ks; j++) {
        //                const int is = (radius) - i;
        //                const int js = (radius) - j;
        //                std::cout << i << " " << j << " " << is << " " << js << " " << coef_ * gauss(i, j) ;
        //            }
        //            std::cout << std::endl;
        //        }

    }

    double operator()(int i, int j) const {
        return coef_ * gauss(i, j);
    }
};

template <class KERNEL>
struct Convolution {
    KERNEL kernel_;

    Convolution() : kernel_() {
    }

    template <class A4D>
    void operator()(int begin, int end, const A4D source, A4D target) const {

        //        typedef typename A3D::Element Element;
        typedef typename A4D::RealType Scalar;

        const int r = KERNEL::radius;
        const int ks = 2 * r + 1;
        Scalar kmat[ks][ks];
        for (int ik = 0; ik < ks; ik++) {
            for (int jk = 0; jk < ks; jk++) {
                kmat[jk][ik] = Scalar(kernel_(ik, jk));
            }
        }


        for (int k = begin; k < end; k++) {
            auto source3D = source[k];
            auto target3D = target[k];

            //const int nc = source3D.size();
            assert(source3D.size() == 3);
            const int ny = source3D[0].size();
            const int nx = source3D[0][0].size();

            const int r = KERNEL::radius;

            const Scalar zero(0.0);

            Scalar sumR(zero);
            Scalar sumG(zero);
            Scalar sumB(zero);


            for (int j = 0; j < ny; j++) {
                for (int i = 0; i < nx; i++) {

                    sumR = zero;
                    sumG = zero;
                    sumB = zero;

                    for (int sj = -r; sj <= r; sj++) {
                        int jp = j + sj;
                        if (jp < 0 || jp >= ny) jp = j - sj;

                        for (int si = -r; si <= r; si++) {

                            int ip = i + si;

                            if (ip < 0 || ip >= nx) ip = i - si;

                            sumR += kmat[sj + r][si + r] * source3D[0][jp][ip];
                            sumG += kmat[sj + r][si + r] * source3D[1][jp][ip];
                            sumB += kmat[sj + r][si + r] * source3D[2][jp][ip];


                        }
                    }

                    target3D[0][j][i] = sumR;
                    target3D[1][j][i] = sumG;
                    target3D[2][j][i] = sumB;



                }
            }
        }
    }
};

template <class A3D>
void saveArray3DToImage(const A3D a3d, bitmap_image & image) {
    const int nc = a3d.size();
    assert(nc == 3);
    const int ny = a3d[0].size();
    const int nx = a3d[0][0].size();
    typedef typename A3D::RealType Scalar;
    Legolas::Array<Scalar, 3> tmp(nc, ny, nx);
    for (int c = 0; c < nc; c++) {
        for (int j = 0; j < ny; j++) {
            for (int i = 0; i < nx; i++) {

                tmp[c][j][i] = a3d[c][j][i];
            }
        }
    }
    image.import_rgb(tmp[0].realDataPtr(), tmp[1].realDataPtr(), tmp[2].realDataPtr());
}

template <class A3D>
void loadArray3DFromImage(const bitmap_image & image, A3D a3d) {
    const int nc = a3d.size();
    assert(nc == 3);
    const int ny = a3d[0].size();
    const int nx = a3d[0][0].size();
    typedef typename A3D::RealType Scalar;
    Legolas::Array<Scalar, 3> tmp(nc, ny, nx);
    image.export_rgb(tmp[0].realDataPtr(), tmp[1].realDataPtr(), tmp[2].realDataPtr());

    for (int c = 0; c < nc; c++) {
        for (int j = 0; j < ny; j++) {
            for (int i = 0; i < nx; i++) {

                a3d[c][j][i] = tmp[c][j][i];
            }
        }
    }
}

struct SeqMap{
    template <typename... Args>
    static inline void apply(const Args... args){Legolas::map(args...);}
    static inline std::string name( void ){ return "sequential";}
};

struct ParMap{
    template <typename... Args>
    static inline void apply(const Args... args){Legolas::parmap(args...);}
    static inline std::string name( void ){ return "parallel";}
};


template <class MAP_TYPE>
struct TimeEval{

    template <typename... Args>
    static inline double apply(const Args... args){

        const int nloop=1;
        const int nsample=3;

        double minTime=std::numeric_limits<double>::max();
        for (int s=0 ; s<nsample ; s++){

            auto start_time = std::chrono::high_resolution_clock::now();
            for (int i=0 ; i<nloop ; i++){
                MAP_TYPE::apply(args...);
            }
            auto end_time = std::chrono::high_resolution_clock::now();

            const double microsec=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            const double sec=microsec/1.e6;
            if (minTime>sec) minTime=sec;
        }
        //    display2D(X2D);
        return minTime/double(nloop);
    }
};

template <class V4D, class MAP_TYPE>
struct ConvolutionBench{

    static inline std::string name( void ){
        std::string result("Convolution");
        result+="_"+std::to_string(V4D::packSize)+"_"+MAP_TYPE::name();
        return result;
    }


    static inline double evalGflops(const int nx){


        const int ny = nx;
        //      const int nz = 200; // nimages
        const int nz = 256; // nimages
        const int nc = 3; //nchannel (rgb)

        V4D sourceImages(nz, nc, ny, nx);
        V4D targetImages(nz, nc, ny, nx);

        sourceImages.fill(1.0);

        static const int radius = 5;
        typedef GaussianKernel<radius> Kernel;


        const double t=TimeEval<MAP_TYPE>::apply(Convolution<Kernel>(),sourceImages,targetImages);
        const double nbops = double(2 * radius + 1) * double(2 * radius + 1) * double(nz * nc * ny * nx)*2.0;
        return nbops/(t*1.e9);

    }


};

inline std::vector<int> makeSizes( void ){
    const int firstPowerOfTwo=8;
    const int lastPowerOfTwo=512;
    const int nbPointsBetweenPowerOfTwo=5;

    std::vector<int> result;

    int powerOfTwo=firstPowerOfTwo;
    while (powerOfTwo<lastPowerOfTwo){
        const int nextPowerOfTwo=powerOfTwo*2;
        const int shift=(nextPowerOfTwo-powerOfTwo)/nbPointsBetweenPowerOfTwo;
        int s=powerOfTwo;
        for (int i=0 ; i< nbPointsBetweenPowerOfTwo-1 ; i++){
            result.push_back(s);
            s+=shift;
        }
        powerOfTwo=nextPowerOfTwo;
    }
    result.push_back(powerOfTwo);

    for (size_t i=0 ; i<result.size(); i++){
        std::cout << "sizes["<<i <<"]="<<result[i] << std::endl;
    }
    return result;
}
inline void savePerf(const std::vector<int> & sizes,
                     const std::vector<double> & perfs,
                     const std::string filename){
     std::ofstream myfile;
     myfile.open(filename.c_str());
     for (size_t i=0 ; i<sizes.size() ; i++){
       myfile << sizes[i] <<" "<< perfs[i] << std::endl;
     }
     myfile.close();
}


template <class AlgoBench>
void goBench(void){
//    std::vector<int> sizes=makeSizes();
    std::vector<int> sizes={10,20,40,80,160,320,640,1280};
    const int nbPoints=sizes.size();
    std::vector<double> perfs(nbPoints);
    for (int i=nbPoints-1 ; i>=0 ; i--) perfs[i]=AlgoBench::evalGflops(sizes[i]);
    std::string filename(AlgoBench::name());
    filename+=".dat";
    savePerf(sizes,perfs,filename);
}
/**/



/*
 *
 */
int main(int argc, char** argv) {

    INFOS("ImageFilterTest");
    typedef float RealType;

    goBench< ConvolutionBench<Legolas::Array<RealType,4>, SeqMap > >();
    goBench< ConvolutionBench<Legolas::Array<RealType,4,4,4>, SeqMap > >();
    goBench< ConvolutionBench<Legolas::Array<RealType,4,8,4>, SeqMap > >();
    goBench< ConvolutionBench<Legolas::Array<RealType,4>, ParMap > >();
    goBench< ConvolutionBench<Legolas::Array<RealType,4,4,4>, ParMap > >();
    goBench< ConvolutionBench<Legolas::Array<RealType,4,8,4>, ParMap > >();

    std::string file_name("lena.bmp");
    bitmap_image image(file_name);

    const int nx = image.width();
    const int ny = image.height();

    typedef float RealType;



    //    typedef Legolas::Array<RealType, 4,4,4> A4D;
    typedef Legolas::Array<RealType, 4,8,4> A4D;

    const int nz = 200; // nimages
    const int nc = 4; //nchannel (rgb)

    A4D sourceImages(nz, nc, ny, nx);
    A4D targetImages(nz, nc, ny, nx);

    for (int k = 0; k < nz; k++) {
        loadArray3DFromImage(image, sourceImages[k]);
    }


    auto start_time = std::chrono::high_resolution_clock::now();

    static const int radius = 5;
    typedef GaussianKernel<radius> Kernel;


    //    Legolas::parmap(Convolution< Kernel >(), sourceImages, targetImages);
    Legolas::parmap(Convolution< Kernel >(), sourceImages, targetImages);
    auto end_time = std::chrono::high_resolution_clock::now();

    const double microsec = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    INFOS(" duration : " << microsec / 1000000. << " (s)");
    double nbops = double(2 * radius + 1) * double(2 * radius + 1) * double(nz * nc * ny * nx)*2.0;
    double GFLOPS = nbops / (microsec * 1000.);
    INFOS(" GFLOPS=" << GFLOPS)


            saveArray3DToImage(targetImages[0], image);


    image.save_image("lenaBlured.bmp");


    return 0;
}

