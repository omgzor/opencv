/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2010-2012, Multicoreware, Inc., all rights reserved.
// Copyright (C) 2010-2012, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// @Authors
//    Chunpeng Zhang, chunpeng@multicorewareinc.com
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other oclMaterials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors as is and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include <iomanip>
#include "precomp.hpp"

using namespace cv;
using namespace cv::ocl;
using namespace std;


#if !defined(HAVE_OPENCL)

void cv::ocl::columnSum(const oclMat &src, oclMat &dst)
{
    throw_nogpu();
}

#else /*!HAVE_OPENCL */

namespace cv
{
    namespace ocl
    {
        extern const char *imgproc_columnsum;
    }
}

void cv::ocl::columnSum(const oclMat &src, oclMat &dst)
{
    CV_Assert(src.type() == CV_32FC1);

    dst.create(src.size(), src.type());

    Context *clCxt = src.clCxt;

    const std::string kernelName = "columnSum";

    std::vector< pair<size_t, const void *> > args;

    args.push_back( make_pair( sizeof(cl_mem), (void *)&src.data));
    args.push_back( make_pair( sizeof(cl_mem), (void *)&dst.data));
    args.push_back( make_pair( sizeof(cl_int), (void *)&src.cols));
    args.push_back( make_pair( sizeof(cl_int), (void *)&src.rows));
    args.push_back( make_pair( sizeof(cl_int), (void *)&src.step));
    args.push_back( make_pair( sizeof(cl_int), (void *)&dst.step));

    size_t globalThreads[3] = {dst.cols, 1, 1};
    size_t localThreads[3]  = {256, 1, 1};

    openCLExecuteKernel(clCxt, &imgproc_columnsum, kernelName, globalThreads, localThreads, args, src.channels(), src.depth());

}
#endif