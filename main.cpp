#include <iostream>
#include <iomanip>

using namespace std;

class AVLTree {

private:
    //Clase NODO
    class Node {

    public:
        // Dato
        int data;
        // Altura
        int height;
        // Hermano izquierdo
        Node *left_child;
        // Papa
        Node *parent;
        // Hermano derecho
        Node *right_child;

        //Constructor
        Node(int val);

        // Calculate the balance point.
        int getBalance();

        // Remove the node's parent.
        void removeParent();

        // Set the node's height.
        int updateHeight();

    };


private:
    //Tengo al papa
    Node *root;

private:
    // Balance
    void balanceAtNode(Node *n);


    // Encontrar data
    Node *findNode(int val);


    // Print
    void printSubtree(Node *subtree, int depth, int offset, bool first);

    // Rotate izquierda.
    void rotateLeft(Node *n);

    // Rotate derecha.
    void rotateRight(Node *n);


    // Figure out the default spacing for element.
    int spacing(int offset);


public:
    // Constructor
    AVLTree();
    AVLTree(int valor);

    // Insertar el valor al AVL
    bool insert(int val);

    // Print
    void print();


};



//Inicializo el primer nodo
AVLTree::Node::Node(int valor) {
    data = valor;
    height = 0;
    parent = nullptr;
    left_child = nullptr;
    right_child = nullptr;
}


int AVLTree::Node::getBalance() {

    int result;
    if (left_child == nullptr)
    {
        // Si no tiene ningun subarbol derecho retorna zero
        if (right_child == nullptr)
        {
            result = 0;
        }
        else {
            result = -right_child->height-1;
        }

    }
    else if (right_child == nullptr) {

        result = left_child->height+1;
    }
        //Mismo tamaño de los dos hijos, resto el tamaño de los dos
    else {

        result = left_child->height-right_child->height;
    }

    return result;
}


//Remover al papa
void AVLTree::Node::removeParent() {
    parent = nullptr;
}

int AVLTree::Node::updateHeight() {

    if (left_child == nullptr)
    {
        if (right_child == nullptr){
            height = 0;
        }

        else{
            height = right_child->height+1;
        }
    }
    else if (right_child == nullptr)
    {
        height = left_child->height+1;
    }

    else if (left_child->height > right_child->height)
    {
        height = left_child->height+1;
    }
    else {
        height = right_child->height+1;

    }
    return height;
}


// Constructor por defecto
AVLTree::AVLTree() {
    root = nullptr;
}

// Constructor con un valor
AVLTree::AVLTree(int val) {
    root = new Node(val);
}

// Balance del arbol
void AVLTree::balanceAtNode(Node *n) {

    int bal = n->getBalance();

    if (bal > 1) {
        if (n->left_child->getBalance() < 0)
        {
            rotateLeft(n->left_child);
        }
        rotateRight(n);


    } else if (bal < -1) {
        if (n->right_child->getBalance() > 0) {
            rotateRight(n->right_child);
        }

        rotateLeft(n);
    }
}


// Encontrar el nodo que tiene la data
AVLTree::Node *AVLTree::findNode(int val) {

    Node *temp = root;

    while (temp != nullptr) {

        if (val == temp->data)
        {
            break;
        }

        else if (val < temp->data)
        {
            temp = temp->left_child;
        }

        else{
            temp = temp->right_child;
        }

    }
    return temp;
}

bool AVLTree::insert(int val) {

    // Si el arbol esta vacio, añado el nuevo valor al root
    if (root == nullptr)
    {
        root = new Node(val);
    }

    else {

        Node *added_node = nullptr;

        Node *temp = root;

        while (temp != nullptr && added_node == nullptr) {

            if (val < temp->data)
            {

                if (temp->left_child == nullptr)
                {
                    auto *tempNode = new Node(val);

                    tempNode->parent = temp;
                    temp->left_child = tempNode;
                    temp->updateHeight();
                    added_node = temp->left_child;

                    if(temp->right_child == nullptr)
                    {
                        auto *tempNodeRangetree = new Node(val);
                        tempNodeRangetree->parent = tempNode;
                        tempNode->right_child = tempNodeRangetree;
                        tempNodeRangetree->updateHeight();
                        added_node = tempNode->right_child;

                    }

                }
                else{
                    temp = temp->left_child;
                }


            }
            else if (val >= temp->data)
            {
                if (temp->right_child == nullptr)
                {

                    auto *tempNode = new Node(val);
                    tempNode->parent = temp;
                    temp->right_child = tempNode;
                    temp->updateHeight();
                    added_node = temp->right_child;

                    if(temp->left_child == nullptr)
                    {
                        auto *tempNodeRangetree = new Node(temp->data);
                        tempNodeRangetree->parent = tempNode;
                        tempNode->left_child = tempNodeRangetree;
                        tempNodeRangetree->updateHeight();
                        added_node = tempNode->left_child;
                    }

                }
                else{
                    temp = temp->right_child;
                }

            }
        }
        temp = added_node;
        while(temp != nullptr)
        {
            temp->updateHeight();
            balanceAtNode(temp);
            temp = temp->parent;
        }
    }
    return true;
}


void AVLTree::print() {

    if (root == nullptr)
        std::cout << "Tree is empty!" <<
                  std::endl;


    else if (root->height > 4)
        std::cout << "Not currently supported!" <<
                  std::endl;


    else {
        int max = root->height;
        for (int depth = 0; depth <= max; depth++) {
            printSubtree(root, depth, max-depth+1, true);
            std::cout << std::endl;
        }
    }
}


void AVLTree::printSubtree(Node *subtree, int depth,
                           int level, bool first) {

    if (depth > 0) {
        if (subtree == nullptr) {
            printSubtree(subtree, depth-1, level, first);
            printSubtree(subtree, depth-1, level, false);
        } else {
            printSubtree(subtree->left_child, depth-1,
                         level, first);
            printSubtree(subtree->right_child, depth-1,
                         level, false);
        }


    } else if (subtree == nullptr)
        std::cout << std::setw((first)?
                               spacing(level)/2:spacing(level)) << "-";

    else
        std::cout << std::setw((first)?
                               spacing(level)/2:spacing(level)) <<
                  subtree->data;
}


// Rotacion Izquierda
void AVLTree::rotateLeft(Node *n) {

    string lado;

    Node *p = n->parent;

    if (p != nullptr && p->left_child == n)
    {
        lado = "left";
    }

    else{
        lado = "right";
    }


    Node *temp = n->right_child;

    auto tempNode = temp->left_child;

    if (tempNode != nullptr) {
        tempNode->parent = temp->parent;
    }

    n->right_child = tempNode;
    n->updateHeight();


    temp->left_child = n;
    temp->updateHeight();


    if (p == nullptr)
    {
        root = temp;
        if(root != nullptr){
            root->removeParent();
        }
    }

    else if (lado == "left"){
        temp->parent = p;

        p->left_child = temp;
        p->updateHeight();
    }

    else {
        temp->parent = p;

        p->right_child = temp;
        p->updateHeight();

    }

}

// Rotacion Derecha
void AVLTree::rotateRight(Node *n) {
    string lado;

    Node *p = n->parent;

    if (p != nullptr && p->left_child == n)
    {
        lado = "left";
    }

    else{
        lado = "right";
    }

    Node *temp = n->left_child;

    auto tempNode = temp->right_child;

    if(tempNode )


    if (tempNode->left_child != nullptr) {
        tempNode->parent = temp->parent;
    }

    n->left_child = tempNode;
    n->updateHeight();


    temp->right_child = n;
    temp->updateHeight();


    if (p == nullptr){
        root = temp;
        if(root != nullptr)
        {
            root->removeParent();
        }

    }

    else if (lado == "left")
    {
        temp->parent = p;

        p->left_child = temp;
        p->updateHeight();
    }

    else{
        temp->parent = p;

        p->right_child = temp;
        p->updateHeight();
    }

}


int AVLTree::spacing(int level) {
    int result = 2;
    for (int i = 0; i < level; i++)
        result += result;
    return result;
}




int main() {

    AVLTree *tree = new AVLTree();

    tree->insert(6);
    tree->insert(17);
    tree->insert(20);
    tree->insert(15);

    tree->print();

    //Poner las referencias bibliograficas(la implementacion y usamos sus dos
    // funciones de impresion para poder tener una mejor apreciacion de los resultados)


    return 0;
}