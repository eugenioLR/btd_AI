mkdir -p build
cd build
cmake -DOpenGL_GL_PREFERENCE=GLVND ..
cmake --build .
cp ./btd_ai ../btd_ai
