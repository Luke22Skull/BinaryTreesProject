#include <exception>
#include <iostream>
#include <stdexcept>
// #include <string>

//Questa classe di albero binario pretende lo sviluppo dalla RADICE dell'albero, fino alle foglie
//Quindi, prevede la costruzione dall'alto verso il basso
template <class T>
class BinTree
{
private:
    struct BinTreeNode
    {
        T data;
        BinTreeNode *left;
        BinTreeNode *right;

        explicit BinTreeNode (T value) {
            data = value;
            left = nullptr;
            right = nullptr;
        }

		BinTreeNode () {
			data = T{};
			left = nullptr;
			right = nullptr;
		}
    };

    BinTreeNode *root;


	void printInOrder(const BinTreeNode* currentNode) const {
		if (!currentNode) return;  // Caso base: nodo nullo (fine ricorsione)
		printInOrder(currentNode->left);  // Visita il sottoalbero sinistro
		std::cout << currentNode->data << " ";  // Stampa il valore del nodo corrente
		printInOrder(currentNode->right);  // Visita il sottoalbero destro
	}

public:
    explicit BinTree(T value) : root(new BinTreeNode(value)) {};    //costruttore della radice col valore
    BinTree() : root(nullptr) {};                                   //costruttore vuoto
    // ~BinTree() {};							//eliminiamo l'intero albero con la funzione
												//dell'eliminazione del sottoalbero a partire
												//dalla root dell'intero albero
	// stampa dell'albero
	void printTree () const {
		printInOrder(this->root);
		std::cout << "\n" << std::endl;
	}

	//funzioni booleane
    bool isEmptyTree() {return this->root == nullptr;};
    bool isEmptyDx(const BinTreeNode* currentNode) {return currentNode->right == nullptr;};
    bool isEmptySx(const BinTreeNode* currentNode) {return currentNode->left == nullptr;};
	bool isEmptyNode(const BinTreeNode* currentNode) {return isEmptyDx(currentNode) &&
														isEmptySx(currentNode);};

	// Metodo per impostare il valore di un nodo (se il nodo creato dovesse essere un nodo vuoto)
    void setNodeData(BinTreeNode *currentNode, T value) {
        if (!currentNode) throw std::invalid_argument("Nodo nullo.");
        currentNode->data = value;
    }

	T readNodeData(BinTreeNode *currentNode) const {return currentNode->data;};	//legge il valore del
																				//nodo corrente
	BinTreeNode* getRoot() const {return this->root;};	//ritorna un puntatore alla root
														//questa funzione mi permette di accedere
														//in tempo O(1) alla root dell'albero intero

    //funzioni di inserimento
    BinTreeNode* insertFather(BinTreeNode* currentNode) {	//inserisce un padre con valore "vuoto"
															//al nodo corrente di input
        if (!currentNode)
            throw std::invalid_argument("Puntatore nullo fornito.");
        BinTreeNode* newRoot = new BinTreeNode(T{}); // Creazione di un nuovo nodo con valore di default
        newRoot->left = currentNode;
        return newRoot;
    }

    BinTreeNode* insertChildDx(BinTreeNode* currentNode) {	//le due funzioni di inserimento del
															//figlio destro e sinistro creano
															//un nodo figlio con valore default
															//Serve la funzione di scrittura del
															//nodo per scrivere il valore di esso
		if (!currentNode)
			throw std::invalid_argument("Puntatore nullo");
		else if (!isEmptyDx(currentNode)) {
			// std::cout << "passo all'altro nodo a destra" << std::endl;
			return insertChildDx(currentNode->right);	//ricorsività nel caso in cui il nodo destro del nodo
														//corrente dovesse essere vuoto
		}
		BinTreeNode* newChildNode = new BinTreeNode();	//nodo vuoto di default
		currentNode->right = newChildNode;	//il puntatore destro del current node punta al nodo
											//appena creato
		return newChildNode;	//ritorniamo il puntatore creato
	}

    BinTreeNode* insertChildSx(BinTreeNode* currentNode) {  //le due funzioni di inserimento del
															//figlio destro e sinistro creano
															//un nodo figlio con valore default
															//Serve la funzione di scrittura del
															//nodo per scrivere il valore di esso

		if (!currentNode)
			throw std::invalid_argument("Puntatore nullo");
		else if (!isEmptySx(currentNode)) {
			// std::cout << "passo all'altro nodo a sinistra" << std::endl;
			return insertChildSx(currentNode->left);	//ricorsività col nodo sinistro
			// std::cout << "currentNode val: " << currentNode->data << std::endl;
		}

		// std::cout << "currentNode val: " << currentNode->data << std::endl;
		BinTreeNode* newChildNode = new BinTreeNode();
		// std::cout << "creato nuovo nodo" << std::endl;
		currentNode->left = newChildNode;
		// std::cout << "il figlio sinistro del nodo con valore " << currentNode->data << " e' il nuovo nodo" << std::endl;
		return newChildNode;
	}

	BinTree uniteTrees(BinTree& T1, BinTree& T2) {
		// Crea un nuovo albero con una radice vuota
		BinTree<T> unionTree(T{});

		// Controlla se gli alberi T1 e T2 non sono vuoti
		if (!T1.isEmptyTree() && !T2.isEmptyTree()) {

			// Collega T1 come sottoalbero sinistro e T2 come sottoalbero destro
			unionTree.root->left = T1.root;
			unionTree.root->right = T2.root;

			// Invalida i puntatori alle radici degli alberi originali
			T1.root = nullptr;
			T2.root = nullptr;

		} else {
			throw std::invalid_argument("Entrambi gli alberi devono essere non vuoti per l'unione.");
		}

		return unionTree;
	}

	BinTreeNode* deleteSubBinTree (BinTreeNode* currentNode) {	//la funzione ha il compito di
																 //eliminare un sottoalbero a partire da
																 //un nodo di esso senza eliminare
																 //il nodo di input
		
		std::cout << " currentNode val: " << readNodeData(currentNode) << std::endl;	//print del valore di
																						//currentNode

		if (!currentNode) {	//check se il nodo di input è nullo
			this->printTree();	//stampa l'albero (debug)
			throw std::invalid_argument("nodo nullo fornito");
		}

		BinTreeNode** travelerPointer = &currentNode;	//creiamo un DOPPIO PUNTATORE a currentNode
														//il cui VALORE corrisponde al currentNode
														//e che PUNTA all'indirizzo del currentNode
		
		std::cout << " traveler pointer val: " << readNodeData(*travelerPointer) << std::endl;	//stampa del valore del
																								//travelerPointer
																								//(debug)

		while ((*travelerPointer)->left || (*travelerPointer)->right) {	//ECCO COME EVITARE IL SEG. FAULT:
																		//al posto di checkare i sottoalberi
																		//del valore del travelerPointer
																		//(ovvero il currentNode) con un IF,
																		//usiamo le leggi di De Morgan e un
																		//ciclo while, che si ripeterà
																		//fin quando entrambi i nodi left
																		//e right del travelerPointer non
																		//saranno nulli
			if ((*travelerPointer)->left) {	//se il nodo non-nullo dovesse essere il sinistro,
											//il trav.ptr punterà all'INDIRIZZO DI MEMORIA DEL
											//CAMPO left DEL PROPRIO VALORE
											// (è l'unico modo per farlo scalare in giù a sinistra di
											// una posizione, dato che si tratta di un doppio puntatore)
				travelerPointer = &(*travelerPointer)->left;
				std::cout << " traveler pointer val (left) : " << readNodeData(*travelerPointer) << std::endl;
			}
			
			else if ((*travelerPointer)->right) {	//medesima cosa con il campo right
													// (ma tanto li checka entrambi perché poi
													// operiamo la ricorsione)
				travelerPointer = &(*travelerPointer)->right;
				std::cout << " traveler pointer val (right) : " << readNodeData(*travelerPointer) << std::endl;
			}
		}

		/* === FINE DEL WHILE == 
		 * a questo punto il travelerPointer è
		 * - senza nodi figli
		 * - esistente e con un valore
		 */

		if (*travelerPointer == currentNode)	//se il valore del trav.ptr dovesse essere uguale al
												//currentNode iniziale, allora il codice si ferma,
												//poiché avremmo eliminato tutti i sottoalberi
												//al di sotto di currentNode
			return currentNode;

		else {	//ALTRIMENTI, eliminiamo IL VALORE di travelerPointer, per poi settarlo a
				//nullptr in modo tale da evitare dangling points
			
			delete *travelerPointer;
			*travelerPointer = nullptr;
			return deleteSubBinTree(currentNode);	//chiamata ricorsiva della funzione sul currentNode (non modificato)
		}
	}
};

int main() {
    try {
        std::cout << " ===== Test dell'albero binario: ===== \n";

        // 1. Test del costruttore vuoto
        BinTree<int> emptyTree;
        std::cout << "Albero vuoto creato correttamente: " << (emptyTree.isEmptyTree() ? "Sì" : "No") << "\n";

        // 2. Test del costruttore con radice
        BinTree<int> tree(10);
        std::cout << "Radice dell'albero inizializzata con valore 10: " << (tree.readNodeData(tree.getRoot()) == 10 ? "Sì" : "No") << "\n";

        // 3. Test inserimento figlio sinistro
        auto* leftChild = tree.insertChildSx(tree.getRoot());
        tree.setNodeData(leftChild, 5);
        std::cout << "Figlio sinistro aggiunto con valore 5: " << (tree.readNodeData(leftChild) == 5 ? "Sì" : "No") << "\n";

        // 4. Test inserimento figlio destro
        auto* rightChild = tree.insertChildDx(tree.getRoot());
        tree.setNodeData(rightChild, 15);
        std::cout << "Figlio destro aggiunto con valore 15: " << (tree.readNodeData(rightChild) == 15 ? "Sì" : "No") << "\n";
		
        // 5. Test unione di due alberi
        BinTree<int> tree1(20);
        BinTree<int> tree2(30);
        BinTree<int> unionTree = tree.uniteTrees(tree1, tree2);
		
		std::cout << "Unione completata correttamente: \n";
        std::cout << "Radice dell'albero unito: " << unionTree.readNodeData(unionTree.getRoot()) << "\n";
        std::cout << "Radice del sottoalbero sinistro: " << unionTree.readNodeData(unionTree.getRoot()->left) << "\n";
        std::cout << "Radice del sottoalbero destro: " << unionTree.readNodeData(unionTree.getRoot()->right) << "\n";

		// 7. Test lettura dell'albero
		tree.printTree();

        // 6. Test lettura e modifica di un nodo
        tree.setNodeData(rightChild, 25);
        std::cout << "Valore del figlio destro aggiornato a 25: " << (tree.readNodeData(rightChild) == 25 ? "Sì" : "No") << "\n";

		// 7.1 Test stampa dell'albero
		tree.printTree();
		
		// 8. Test di inserimento di un altro figlio sinistro
		auto* secondLeftChild = tree.insertChildSx(tree.getRoot());
		tree.setNodeData(secondLeftChild, 30);	//voglio creare un figlio sinistro sotto al
												//figlio sinistro di root (che ha valore 5)

		// 7.2 Test stampa dell'albero
		tree.printTree();

		// 8.1 Test di inserimento di un ALTRO figlio sinistro
		auto* anotherLeftChild = tree.insertChildSx(tree.getRoot());
		tree.setNodeData(anotherLeftChild, 33);

		// 7.3 Test stampa dell'albero
		tree.printTree();
	

		// 8.2 Test di inserimento di un ALTRO figlio sinistro
		auto* second_anotherLeftChild = tree.insertChildSx(tree.getRoot());
		tree.setNodeData(second_anotherLeftChild, 70);

		// 8.3 Test di inserimento di un ALTRO figlio sinistro
		auto* third_anotherLeftChild = tree.insertChildSx(tree.getRoot());
		tree.setNodeData(third_anotherLeftChild, 71);

		// 7.4 Test stampa dell'albero
		tree.printTree();

		std::cout << "nodo sx della root: " << tree.readNodeData(tree.getRoot()->left) << std::endl;

		// 9. Test dell'eliminazione di un sottoalbero (in questo caso quello sinistro con radice 5)
		auto* node = tree.deleteSubBinTree(tree.getRoot()->left);
		tree.readNodeData(node);

		// 7.5 Test stampa dell'albero
		tree.printTree();

    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << "\n";
    }
		
    return 0;
}
