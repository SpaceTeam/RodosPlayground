From ubuntu:latest

RUN apt-get update -yq \
&& apt-get install cmake git curl build-essential -yq \
&& apt-get install gcc-multilib g++-multilib -yq

# Make gcc-10 the default compiler
#RUN apt-get install gcc-10 -yq\
#&& apt-get install g++-10 -yq
#RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10 
#RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10 

RUN g++ --version
ENV HOME /

RUN git clone https://github.com/ETLCPP/etl.git \
&& git clone https://github.com/foonathan/type_safe.git \
&& git clone https://github.com/foonathan/debug_assert.git \
&& git clone https://github.com/catchorg/Catch2.git

ADD linux-x86.cmake .
RUN echo "something" 

WORKDIR etl
RUN cmake --toolchain /linux-x86.cmake -S . -B build \
&& cmake --install build
#&& cmake --build build --target install
WORKDIR $HOME

WORKDIR debug_assert
RUN cmake --toolchain /linux-x86.cmake -S . -B build \
&& cmake --install build
WORKDIR $HOME

WORKDIR type_safe
RUN cmake --toolchain /linux-x86.cmake -S . -B build \
&& cmake --install build
WORKDIR $HOME

RUN apt-get install tree -yq
RUN tree /usr/local/lib/cmake
RUN cat /usr/local/lib/cmake/debug_assert/debug_assert-config-version.cmake


WORKDIR Catch2
ADD linux-x86.cmake .
RUN git checkout v3.1.0
RUN cmake --toolchain linux-x86.cmake -S . -Bbuild -H.  -DBUILD_TESTING=OFF
RUN cmake --build build --target install
WORKDIR $HOME

RUN apt-get install clang-tidy cppcheck -y -q

ADD . ./rodos_playground/
WORKDIR rodos_playground
ADD linux-x86.cmake .
RUN cmake --preset=ci-linux-x86 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCI_TEST=ON
RUN cmake --build build --target RodosPlayground_test
WORKDIR $HOME


WORKDIR rodos_playground/build
RUN ls
RUN ./Test/Source/RodosPlayground_test
WORKDIR Test/Source
RUN ctest
