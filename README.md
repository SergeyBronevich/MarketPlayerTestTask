1) Build: ./build.sh 
2) Run: Go to "build_debug" or " build_release" directory and launch ./MarketPlayerTestTask [image_file_path] [number_of_threads] [kernel_size] [sigma]

For example: 

cd build_release
./MarketPlayerTestTask ../resources/flowers.jpg

The results in the "resources/flowers_greyscale.png" file and in the "resources/flowers_gaussian_blur_9_2.000000.png" file. 
9 - kernel size and 2.000000 - sigma.
