# Use an official Ubuntu base image
FROM ubuntu:20.04

# Set environment variables to prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    openmpi-common \
    libopenmpi-dev \
    libnuma-dev \
    libpthread-stubs0-dev \
    wget \
    gnupg \
    lsb-release \
    software-properties-common \
    && apt-get clean

# Install Intel oneAPI toolkit
RUN wget -qO - https://apt.repos.intel.com/oneapi/gpgkey | apt-key add - \
    && echo "deb https://apt.repos.intel.com/oneapi all main" > /etc/apt/sources.list.d/oneAPI.list \
    && apt-get update \
    && apt-get install -y intel-basekit intel-hpckit \
    && apt-get clean

# Set up environment variables for Intel oneAPI
ENV ONEAPI_ROOT=/opt/intel/oneapi
ENV CC=$ONEAPI_ROOT/compiler/latest/linux/bin/icx
ENV CXX=$ONEAPI_ROOT/compiler/latest/linux/bin/icpx
ENV MKLROOT=$ONEAPI_ROOT/mkl/latest
ENV PATH=$ONEAPI_ROOT/compiler/latest/linux/bin:$PATH
ENV LD_LIBRARY_PATH=$ONEAPI_ROOT/compiler/latest/linux/compiler/lib/intel64_lin:$LD_LIBRARY_PATH

# Set up working directory
WORKDIR /app

# Copy the entire project directory to the container
COPY . /app

# Set environment variables for MPI
ENV PATH /usr/lib/openmpi/bin:$PATH
ENV LD_LIBRARY_PATH /usr/lib/openmpi/lib:$LD_LIBRARY_PATH

# Build the application
RUN make

# Expose the port on which the server will run
EXPOSE 8080

# Command to run the server
CMD ["./bin/hpc_server"]
