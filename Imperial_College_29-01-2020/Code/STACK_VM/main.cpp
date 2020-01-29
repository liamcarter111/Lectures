#include <stack>
#include <vector>
//#include <iostream>
#include <ios>
#include <string>
#include <fstream>

static std::vector<uint8_t> ReadAllBytes(char const *filename)
{
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<uint8_t> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read((char *)result.data(), pos);

    return result;
}

enum Opcode
{
    PUSH = 0x0,
    ADD = 0x1
};

int Execute(const uint8_t *programBytes, const size_t programLength, const uint8_t *memoryBytes = nullptr, const size_t memoryLength = 0)
{
    if (memoryBytes != nullptr && memoryLength > 0)
    {
        const std::vector<uint8_t> program(programBytes, programBytes + programLength);
        std::vector<uint8_t> heap(1024);
        std::stack<uint8_t> stack;

        if (memoryBytes != nullptr && memoryLength > 0)
        {
            heap.assign(memoryBytes, memoryBytes + memoryLength);
        }

        for (int i = 0; i < program.size(); ++i)
        {
            const uint8_t opcode = program[i];
            switch (opcode)
            {
            case PUSH:
            {
                i++; // Align the operand
                const size_t address = (size_t)program[i];
                const uint8_t heapData = heap[address];
                stack.push(heap[address]);

                //std::cout << "PUSH 0x" << std::hex << address
                //          << std::dec << "\n";

                break;
            }

            case ADD:
            {
                const uint8_t rhs = stack.top();
                stack.pop();
                const uint8_t lhs = stack.top();
                stack.pop();
                stack.push(lhs + rhs);

                //std::cout << "ADD"
                //          << "\n";

                break;
            }

            default:
            {
                //std::cout << "ERROR"
                //          << "\n";
                return 1;
            }
            }
        }

        return stack.empty() ? 0 : stack.top();
    }
    return 1;
}

extern "C" int test()
{
    const std::vector<uint8_t> program = ReadAllBytes("program.bin");
    std::vector<uint8_t> heap = ReadAllBytes("memory.bin");
    return Execute(program.data(), program.size(), heap.data(), heap.size());
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        const std::vector<uint8_t> program = ReadAllBytes(argv[1]);

        if (argc > 2)
        {
            std::vector<uint8_t> heap = ReadAllBytes(argv[2]);
            return Execute(program.data(), program.size(), heap.data(), heap.size());
        }
        else
        {
            return Execute(program.data(), program.size(), nullptr, 0);
        }
    }
    return 1;
}
