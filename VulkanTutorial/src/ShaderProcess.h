#include <fstream>
#include <vector>

class ShaderProcess
{
public:
    static void compileShader(const std::string& fileName);

    static std::string getRootPath();
    static std::vector<char> readFile(const std::string& filename);
};
