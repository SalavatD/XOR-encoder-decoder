#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

void printAbout()
{
    cout << "About:" << endl;
    cout << "    Utility to encode/decode files with XOR" << endl;
    cout << "    Version 1.0" << endl;
    cout << "    Author: Salavat Dautov" << endl;
    cout << "    Creation date: July 2016" << endl;
}

void printUsage(string executableName)
{
    cout << "Usage:" << endl;
    cout << "    " << executableName << " [options]" << endl;
    cout << "Options:" << endl;
    cout << "    -i --input  <file> Input file path" << endl;
    cout << "    -o --output <file> Output file path" << endl;
    cout << "    -k --key    <key>  Encryption key" << endl;
    cout << "Or print help message:" << endl;
    cout << "    " << executableName << " [-h | --help]" << endl;
}

void printHelp(string executableName)
{
    printAbout();
    printUsage(executableName);
}

void printError(string error)
{
    cout << error << endl;
}

void encryptOrDecryptFile(string inputFilePath, string outputFilePath, string encryptionKey)
{
    ifstream fin(inputFilePath.c_str(), ios::binary | ios::in);
    fin.seekg(0, ios::end);
    unsigned long long inputFileSize = fin.tellg();
    fin.seekg(0, ios::beg);
    unsigned char *data = new unsigned char[inputFileSize];
    for (unsigned long long i = 0; i < inputFileSize; i++)
    {
        fin >> data[i];
    }
    fin.close();
    ofstream fout(outputFilePath.c_str(), ios::binary | ios::out);
    const unsigned char *keyArray = reinterpret_cast<const unsigned char *>(encryptionKey.c_str());
    for (unsigned long long i = 0; i < inputFileSize; i++)
    {
        fout << (unsigned char) (data[i] ^ keyArray[i % encryptionKey.length()]);
    }
    fout.close();
    delete[] data;
}

void parseArgs(int argc, char **argv)
{
    string inputFilePath;
    string outputFilePath;
    string encryptionKey;

    string executablePath(argv[0]);
    string executableName(executablePath.substr(executablePath.find_last_of("/\\") + 1));

    if (argc == 1)
    {
        printUsage(executableName);
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
        {
            printHelp(executableName);
        }
        else
        {
            printError("Not enough arguments.");
            printUsage(executableName);
        }
    }
    else if (argc < 7)
    {
        printError("Not enough arguments.");
        printUsage(executableName);
    }
    else if (argc > 7)
    {
        printError("Too many arguments.");
        printUsage(executableName);
    }
    else
    {
        for (int i = 0; i < argc; i++)
        {
            if (i + 1 < argc)
            {
                if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0)
                {
                    inputFilePath = argv[i + 1];
                }
                else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
                {
                    outputFilePath = argv[i + 1];
                }
                else if (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--key") == 0)
                {
                    encryptionKey = argv[i + 1];
                }
            }
        }
        if (inputFilePath.empty() || outputFilePath.empty() || encryptionKey.empty())
        {
            printError("Undefined arguments.");
            printUsage(executableName);
        }
        else
        {
            encryptOrDecryptFile(inputFilePath, outputFilePath, encryptionKey);
        }
    }
}

int main(int argc, char **argv)
{
    parseArgs(argc, argv);
    return 0;
}
