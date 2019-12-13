#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>

#include "BinaryFile.hpp"
#include "PrefixTree.hpp"

#define BITS                       12  // Определяем количество бит, которое выделим для 1 закодированной фразы
#define MAX_CODE                   ( ( 1 << BITS ) - 1 )  // Максимальное количество элементов в словаре(размер словаря)
#define TABLE_SIZE                 5021
#define END_OF_STREAM              256  // Символ конца потока
#define FIRST_CODE                 257  //  Первый свободный символ для новой фразы словаря


///@brief ФункцияСтруктура словаря для алгоритма LZW
struct dictionary
{
    int code_value;
    int prefix_code;
    char character;
}
        dict[TABLE_SIZE];

///@brief ФункцияСтек для декодирования
char decode_stack[TABLE_SIZE];



///@brief Функция сжатия файла
void Compress(FILE* input, BinaryFile* output)
{
    unsigned int next_code = FIRST_CODE;
    PrefixTree tree;
    int string_code;
    Node* where = nullptr;
    int character;

    while ((character = getc(input)) != EOF) {

        auto addition_information = tree.find(character, where);

        if (!addition_information.second) {

            if (next_code < MAX_CODE) {

                tree.insert((char)character, next_code, addition_information.first);
                next_code++;
            }

        } else {
            string_code = character;
            WriteBits(output, (uint)string_code, BITS);
            where = tree.find((char)character, where).first;
        }
    }
    WriteBits(output, (uint)string_code, BITS);
    WriteBits(output, (uint) END_OF_STREAM, BITS);
}

///@brief Процедура декодирования строки. Размещает символы в стеке,
///   возвращает их количество.
uint decode_string(uint count, uint code)
{
    while (code > 255)
    {
        decode_stack[count++] = dict[code].character;
        code = dict[code].prefix_code;
    }
    decode_stack[count++] = (char)code;
    return count;
}


///@brief Функция декодирования сжатого файла
void Decompress(BinaryFile* input, FILE* output)
{
    uint next_code;
    uint new_code;
    uint old_code;
    int character;
    uint count;

    PrefixTree tree;

    next_code = FIRST_CODE;
    old_code = (uint)ReadBits(input, BITS);
    if (old_code == END_OF_STREAM)
        return;

    character = old_code;

    putc(old_code, output);

    while ((new_code = (uint)ReadBits(input, BITS))!= END_OF_STREAM)
    {

        if (new_code >= next_code)
        {
            decode_stack[0] = (char)character;
            count = decode_string(1, old_code);
        }
        else
            count = decode_string( 0, new_code );

        character = decode_stack[count - 1];

        while (count > 0)
            putc(decode_stack[--count], output);
        /* Обновление словаря */
        if (next_code <= MAX_CODE)
        {
            dict[next_code].prefix_code = old_code;
            dict[next_code].character = (char)character;
            next_code++;
        }
        old_code = new_code;
    }
}


///@brief
int main(int argc, char* argv[])
{
    std::string state = "compress";

    std::string s = "/home/mariasolovyova/Рабочий стол/new-client.txt";
    char argv2[s.size() + 1];
    for (int i = 0; i < s.size(); i++) {
        argv2[i] = s[i];
    }
    std::string ss = "/home/mariasolovyova/Рабочий стол/packed.lzw";
    char argv3[ss.size() + 1];
    for (int i = 0; i < s.size(); i++) {
        argv3[i] = ss[i];
    }

    /*std::string s = "/home/mariasolovyova/Рабочий стол/unpacked.txt";
    std::string ss = "/home/mariasolovyova/Рабочий стол/packed.lzw";

    char argv2[ss.size() + 1];

    for (int i = 0; i < ss.size(); i++) {
        argv2[i] = ss[i];
    }

    char argv3[s.size() + 1];
    for (int i = 0; i < s.size(); i++) {
        argv3[i] = s[i];
    }*/

    if (state == "compress")
    {
        FILE* input_file;
        BinaryFile* output_file;

        input_file = fopen(argv2, "rb");
        output_file = CreateOutputBinaryFile(argv3);

        if (input_file != nullptr || output_file != nullptr) {
            printf("\nКомпрессия %s в %s\n", argv2, argv3);

            // вызов процедуры компрессии
            Compress(input_file, output_file);

            // закрытие файлов
            CloseOutputBFile(output_file);
            fclose(input_file);

            printf("\nCompression complete.");

            // вывод коэффициента сжатия
            find_compression_ratio(argv2, argv3);
        } else {
            throw std::invalid_argument("Ошибка при открытии файла для ввода\n");
        }
    }
    else if (state == "decompress")
    {
        BinaryFile* input_file;
        FILE* output_file;

        // открытие входного файла для чтения
        input_file = OpenInputBinaryFile(argv2);

        // открытие выходного файла для записи
        output_file = fopen(argv3, "wb");

        if (input_file != nullptr || output_file != nullptr) {
            printf("\nDecompression %s into %s\n", argv2, argv3);

            // вызов процедуры декомпрессии
            Decompress(input_file, output_file);

            // закрытие файлов
            CloseInputBFile(input_file);
            fclose(output_file);

            printf("\nDecompression complete.");
        } else {
            throw std::invalid_argument("Ошибка при открытии argv3 для ввода\\n\"");
        }
    }

    return 0;

}