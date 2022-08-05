From ubuntu:latest

RUN apt-get update -yq \
&& apt-get install cmake git curl build-essential -yq \
&& apt-get install gcc-multilib g++-multilib -yq

RUN apt-get install clang-tidy cppcheck -y -q

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
&& git clone https://github.com/catchorg/Catch2.git \
&& git clone https://github.com/SpaceTeam/rodos.git 


# Add linux-x86.cmake toolchain file to the root of our image
# since it is found in pulled git repository, we do not need 
# to add it from somewhere on our host machine
RUN cd rodos && find . -name linux-x86.cmake | xargs cp -t $HOME -v

# Install rodos
WORKDIR rodos
RUN cmake --toolchain cmake/port/linux-x86.cmake -S . -B build 
RUN cmake --build build
RUN cmake --install build
WORKDIR $HOME

# Install etl
WORKDIR etl
RUN cmake --toolchain /linux-x86.cmake -S . -B build \
&& cmake --install build
#&& cmake --build build --target install
WORKDIR $HOME

# Install debug_assert
WORKDIR debug_assert
RUN cmake --toolchain /linux-x86.cmake -S . -B build \
&& cmake --install build
WORKDIR $HOME

# Install type_safe
WORKDIR type_safe
RUN cmake --toolchain /linux-x86.cmake -S . -B build \
&& cmake --install build
WORKDIR $HOME

# Debug
RUN apt-get install tree -yq
RUN tree /usr/local/lib/cmake
RUN cat /usr/local/lib/cmake/debug_assert/debug_assert-config-version.cmake
RUN cat /usr/local/include/rodos/api/rodos-debug.h

# Install Catch2
WORKDIR Catch2
RUN git checkout v3.1.0
RUN cmake --toolchain /linux-x86.cmake -S . -Bbuild -H.  -DBUILD_TESTING=OFF
RUN cmake --build build --target install
WORKDIR $HOME


# Build tests
ADD . ./rodos_playground/
WORKDIR rodos_playground
RUN cmake --preset=ci-linux-x86 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
#RUN cmake --preset=ci-linux-x86 -DCI_TEST=ON
#RUN cmake --build build --target RodosPlayground_test
RUN cmake --build build 
WORKDIR $HOME

# Run tests
WORKDIR rodos_playground/build
RUN ls
RUN ./Test/Source/RodosPlayground_test
WORKDIR Test/Source
RUN ctest
