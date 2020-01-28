#include <stack>
#include <vector>
#include <iostream>
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

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        const std::vector<uint8_t> program = ReadAllBytes(argv[1]);

        std::vector<uint8_t> heap;
        std::stack<uint8_t> stack;

        if (argc > 2)
        {
            heap = ReadAllBytes(argv[2]);
        }
        else
        {
            heap.reserve(4098);
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
}