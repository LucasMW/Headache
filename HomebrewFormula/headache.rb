
class Headache < Formula
  desc "A language that compiles to 8 bit brainfuck"
  homepage "https://github.com/LucasMW/Headache"
  url "https://github.com/LucasMW/Headache/archive/v0.51.zip"
  sha256 "ec68f8a87b928a9c45741662f8eee5c3c859d02766684d5e1f1d3aac43813b2b"

  depends_on "bison" => :build

  def install
    make install
  end
end