
class Headache < Formula
  desc "Programming Language that compiles to 8 Bit Brainfuck"
  homepage ""
  url "https://github.com/LucasMW/Headache/archive/v0.51.zip"
  sha256 "ec68f8a87b928a9c45741662f8eee5c3c859d02766684d5e1f1d3aac43813b2b"

  depends_on "make"
  depends_on "flex"  
  depends_on "bison"

  def install
    system "make", "install"
  end

  test do
    system "make", "test"
  end
end
