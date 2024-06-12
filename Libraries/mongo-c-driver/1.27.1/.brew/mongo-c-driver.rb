class MongoCDriver < Formula
  desc "C driver for MongoDB"
  homepage "https://github.com/mongodb/mongo-c-driver"
  url "https://github.com/mongodb/mongo-c-driver/archive/refs/tags/1.27.1.tar.gz"
  sha256 "cc97407b16da54ba9b27029237b9a7a13651ada04da15e3b5dcd8fee8db20eed"
  license "Apache-2.0"
  head "https://github.com/mongodb/mongo-c-driver.git", branch: "master"

  livecheck do
    url :stable
    regex(/^v?(\d+(?:\.\d+)+)$/i)
  end

  depends_on "cmake" => :build
  depends_on "pkg-config" => :build
  depends_on "sphinx-doc" => :build
  depends_on "openssl@3"

  uses_from_macos "zlib"

  def install
    cmake_args = std_cmake_args
    cmake_args << "-DCMAKE_INSTALL_RPATH=#{rpath}"
    File.write "VERSION_CURRENT", version.to_s unless build.head?
    inreplace "src/libmongoc/src/mongoc/mongoc-config.h.in", "@MONGOC_CC@", ENV.cc
    system "cmake", ".", *cmake_args
    system "make", "install"
    (pkgshare/"libbson").install "src/libbson/examples"
    (pkgshare/"libmongoc").install "src/libmongoc/examples"
  end

  test do
    system ENV.cc, "-o", "test", pkgshare/"libbson/examples/json-to-bson.c",
      "-I#{include}/libbson-1.0", "-L#{lib}", "-lbson-1.0"
    (testpath/"test.json").write('{"name": "test"}')
    assert_match "\u0000test\u0000", shell_output("./test test.json")

    system ENV.cc, "-o", "test", pkgshare/"libmongoc/examples/mongoc-ping.c",
      "-I#{include}/libmongoc-1.0", "-I#{include}/libbson-1.0",
      "-L#{lib}", "-lmongoc-1.0", "-lbson-1.0"
    assert_match "No suitable servers", shell_output("./test mongodb://0.0.0.0 2>&1", 3)
  end
end
