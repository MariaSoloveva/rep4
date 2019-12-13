#ifndef LZW_PREFIXTREE_HPP
#define LZW_PREFIXTREE_HPP


#include <cstring>
#include <unordered_map>


/// @brief Структура, в которой хранятся все возможные символы, следующие после заданного
struct Node {
    Node* parent;
    unsigned int code_value;
    std::unordered_map<char, Node*> list_of_vertexes;
};

/// @brief Класс префиксного дерева
class PrefixTree {
 private:
    /// @brief Указатель на корневую Node(корень дерева)
    Node* root;
 public:

    /// @brief Конструктор по умолчанию
    PrefixTree()
    {
        root = new Node;
        root->code_value = -1;
        root->parent = nullptr;
    }

    /// @brief Деструктор. Удаляет все элементы дерева
    ~PrefixTree() {
        if (root != nullptr) {
            Node* where = root;
            while (where->list_of_vertexes)
                where = where->Left;
            while (where->Right)
                where = where->Right;
            while (Root->Right || Root->Left)
            {
                where = where->parent;
                if (where->Left)
                {
                    delete where->Left;
                    where->Left = NULL;
                }
                if (where->Right)
                {
                    delete where->Right;
                    where->Right = NULL;
                }
            }
            delete[] root;
            root = nullptr;
        }
    };

    /// @brief Функция проверки присутствия символа character, следующего за заданным нодой n
    /// @param character
    /// @param n
    /// @return Пара: указатель на ноду, которой соответствует символ, и flag - изменился ли указатель(присутствует ли character в дереве)
    std::pair<Node*, bool> find(const char, Node* n = nullptr);

    /// @brief Функция вставка символа после указанного символа
    /// @param character символ, который требуется вставить
    /// @param n указатель на ноду, после которой будет вставлен символ
    void insert(const char, unsigned int, Node*);
};



#endif //LZW_PREFIXTREE_HPP
