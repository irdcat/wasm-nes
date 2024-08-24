#include "NesTestLogParser.hpp"

NesTestLogParser::NesTestLogParser(std::ifstream stream) 
    : nesTestLogStream(std::move(stream))
    , lineNumberCounter(1)
{
}

NesTestLogParser::~NesTestLogParser()
{ 
    nesTestLogStream.close();
}

bool NesTestLogParser::canParseNextLine()
{
    return nesTestLogStream.good() && !nesTestLogStream.eof();
}

NesTestLogData NesTestLogParser::parseNextLine()
{
    static std::string previousLine;

    std::string line;
    std::getline(nesTestLogStream, line);
    std::istringstream lineStream(line);
    NesTestLogData data;
    data.line = line;
    data.previousLine = previousLine;
    data.lineNumber = lineNumberCounter++;

    std::string addressString;
    lineStream >> addressString;
    data.instructionAddress = std::stoul(addressString, nullptr, 16);

    std::string opcodeString;
    lineStream >> opcodeString;
    data.opcode = std::stoul(opcodeString, nullptr, 16);

    std::string hexValue;
    while(lineStream >> hexValue) 
    {
        if (hexValue.find(':') != std::string::npos)
        {
            char label = hexValue[0];
            if(label == 'P' && hexValue[1] == 'P')
            {
                continue;
            }

            std::istringstream hexStream(hexValue.substr(hexValue.find(':') + 1));
            std::string valueString;
            hexStream >> valueString;

            switch(label)
            {
                case 'A': data.registerA = std::stoul(valueString, nullptr, 16); break;
                case 'X': data.registerX = std::stoul(valueString, nullptr, 16); break;
                case 'Y': data.registerY = std::stoul(valueString, nullptr, 16); break;
                case 'P': data.registerP = std::stoul(valueString, nullptr, 16); break;
                case 'S': data.registerS = std::stoul(valueString, nullptr, 16); break;
                case 'C': data.cycleCounter = std::stoul(valueString); break;
                default: break;
            }
        }
    }

    previousLine = line;
    return data;
};