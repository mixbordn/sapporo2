CUDA_VISIBLE_DEVICES=1,0 OPENCL_PROFILE_CSV=1 OPENCL_PROFILE=1 OPENCL_PROFILE_CONFIG=./profiler.conf OPENCL_PROFILE_LOG="perfOCL_41_GTX580_128k.csv" ./test_performance_ocl 131072

sleep 30

CUDA_VISIBLE_DEVICES=1,0 OPENCL_PROFILE_CSV=1 OPENCL_PROFILE=1 OPENCL_PROFILE_CONFIG=./profiler.conf OPENCL_PROFILE_LOG="perfOCL_41_GTX580_256k.csv" ./test_performance_ocl 262144
sleep 30

CUDA_VISIBLE_DEVICES=1,0 CUDA_PROFILE_CSV=1 CUDA_PROFILE=1 CUDA_PROFILE_CONFIG=./profiler.conf CUDA_PROFILE_LOG="perfCUDA_41_GTX580_128k.csv" ./test_performance_cuda 131072

sleep 30

CUDA_VISIBLE_DEVICES=1,0 CUDA_PROFILE_CSV=1 CUDA_PROFILE=1 CUDA_PROFILE_CONFIG=./profiler.conf CUDA_PROFILE_LOG="perfCUDA_41_GTX580_256k.csv" ./test_performance_cuda 262144