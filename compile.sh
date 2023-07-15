mkdir -p build
cd build
cmake -DOpenGL_GL_PREFERENCE=GLVND ..
cmake --build .
cp ./btd_ai ../btd_ai
cp ./pybtd_ai.cpython-310-x86_64-linux-gnu.so ../pybtd_ai.so
#cp ./libpybtd_ai.so ../pybtd_ai.so
