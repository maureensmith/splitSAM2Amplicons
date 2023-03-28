#include "errors/error.hpp"
#include "files/files.hpp"
#include "lines/line.hpp"


#include <iostream>


auto main(int argc, char** argv) -> int
{
    if (argc < 2) {
        std::cerr << "No input file provided\n";
        auto constexpr NO_INPUT = 1;
        return NO_INPUT;
    } 

    try
    {   
        // first argument is file directory
        auto constexpr INPUT_ARG_DIR = 1;
        auto const* input = argv[INPUT_ARG_DIR];

        if(argc == 5) {
            lines::Line::ampli1_start = std::stoul(argv[2]);
            lines::Line::ampli2_start = std::stoul(argv[3]);
            lines::Line::ampli2_end = std::stoul(argv[4]);
        } else {
            std::cout << "No values given for amplicon starts and end of sequence. Using default values.\n\n";
        }

        std::cout << "\nAmplicon 1 start: " << lines::Line::ampli1_start
                    << "\nAmplicon 2 start: " << lines::Line::ampli2_start
                    << "\nAmplicon 2 end: " << lines::Line::ampli2_end << "\n\n";


        auto files = files::Files::setup(input);
        files.filter();
        auto const [na1, na2, nf] = files.status();
        std::cout << "Total number of read pairs: " << na1 + na2 + nf
                  << "\nNumber of read pairs in ampli1: " << na1
                  << "\nNumber of read pairs in ampli2: " << na2
                  << "\nNumber of read pairs filtered: " << nf
                  << std::endl;
    }
    catch (errors::ErrorId const e)
    {
        auto const code = static_cast<int> (e);
        std::cerr << "ERROR (" << code << "):" << errors::message(e) << std::endl;
        return code;
    }
    catch (std::exception const& e)
    {
        auto const code = static_cast<int> (errors::ErrorId::NOT_COVERED);
        std::cerr << "ERROR (" << code << "): " << e.what() << std::endl;
        return code;
    }
}

