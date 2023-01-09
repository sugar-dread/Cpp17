#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>

/*
    With C++17 the Boost.filesystem library was finally adopted as a C++ standard library. By doing
    this, the library was adjusted to new language features, made more consistent with other parts of the
    library, cleaned-up, and extended to provided some missing pieces (such as operations to compute a
    relative path between filesystem paths).
*/

void create_different_types_of_files() {
  namespace fs= std::filesystem;
  try {
    // create directories tmp/test/ (if they don’t exist yet):
    fs::path testDir { "tmp/test" };
    create_directories(testDir);

    auto testFile= testDir / "data.txt";
    std::ofstream dataFile { testFile };
    if (!dataFile) {
      std::cout << "OOPS can't open \"" << testFile.string() << "\"\n";
      std::exit(EXIT_FAILURE);
    }

    dataFile << "The answer is 42\n";
    // create symbolic link from tmp/slink/ to tmp/test/:
    create_directory_symlink("test", testDir.parent_path() / "slink");

    // recursively list all files (also following symlinks)
    std::cout << fs::current_path().string() << ":\n";
    auto iterOpts { fs::directory_options::follow_directory_symlink };
    for (const auto& e : fs::recursive_directory_iterator(".", iterOpts)) {
      std::cout << " " << e.path().lexically_normal().string() << '\n';
    }
  } catch (const fs::filesystem_error& e) {
    std::cerr << "EXCEPTION: " << e.what() << '\n';
    std::cerr << " path1: \"" << e.path1().string() << "\"\n";
  }
}

int main() {
  if (std::filesystem::path p { "/home/phytm/Desktop" }; is_regular_file(p)) {
    std::cout << p << " exists with " << file_size(p) << " bytes\n";
  } else if (is_directory(p)) {
    std::cout << p << " is a directory containing:\n";
    for (auto& e : std::filesystem::directory_iterator { p }) {
      std::cout << " " << e.path() << '\n';
    }
  } else if (exists(p)) {  // does path p actually exist?
    std::cout << p << " is a special file\n";
  } else {
    std::cout << "path " << p << " does not exist\n";
  }

  std::cout << "----------\n";
  namespace fs= std::filesystem;

  switch (fs::path p { "/home/phytm/Desktop" }; status(p).type()) {
    case fs::file_type::not_found:
      std::cout << "path \"" << p.string() << "\" does not exist\n";
      break;
    case fs::file_type::regular:
      std::cout << '"' << p.string() << "\" exists with " << file_size(p) << " bytes\n";
      break;
    case fs::file_type::directory:
      std::cout << '"' << p.string() << "\" is a directory containing:\n";
      for (auto& e : std::filesystem::directory_iterator { p }) {
        std::cout << " " << e.path().string() << '\n';
      }
      break;
    default:
      std::cout << '"' << p.string() << "\" is a special file\n";
      break;
  }

  create_different_types_of_files();

  return 0;
}
