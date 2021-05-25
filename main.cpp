#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stack>

using namespace std;

pair<int,int> cor1;
pair<int,int> cor2;
int Casos;
class RangeTree2D;

vector<pair<int,int>> LeerArchivo(const char *file){

    fstream hola;
    string data;
    int i = 0;
    pair<int,int> cordenaditas1;
    pair<int,int> cordenaditas2;
    pair<int,int> paraelarbolito;
    vector<pair<int,int>> Hayquearbolito;
    int contador = 1;
    int NumerodeVeces;

    hola.open(file,ios::in|ios::out);
    if (hola.is_open()) {

        while(getline(hola,data)) {

            stringstream stream(data);
            string valor;
            while (getline(stream, valor, ' ')) {
                string x, y;

                if(contador == 1){
                    if (i == 0){
                        x = valor;
                        i++;
                        cordenaditas1.first = stoi(x);

                    }
                    else {
                        y = valor;
                        cordenaditas1.second = stoi(y);
                        i--;
                    }

                }
                else if(contador == 2){
                    if (i == 0){
                        x = valor;
                        i++;
                        cordenaditas2.first = stoi(x);

                    }
                    else {
                        y = valor;
                        cordenaditas2.second = stoi(y);

                        i--;
                    }

                }
                else if(contador == 3){
                    NumerodeVeces = stoi(valor);

                }
                else if(contador >= 4){
                    if (i == 0){
                        x = valor;
                        i++;
                        paraelarbolito.first = stoi(x);
                    }
                    else {
                        y = valor;
                        paraelarbolito.second = stoi(y);
                        i--;
                    }

                }

            }
            if(paraelarbolito.first and paraelarbolito.second){
                Hayquearbolito.push_back(paraelarbolito);
            }
            contador++;

        }
    }

    cor1 = cordenaditas1;
    cor2 = cordenaditas2;
    Casos =  NumerodeVeces;
    return Hayquearbolito;

}

class Node {

public:
    // Dato
    pair<int,int> data;
    // Altura
    int height;
    // Hermano izquierdo
    Node *left_child;
    // Papa
    Node *parent;
    // Hermano derecho
    Node *right_child;

    RangeTree2D *subtree;


    //Constructor
    Node(pair<int,int> val);

    // Calculate the balance point.
    int getBalance();

    // Remove the node's parent.
    void removeParent();

    // Set the node's height.
    int updateHeight();

    void printAllNodes(fstream &file);

    void printNodesConexiones(fstream &file);

    void generarsubtree();

};


class RangeTree2D {


private:
    //Raiz
    Node *root;
    bool indice;

private:
    // Balance
    void balanceAtNode(Node *n);


    // Encontrar data


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
    RangeTree2D(bool index);
    RangeTree2D(pair<int,int> valor);

    Node *findNode(int val);

    // Insertar el valor al AVL
    bool insert(pair<int,int> val);

    // Print
    void print();

    void generatePDF();

    void generarArbolesY();

    vector<pair<int,int>> EncontrarCor(pair<int, int> cor1, pair<int, int> cor2);
    bool RecorrerTreeY(Node* subarbol,pair<int,int> &cor1,pair<int,int>& cor2,vector<int> & ValoresEncontrados, vector<pair<int,int>> &resultado );
    bool RecorrerTreeX(Node* subarbol,pair<int,int> &cor1,pair<int,int>& cor2,vector<int> & ValoresEncontrados, vector<pair<int,int>> &resultado);
    //Node* RecorrerTreeSplit(Node* subarbol, Node* split);
};



//Inicializo el primer nodo
Node::Node(pair<int,int> valor) {
    data = valor;
    height = 0;
    parent = nullptr;
    left_child = nullptr;
    right_child = nullptr;
}


int Node::getBalance() {

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
void Node::removeParent() {
    parent = nullptr;
}

int Node::updateHeight() {

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


RangeTree2D::RangeTree2D(bool index) {
    root = nullptr;
    indice = index;
}

// Constructor con un valor
RangeTree2D::RangeTree2D(pair<int,int> val) {
    root = new Node(val);
}

// Balance del arbol
void RangeTree2D::balanceAtNode(Node *n) {

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


vector<pair<int,int>> RangeTree2D::EncontrarCor(pair<int, int> cor1, pair<int, int> cor2) {

    Node *temp = root;

    vector<int> ValoresEncontradosX;
    vector<int> ValoresEncontradosY;
    vector<pair<int,int>> Resultado;

    vector<Node*> SplitsCor1;
    vector<Node*> SplitsCor2;
    Node* SplitDefinitivo;
    string lado;

    while(temp != nullptr){
        auto Papa = temp;

        if (cor1.first >= temp->data.first)
        {
            SplitsCor1.push_back(Papa);
            temp = temp->right_child;
        }

        else{
            SplitsCor1.push_back(Papa);
            temp = temp->left_child;
        }

    }
    temp = root;

    while(temp != nullptr){
        auto Papa = temp;

        if (cor2.first >= temp->data.first)
        {
            SplitsCor2.push_back(Papa);
            temp = temp->right_child;
        }

        else{
            SplitsCor2.push_back(Papa);
            temp = temp->left_child;
        }

    }

    temp = root;

    bool salio = false;

    //Obtengo el split definitivo
    for(int i = 0; i < SplitsCor1.size() ; i++ ){
        for(int j = 0; j < SplitsCor2.size() ; j++ ){
            if(SplitsCor1[i] == SplitsCor2[j]) {
                SplitDefinitivo = SplitsCor1[i];
                i++;
                continue;
            }
            if(SplitsCor1[i] != SplitsCor2[j]){
                salio = true;
                break;
            }
        }
        if(salio){
            break;
        }
    }
    temp = root;


while(temp != nullptr){
    if(temp == SplitDefinitivo)
    {

        auto BuscarY = root->subtree->root;
        temp = root;
        RecorrerTreeY(BuscarY,cor1,cor2,ValoresEncontradosY,Resultado);
        RecorrerTreeX(temp,cor1,cor2,ValoresEncontradosX,Resultado);
        break;

    }
    else if(temp->right_child->data.first > SplitDefinitivo->data.first){
        temp = temp->left_child;
    }
    else if(temp->left_child->data.first < SplitDefinitivo->data.first){
        temp = temp->right_child;
    }

}
    return Resultado;
}

bool RangeTree2D::RecorrerTreeY(Node* subarbol,pair<int,int> &cor1,pair<int,int>& cor2,vector<int> & ValoresEncontrados,vector<pair<int,int>> & resultado )
{
    if (subarbol->right_child == nullptr or subarbol->left_child == nullptr){

        if((cor1.second <= subarbol->data.second) and (subarbol->data.second <= cor2.second)){
            if ((cor1.first <= subarbol->data.first) and (subarbol->data.first <= cor2.first)){

                ValoresEncontrados.push_back(subarbol->data.second);
            }

        }
        return true;
    }
    RecorrerTreeY(subarbol->left_child,cor1, cor2, ValoresEncontrados,resultado);
    RecorrerTreeY(subarbol->right_child,cor1, cor2, ValoresEncontrados,resultado);

}

bool RangeTree2D::RecorrerTreeX(Node* subarbol,pair<int,int> &cor1,pair<int,int>& cor2,vector<int> & ValoresEncontrados,vector<pair<int,int>> & resultado)
{
    if (subarbol->right_child == nullptr or subarbol->left_child == nullptr){

        if ((cor1.first <= subarbol->data.first) and (subarbol->data.first <= cor2.first)){
            if ((cor1.second <= subarbol->data.second) and (subarbol->data.second <= cor2.second)){
                resultado.push_back(subarbol->data);
                ValoresEncontrados.push_back(subarbol->data.first);
            }
        }
        return true;
    }
    RecorrerTreeX(subarbol->left_child,cor1, cor2, ValoresEncontrados,resultado);
    RecorrerTreeX(subarbol->right_child,cor1, cor2, ValoresEncontrados,resultado);

}




// Encontrar el nodo que tiene la data
Node* RangeTree2D::findNode(int val) {

    Node *temp = root;

    while (temp != nullptr) {

        if (val == temp->data.first)
        {
            break;
        }

        else if (val < temp->data.first)
        {
            temp = temp->left_child;
        }

        else{
            temp = temp->right_child;
        }

    }
    return temp;
}

bool RangeTree2D::insert(pair<int,int> val) {

    // Si el arbol esta vacio, añado el nuevo valor al root
    if (root == nullptr)
    {
        root = new Node(val);
    }

    else if (!indice){

        Node *added_node = nullptr;

        Node *temp = root;

        while (temp != nullptr && added_node == nullptr) {

            if (val.first < temp->data.first)
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
            else if (val.first >= temp->data.first)
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
    else
    {
        Node *added_node = nullptr;

        Node *temp = root;

        while (temp != nullptr && added_node == nullptr) {

            if (val.second < temp->data.second)
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
            else if (val.second > temp->data.second)
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
            else
            {
                return false;
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

void Node::generarsubtree()
{
    stack<Node *> s;
    Node *curr = this;
    this->subtree = new RangeTree2D(1);

    while (curr != NULL || s.empty() == false)
    {
        while (curr !=  NULL)
        {
            s.push(curr);
            curr = curr->left_child;
        }

        curr = s.top();
        s.pop();

        //    cout << curr->data << " ";
        this->subtree->insert(curr->data);
        /* we have visited the node and its
           left subtree.  Now, it's right
           subtree's turn */
        curr = curr->right_child;

    } /* end of while */


}

void RangeTree2D::generarArbolesY()
{

    stack<Node *> s;
    Node *curr = this->root;

    while (curr != NULL || s.empty() == false)
    {
        while (curr !=  NULL)
        {
            s.push(curr);
            curr = curr->left_child;
        }

        curr = s.top();
        s.pop();

        curr->generarsubtree();

        curr = curr->right_child;

    } /* end of while */


}

void RangeTree2D::print() {

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


void RangeTree2D::printSubtree(Node *subtree, int depth,
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
                  subtree->data.second;
}


// Rotacion Izquierda
void RangeTree2D::rotateLeft(Node *n) {

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
    n->parent = temp;
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
void RangeTree2D::rotateRight(Node *n) {
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



    if (tempNode != nullptr) {
        tempNode->parent = temp->parent;
    }

    n->left_child = tempNode;
    n->parent = temp;
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


int RangeTree2D::spacing(int level) {
    int result = 2;
    for (int i = 0; i < level; i++)
        result += result;
    return result;
}

void Node :: printAllNodes(fstream &file){
    file << "\"" << this << "\" [\n";
    file << "\tlabel = \"" << "(" << this->data.first << ", " << this->data.second << ")"<< " \"\n]\n";
    if(this->left_child != nullptr){
        this->left_child->printAllNodes(file);
    }
    if(this->right_child != nullptr){
        this->right_child->printAllNodes(file);
    }
}

void Node :: printNodesConexiones(fstream &file){
    if(this->left_child != nullptr){
        file << "\"" << this << "\"->";
        file << "\"" << left_child << "\";\n";
        this->left_child->printNodesConexiones(file);
    }
    if(this->right_child != nullptr){
        file << "\"" << this << "\"->";
        file << "\"" << right_child << "\";\n";
        this->right_child->printNodesConexiones(file);
    }
}

void RangeTree2D::generatePDF() {
    fstream file("graph.vz", fstream::out | fstream::trunc);
    if (file.is_open()) {
        file << "digraph G\n";
        file << "{\n";
        root->printAllNodes(file);
        root->printNodesConexiones(file);
        file << "}\n";
        file.close();
        system("dot -Tpdf graph.vz -o graph.pdf");
    }
}




int main() {

    auto tree = new RangeTree2D(0);

    //Cambiar en la funcion leer Archivo por el nombre del archivo que deseamos leer, este
    //Siguiendo el formato presentado en el laboratorio03
    vector<pair<int,int>> coordenadas = LeerArchivo("dataset100.txt");

    map<int, int> repetidos;

    for(auto it : coordenadas)
    {
/*         if(repetidos[it.first] == 0)
        {    tree->insert(it);
             repetidos[it.first] = 1;
        } */
        tree -> insert(it);
    }

    tree->generarArbolesY();

    auto resultaditoFinalfinal = tree->EncontrarCor(cor1,cor2);

    for(auto it:resultaditoFinalfinal){
        cout << it.first << " " << it.second << endl;
    }

    tree->generatePDF();
    tree->findNode(19)->subtree->print();


    return 0;
}